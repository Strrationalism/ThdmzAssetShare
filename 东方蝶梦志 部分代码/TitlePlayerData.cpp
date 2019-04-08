#include "NuzPrecompiled.h"
#include "TitlePlayerData.h"
#include "SeManager.h"
#include "CharReimu.h"
#include "CharMarisa.h"
#include "HRollingFadingMenuItem.h"
using ChMenuItem = Activities::Title::MenuStack::TitleCharaterSelectMenu::CharaterMenuItem;
using namespace Activities::Title::MenuStack;
using namespace std;

void Activities::Title::MenuStack::TitlePlayerData::fadeIn()
{
	for (auto& p : m_charSelect)
		dynamic_cast<ChMenuItem&>(*p).FadeIn();

	for (auto& p : m_lvSelect)
		dynamic_cast<Menus::HRollingFadingMenuItem&>(*p).GetAlpha().Tween(1, 0.25f, 1);
}

void Activities::Title::MenuStack::TitlePlayerData::fadeOut()
{
	for (auto& p : m_charSelect)
		dynamic_cast<ChMenuItem&>(*p).FadeOut();

	for (auto& p : m_lvSelect)
		dynamic_cast<Menus::HRollingFadingMenuItem&>(*p).GetAlpha().Tween(0, 0.25f, 1);
}

std::tuple<int,StageLevel,bool> Activities::Title::MenuStack::TitlePlayerData::getSelected() const
{
	return std::make_tuple(
		m_charSelect.GetSelected(),
		(StageLevel)(m_lvSelect.GetSelected()),
		m_lvSelect.GetSelected() == 4
	);
}

std::shared_ptr<Basic::Menu::MenuItem> Activities::Title::MenuStack::TitlePlayerData::makeLevelMenuItem(int imgID, const std::function<void()>& func)
{
	const int widths[] = { 199,251,197,269,181 };

	return std::make_shared<Menus::HRollingFadingMenuItem>(
		"TitleCharaterSelection",
		imgID,
		-200 - widths[imgID - 4] / 2,
		0,
		250,
		func
		);
}

std::vector<std::shared_ptr<Basic::Menu::MenuItem>> TitlePlayerData::makePages(const std::tuple<int, StageLevel, bool>& param)
{
	std::queue<std::unique_ptr<TitlePlayerDataSpellCardItem>> pageSrc;
	
	auto scs = SpellCardDatabase::Get().GetSpellTitleHash(
		(SpellCardDatabase::Charater)std::get<0>(param),
		get<1>(param),
		get<2>(param));

	for (auto& sc : scs)
	{
		pageSrc.push(std::make_unique<TitlePlayerDataSpellCardItem>(get<0>(sc), get<1>(sc)));
	}

	return SpellPage::MakePages(pageSrc,10);
}

Activities::Title::MenuStack::TitlePlayerData::TitlePlayerData(TitleMainMenu * mainMenu) :
	m_mainMenu {mainMenu},
	m_charSelect{
		{
			std::make_shared<ChMenuItem>(0,5,255,121,121,Char::Reimu::CharReimu::Get(),true),
			std::make_shared<ChMenuItem>(2,6,154,242,255,Char::Marisa::CharMarisa::Get(),true)
		},0
	},
	m_lvSelect{
		{
			makeLevelMenuItem(4,[] {}),
			makeLevelMenuItem(5,[] {}),
			makeLevelMenuItem(6,[] {}),
			makeLevelMenuItem(7,[] {}),
			makeLevelMenuItem(8,[] {})
		}
	},
	m_lvMat{
		[this] {m_lvSelect.Down(); },
		[this] {m_lvSelect.Up(); }
	},
	m_spellList{
		makePages(getSelected())
	},
	m_scMat{
		[this] {m_spellList->Down(); },
		[this] {m_spellList->Up(); }
	}
{
	auto onMove = [] 
	{
		Basic::Sound::SeManager::Play("timeout2", 0);
	};

	auto updateSpells = [this, onMove]
	{
		onMove();

		menuVisible(false);
		m_tl.AddTask([this, onMove] {
			m_spellList.emplace(makePages(getSelected()));
			m_spellList->SetOnDown(onMove);
			m_spellList->SetOnUp(onMove);
			m_spellList->SetEnabled(true);
			menuVisible(true);
		},0.25F);
	};

	m_charSelect.SetOnDown(updateSpells);
	m_charSelect.SetOnUp(updateSpells);
	m_charSelect.SetEnabled(true);

	m_lvSelect.SetOnDown(updateSpells);
	m_lvSelect.SetOnUp(updateSpells);
	m_lvSelect.SetEnabled(true);
	m_lvSelect.SetClamp(true);

	m_spellList->SetOnDown(onMove);
	m_spellList->SetOnUp(onMove);
	m_spellList->SetEnabled(true);

	m_hint.UseImage("GameUI/ZChangePlayer");
	m_hint.SetPos(-500, 300);
	m_hint.SetZoom(0.5);
	m_hint.SetAlpha(0);
	m_hint.UpdateVertex();
}

void Activities::Title::MenuStack::TitlePlayerData::OnEntryToThis()
{
	fadeIn();
	m_alpha = 0;
	m_alpha.Tween(1, 0.25f, 1);
}

void Activities::Title::MenuStack::TitlePlayerData::OnUpdate(float deltaTime)
{
	m_alpha.Update(deltaTime);
	m_tl.Update(deltaTime);
	m_charSelect.Update(deltaTime);
	m_lvSelect.Update(deltaTime);

	m_spellList->Update(deltaTime);
	m_hint.SetAlpha(m_alpha);
}

void Activities::Title::MenuStack::TitlePlayerData::OnDrawYSJD() const
{
	m_charSelect.Draw();
	m_lvSelect.Draw();

	
}

void Activities::Title::MenuStack::TitlePlayerData::OnDrawPost() const
{
	m_spellList->Draw();
	m_hint.Draw();
}

void Activities::Title::MenuStack::TitlePlayerData::OnEvent(const Basic::Input::Message & e)
{
	if (e.type == decltype(e.type)::KeyDown)
	{
		if (
			e.key == decltype(e.key)::Esc ||
			e.key == decltype(e.key)::Bomb)
		{
			menuVisible(false);

			m_alpha.Tween(0, 0.25f, 1);

			Basic::Sound::SeManager::Play("exit", 0,120);
			m_mainMenu->WannaBackThisMenu();
			fadeOut();
			m_tl.AddTask([this] {
				m_mainMenu->BackThisMenu();
			}, 0.25f);
		}

		else if (
			e.key == decltype(e.key)::Shoot ||
			e.key == decltype(e.key)::Enter)
			m_charSelect.Down();
	}

	else if (e.type == decltype(e.type)::VecMotion)
	{
		if (e.vec.x == 0)
			m_scMat.OnEvent(0);

		if (e.vec.y == 0)
			m_lvMat.OnEvent(0);

		if (std::abs(e.vec.x) > std::abs(e.vec.y))
			m_scMat.OnEvent(e.vec.x);
		else
			m_lvMat.OnEvent(e.vec.y);
	}
}

void Activities::Title::MenuStack::TitlePlayerData::menuVisible(bool b)
{
	for (auto& p : *m_spellList)
	{
		for (auto& q : dynamic_cast<SpellPage&>(*p))
		{
			q->Visible(b);
		}
	}
	if(b)
		m_spellList->RefreshSelectIndex();
}
