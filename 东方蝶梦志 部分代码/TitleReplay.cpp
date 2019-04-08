#include "NuzPrecompiled.h"
#include "TitleReplay.h"
#include "SeManager.h"
#include "GetPath.h"
#include "MovieData.h"
#include "Sprite2DWStreamLayout.h"
#include "TitleToGame.h"
#include "ReplayerActivity.h"
#include <shellapi.h>
using namespace std;

extern Basic::Video::Renderer2D::CtxFont scFont;

using namespace  Activities::Title;

class ReplayItem : public Basic::Menu::MenuItem
{
private:
	std::string m_path;

	Basic::Video::Renderer2D::Sprite2DWStreamLayout m_playerName, m_char,m_score, m_lastStage, m_level;
	Basic::SecTweenData<float> m_y;
	Basic::SecTweenData<float> m_alpha;
	
	TitleMainMenu * const m_menuManager;

	Basic::SecTaskList m_tl;
public:
	ReplayItem(std::string&& path, TitleMainMenu* menuManager):
		m_path { std::move(path) },
		m_playerName{{-400,0,640,96}},
		m_char{{0,0,640,96}},
		m_score{{0,0,640,96},Basic::Video::Renderer2D::Sprite2DWStreamLayout::LayoutMode::RightTop},
		m_lastStage{{0,0,640,96}},
		m_level{{0,0,640,96}},
		m_menuManager{ menuManager }
	{
		MovieData::Head movieHead;
		std::ifstream in(m_path, std::ios::binary);
		in.read((char*)&movieHead, sizeof(movieHead));
		in.close();

		std::wstring playerName;
		for (wchar_t ch : movieHead.playerName)
		{
			if (ch == L'\0')
				break;
			else
			{
				if (ch == L'_')
					playerName += L' ';
				else
					playerName += ch;
			}
		}

		scFont.SetSprite2DWStreamToString(playerName, m_playerName);
		switch (movieHead.charater)
		{
		case decltype(movieHead.charater)::Reimu:
			scFont.SetSprite2DWStreamToString(L"Reimu", m_char);
			break;
		case decltype(movieHead.charater)::Marisa:
			scFont.SetSprite2DWStreamToString(L"Marisa", m_char);
			break;
		default:
			Crash(L"未能读取录像文件，角色数据损坏。");
			break;
		}

		scFont.SetSprite2DWStreamToString(std::to_wstring(movieHead.score),m_score);
		scFont.SetSprite2DWStreamToString(movieHead.lastStage, m_lastStage);

		if (movieHead.isExtra)
		{
			scFont.SetSprite2DWStreamToString(L"Extra", m_level);
		}
		else
		{
			const std::array<std::wstring,4> lv = { L"Easy",L"Normal",L"Hard",L"Lunatic" };
			scFont.SetSprite2DWStreamToString(lv.at((int)movieHead.level), m_level);
		}

		m_playerName.SetSpriteSizeOffset(10, 0);
		m_char.SetSpriteSizeOffset(10, 0);
		m_score.SetSpriteSizeOffset(10, 0);
		m_lastStage.SetSpriteSizeOffset(10, 0);
		m_level.SetSpriteSizeOffset(10, 0);
		m_alpha = 0;
	}

	void Draw() const override
	{
		m_playerName.Draw();
		m_char.Draw();
		m_score.Draw();
		m_lastStage.Draw();
		m_level.Draw();
	}

	void Update(float t) override
	{
		m_alpha.Update(t);
		m_y.Update(t);

		m_playerName.SetRect({ -590,(int)(float)m_y,640,92 });
		for (auto& p : m_playerName)
			p->SetAlpha(m_alpha);

		m_char.SetRect({ -340,(int)(float)m_y,640,92 });
		for (auto& p : m_char)
			p->SetAlpha(m_alpha);

		m_score.SetRect({ 0,(int)(float)m_y,590,92 });
		for (auto& p : m_score)
			p->SetAlpha(m_alpha);

		m_lastStage.SetRect({ -150,(int)(float)m_y,400,92 });
		for (auto& p : m_lastStage)
			p->SetAlpha(m_alpha);

		m_level.SetRect({ 70,(int)(float)m_y,400,92 });
		for (auto& p : m_level)
			p->SetAlpha(m_alpha);

		m_playerName.UpdateStream();
		m_score.UpdateStream();
		m_char.UpdateStream();
		m_lastStage.UpdateStream();
		m_level.UpdateStream();

		m_tl.Update(t);
	}

	void OnInitSelect(int offset) override
	{
		m_y = -200;
		OnSelectChanged(offset);
	}

	void OnSelectChanged(int offset) override
	{
		m_y.Tween(-offset * 40 - 200, 0.25f, 1);
		
		if (offset == 0)
			m_alpha.Tween(1, 0.25f, 1);
		else if(offset <= 1 && offset >= -10)
			m_alpha.Tween(0.5f, 0.25f, 1);
		else
			m_alpha.Tween(0, 0.25f, 1);
	}

	void WannaBack()
	{
		m_y.Tween(-200, 0.25f, 1);
		m_alpha.Tween(0, 0.25f, 1);
	}

	void OnEntry(int off) override
	{
		m_alpha.Tween(0, 0.25f, 1);

		if (off == 0)
		{
			m_tl.AddTask([this] {
				m_menuManager->CallSubMenu(
					std::make_unique<MenuStack::TitleToGame>(
						std::static_pointer_cast<Basic::ActivityEngine::Activity>(std::make_shared<Activities::ReplayerActivity>(m_path))
						)
				);
			}, 0.25f);
		}
	}
};

std::vector<std::shared_ptr<Basic::Menu::MenuItem>> Activities::Title::MenuStack::TitleReplay::createRepList()
{
	std::vector<std::shared_ptr<Basic::Menu::MenuItem>> ret;

	for (auto& p : std::filesystem::recursive_directory_iterator(SaveSystem::GetReplayPath()))
	{
		if (p.is_regular_file() && p.exists())
		{
			ret.push_back(make_shared<ReplayItem>(p.path().string(),m_menuManager));
		}
	}

	return ret;
}
Activities::Title::MenuStack::TitleReplay::TitleReplay(TitleMainMenu * menuManager):
	m_menuManager {menuManager},
	m_repList{createRepList()},
	m_vm{
		[this] {
			m_repList.Down();
		},
		[this] {
			m_repList.Up();
		}
	}
{
	auto se = [] {Basic::Sound::SeManager::Play("timeout2", 0); };
	m_repList.SetOnDown(se);
	m_repList.SetOnUp(se);
	m_repList.SetClamp(true);
	m_repList.SetEnabled(true);

	m_hint.UseImage("GameUI/PauseActivity", 12);
	m_hint.SetAngle(270);
	m_hint.SetPos(-390, 300);
	m_hint.SetAlpha(0);
	m_hint.SetZoom(0.6);
	m_hint.UpdateVertex();

	m_hintAlpha = 0;
}

void Activities::Title::MenuStack::TitleReplay::OnEvent(const Basic::Input::Message & e)
{
	switch (e.type)
	{
	case decltype(e.type)::KeyDown:
		switch (e.key)
		{
		case decltype(e.key)::Esc:
		case decltype(e.key)::Bomb:
			Basic::Sound::SeManager::Play("exit", 0, 110);
			m_repList.For<ReplayItem>([](auto& i) {i.WannaBack(); });
			m_menuManager->WannaBackThisMenu();
			m_hintAlpha.Tween(0, 0.25F, 1);

			m_tl.AddTask([this] {
				m_menuManager->BackThisMenu();
			}, 0.25f);
			break;

		case decltype(e.key)::Shoot:
		case decltype(e.key)::Enter:
			m_hintAlpha.Tween(0, 0.25F, 1);
			Basic::Sound::SeManager::Play("cardget", 0, 110);
			m_repList.Enter();
			m_repList.SetEnabled(false);
			break;

		case decltype(e.key)::Slow:
			Basic::Sound::SeManager::Play("entry2", 0, 110);
			ShellExecute(
				nullptr,
				"explore",
				SaveSystem::GetReplayPath().string().c_str(),
				nullptr,
				SaveSystem::GetReplayPath().string().c_str(),
				SW_SHOW
			);
			break;
		}


		break;

	case decltype(e.type)::VecMotion:
		m_vm.OnEvent(e.vec.y);
		break;
	}
}

void Activities::Title::MenuStack::TitleReplay::OnDrawPost() const
{
	m_repList.Draw();
	m_hint.Draw();
}

void Activities::Title::MenuStack::TitleReplay::OnUpdate(float t)
{
	m_tl.Update(t);
	m_repList.Update(t);
	m_hintAlpha.Update(t);
	m_hint.SetAlpha(m_hintAlpha);
}

void Activities::Title::MenuStack::TitleReplay::OnEntryToThis()
{
	m_hintAlpha.Tween(1, 0.25f, 1);
}

void Activities::Title::MenuStack::TitleReplay::OnEntryOther()
{
	m_hintAlpha.Tween(0, 0.25f, 1);
}
