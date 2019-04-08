#include "NuzPrecompiled.h"
#include "TitleCharaterSelectMenu.h"
#include "MenuItem.h"
#include "Shaders.h"
#include "SeManager.h"
#include "BaseBackground.h"
#include "BasicChar.h"
#include "CharReimu.h"
#include "CharMarisa.h"
#include "HRollingFadingMenuItem.h"




std::shared_ptr<Basic::Menu::MenuItem> Activities::Title::MenuStack::TitleCharaterSelectMenu::makeLevelMenuItem(int imgID)
{
	const int widths[] = { 199,251,197,269,181 };

	return std::make_shared<Menus::HRollingFadingMenuItem>(
		"TitleCharaterSelection",
		imgID,
		200 - widths[imgID - 4] / 2,
		0,
		250,
		[] {}
		);
}

std::vector<std::shared_ptr<Basic::Menu::MenuItem>> Activities::Title::MenuStack::TitleCharaterSelectMenu::createLevelMenu(bool extra)
{
	std::vector<std::shared_ptr<Basic::Menu::MenuItem>> v;
	for (int i = 4; i < 8; ++i)
		v.push_back(makeLevelMenuItem(i));

	if (extra)
		v.push_back(makeLevelMenuItem(8));

	return v;
}

Activities::Title::MenuStack::TitleCharaterSelectMenu::TitleCharaterSelectMenu(Title::TitleMainMenu * m,bool showExtra) :
	m_menuManager{ m },
	m_menuCharater{
		{
			std::make_shared<CharaterMenuItem>(0,5,255,121,121,Char::Reimu::CharReimu::Get()),
			std::make_shared<CharaterMenuItem>(2,6,154,242,255,Char::Marisa::CharMarisa::Get())
		},
		0	
	},
	m_menuLevel {
		createLevelMenu(showExtra),
		1
	},
	m_chMat{ [this] {m_menuCharater.Down(); },[this] {m_menuCharater.Up(); } },
	m_lvMat{ [this] {m_menuLevel.Down(); },[this] {m_menuLevel.Up(); } }
{
	const auto onSelectMoved = []()
	{
		Basic::Sound::SeManager::Play("timeout2",0);
	};

	m_menuCharater.SetEnabled(true);
	m_menuCharater.SetOnDown(onSelectMoved);
	m_menuCharater.SetOnUp(onSelectMoved);
	m_menuCharater.SetClamp(false);

	m_menuLevel.SetEnabled(true);
	m_menuLevel.SetOnDown(onSelectMoved);
	m_menuLevel.SetOnUp(onSelectMoved);
	m_menuLevel.SetClamp(true);
}

void Activities::Title::MenuStack::TitleCharaterSelectMenu::OnEvent(const Basic::Input::Message & e)
{
	if (e.type == Basic::Input::Message::Type::KeyDown)
	{
		if (e.key == Basic::Input::Key::Bomb || e.key == Basic::Input::Key::Esc)
		{
			Basic::Sound::SeManager::Play("exit", 0,120);
			m_menuManager->WannaBackThisMenu();

			m_menuCharater.SetEnabled(false);
			m_menuLevel.SetEnabled(false);

			for (auto& p : m_menuCharater)
				dynamic_cast<CharaterMenuItem&>(*p).FadeOut();

			for (auto& p : m_menuLevel)
				dynamic_cast<Menus::HRollingFadingMenuItem&>(*p).GetAlpha().Tween(0, 0.25f, 1);
			

			m_taskList.AddTask([this] {
				m_menuManager->BackThisMenu();
			}, 0.25f);
		}

		else if (e.key == Basic::Input::Key::Shoot || e.key == Basic::Input::Key::Enter)
		{
			Basic::Sound::SeManager::Play("entry2", 0,120);
			m_menuCharater.Enter();
			m_menuLevel.Enter();

			auto& ch = m_menuCharater.GetSelected() == 0 ? 
				(Char::BasicChar&)Char::Reimu::CharReimu::Get() : 
				(Char::BasicChar&)Char::Marisa::CharMarisa::Get();

			m_callback(
				m_menuLevel.GetSelected() == 4,
				m_menuLevel.GetSelected() == 4 ? StageLevel::L : (StageLevel)m_menuLevel.GetSelected(),
				ch);
		}
	}
	else if (e.type == Basic::Input::Message::Type::VecMotion)
	{
		if (e.vec.x == 0)
			m_chMat.OnEvent(0);

		if (e.vec.y == 0)
			m_lvMat.OnEvent(0);

		if(std::abs(e.vec.x) > std::abs(e.vec.y))
			m_chMat.OnEvent(e.vec.x);
		else
			m_lvMat.OnEvent(e.vec.y);
	}
}

void Activities::Title::MenuStack::TitleCharaterSelectMenu::OnUpdate(float t)
{
	m_menuCharater.Update(t);
	m_menuLevel.Update(t);
	m_taskList.Update(t);
}

void Activities::Title::MenuStack::TitleCharaterSelectMenu::OnDrawYSJD() const
{
	m_menuCharater.Draw();
	m_menuLevel.Draw();
}

void Activities::Title::MenuStack::TitleCharaterSelectMenu::OnEntryOther()
{
	for (auto& p : m_menuCharater)
		dynamic_cast<CharaterMenuItem&>(*p).FadeOut();

	for (auto& p : m_menuLevel)
		dynamic_cast<Menus::HRollingFadingMenuItem&>(*p).GetAlpha().Tween(0, 0.25f, 1);
}

void Activities::Title::MenuStack::TitleCharaterSelectMenu::OnWannaBackToThisShow()
{
	for (auto& p : m_menuCharater)
		dynamic_cast<CharaterMenuItem&>(*p).FadeIn();

	for (auto& p : m_menuLevel)
		dynamic_cast<Menus::HRollingFadingMenuItem&>(*p).GetAlpha().Tween(1, 0.25f, 1);
}

Activities::Title::MenuStack::TitleCharaterSelectMenu::CharaterMenuItem::CharaterMenuItem(int charImageBegin, int charNameImage, float cnr, float cng, float cnb, Char::BasicChar & charaterObject,bool playerDataMode):
	m_charaterObject {
		charaterObject
	},
	m_playerDataMode{playerDataMode}
{
	const auto left = m_playerDataMode ? -400 : -200;
	m_char.UseImage("TitleCharaterSelection", charImageBegin);
	m_char.SetPos(left, 0);
	m_char.UpdateVertex();

	m_charLight.UseImage("TitleCharaterSelection", charImageBegin + 1);
	m_charLight.SetPos(left, 0);
	m_charLight.UpdateVertex();

	if (!m_playerDataMode)
	{
		m_charName.UseImage("GameUI/NameCards", charNameImage);
		m_charName.SetPos(-500, -200);
		m_charName.SetColorFliter(cnr / 255, cng / 255, cnb / 255);
		m_charName.UpdateVertex();

		for (int i = 0; i < 2; ++i)
		{
			m_jt[i].UseImage("TitleCharaterSelection", 9 + charImageBegin / 2);
			m_jt[i].SetZoom(0.5f);
		}
	}

	m_alpha = 0;
}

void Activities::Title::MenuStack::TitleCharaterSelectMenu::CharaterMenuItem::Draw() const
{
	if (m_alpha > 0)
	{
		if (Background::BaseBackground::UsingBeautifulBG())
		{
			const auto blur = 0.000001f + 20 * (1 - m_alpha);
			Basic::Shaders::GaussBlurW(blur);
		}
		if (!m_playerDataMode)
			m_charName.OldDraw();
		m_charLight.OldDraw();
		m_char.OldDraw();

		glUseProgram(0);

		if(!m_playerDataMode)
			for (auto& jt : m_jt)
				jt.OldDraw();
	}
}

void Activities::Title::MenuStack::TitleCharaterSelectMenu::CharaterMenuItem::Update(float t)
{
	m_alpha.Update(t);

	m_timer += t;

	const auto spAlpha = std::clamp(m_alpha * 1.0f, 0.0f, 0.75f);

	
	m_char.SetAlpha(spAlpha);
	m_charLight.SetAlpha(spAlpha);

	if (!m_playerDataMode)
	{
		m_charName.SetAlpha(spAlpha*1.15f);
		for (auto& jt : m_jt)
			jt.SetAlpha(spAlpha * 1.5f);

		const float fd = sin(m_timer * 10) * 5;
		m_jt[0].SetPos(-420 - fd, 0);
		m_jt[1].SetPos(10 + fd, 0);


		m_jt[0].SetFlip(false, true);
		for (auto& jt : m_jt)
			jt.UpdateVertex();
	}
}

void Activities::Title::MenuStack::TitleCharaterSelectMenu::CharaterMenuItem::OnInitSelect(int offset)
{
	OnSelectChanged(offset);
}

void Activities::Title::MenuStack::TitleCharaterSelectMenu::CharaterMenuItem::OnSelectChanged(int offset)
{
	m_selected = offset == 0;

	m_alpha.Tween(m_selected ? 1.0f : 0.0f, 0.5f, 1);
}

void Activities::Title::MenuStack::TitleCharaterSelectMenu::CharaterMenuItem::OnEntry(int offset)
{
	m_selected = offset == 0;
}

bool Activities::Title::MenuStack::TitleCharaterSelectMenu::CharaterMenuItem::IsTopDraw() const
{
	return m_selected;
}

void Activities::Title::MenuStack::TitleCharaterSelectMenu::CharaterMenuItem::FadeOut()
{
	m_alpha.Tween(0, 0.25f, 1);
}

void Activities::Title::MenuStack::TitleCharaterSelectMenu::CharaterMenuItem::FadeIn()
{
	if (m_selected)
		m_alpha.Tween(1, 0.25f, 1);
}
