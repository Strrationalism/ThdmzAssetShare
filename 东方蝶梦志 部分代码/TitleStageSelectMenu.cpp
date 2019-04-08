#include "NuzPrecompiled.h"
#include "TitleStageSelectMenu.h"
#include "SeManager.h"
#include "HRollingFadingMenuItem.h"
#include "Shaders.h"
#include "PlayerProfile.h"
using namespace std;
using namespace Menus;

std::shared_ptr<Basic::Menu::MenuItem> Activities::Title::MenuStack::TitleStageSelectMenu::makeMenuItem(int stageID)
{
	auto ret = make_shared<HRollingFadingMenuItem>("StageTitle", stageID, 0, 0, 250, 
		[this,stageID] { m_callback(stageID); }
	);
	ret->GetX() = 0;
	ret->GetAlpha() = 0.75f;
	return ret;
}

std::vector<std::shared_ptr<Basic::Menu::MenuItem>> Activities::Title::MenuStack::TitleStageSelectMenu::makeMenu(bool isExtra)
{
	std::vector<std::shared_ptr<Basic::Menu::MenuItem>> ret;

	if (isExtra)
		ret.emplace_back(makeMenuItem(6));
	else
		for (int i = 0; i < 6; ++i)
		{
			if(SaveSystem::PlayerProfile::Get().GetPracticeEnabled(i))
				ret.emplace_back(makeMenuItem(i));
		}
	return ret;
}

Activities::Title::MenuStack::TitleStageSelectMenu::TitleStageSelectMenu(Title::TitleMainMenu * m,bool isExtra):
	m_menuManager{m},
	m_menu { makeMenu(isExtra) },
	m_mat {
		[this] { m_menu.Down(); },
		[this] {m_menu.Up(); }
	}
{
	m_menu.SetEnabled(true);
	m_menu.SetClamp(true);

	auto onSelectMove = [] { Basic::Sound::SeManager::Play("timeout2", 0);  };
	m_menu.SetOnUp(onSelectMove);
	m_menu.SetOnDown(onSelectMove);
}

void Activities::Title::MenuStack::TitleStageSelectMenu::OnEvent(const Basic::Input::Message & e)
{
	if (e.type == Basic::Input::Message::Type::KeyDown)
	{
		if (e.key == Basic::Input::Key::Bomb || e.key == Basic::Input::Key::Esc)
		{
			Basic::Sound::SeManager::Play("exit", 0,120);
			m_menuManager->WannaBackThisMenu();
			m_menuManager->BackThisMenu();
		}

		else if (e.type == Basic::Input::Message::Type::KeyDown)
		{
			if (e.key == Basic::Input::Key::Shoot || e.key == Basic::Input::Key::Enter)
			{
				Basic::Sound::SeManager::Play("cardget",0,120);
				m_menu.Enter();
			}
		}
	}

	else if (e.type == Basic::Input::Message::Type::VecMotion)
	{
		m_mat.OnEvent(e.vec.y);
	}
}

void Activities::Title::MenuStack::TitleStageSelectMenu::OnEntryOther()
{
	for (auto& p : m_menu)
		static_cast<HRollingFadingMenuItem&>(*p).GetAlpha().Tween(0, 0.25f, 1);
}


void Activities::Title::MenuStack::TitleStageSelectMenu::OnUpdate(float t)
{
	m_menu.Update(t);
}

void Activities::Title::MenuStack::TitleStageSelectMenu::OnDrawYSJD() const
{
	Basic::Shaders::GaussBlurW(34);
	m_menu.Draw();
	glUseProgram(0);

	m_menu.Draw();
}
