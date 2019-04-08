#include "NuzPrecompiled.h"
#include "TitleJoystickGuide.h"
#include "SeManager.h"

Activities::Title::MenuStack::TitleJoystickGuide::TitleJoystickGuide(TitleMainMenu* mainMenu) :
	m_setter([this](int a) {OnJoystickEvent(a); }),
	m_mainMenu{mainMenu}
{
	m_label.UseImage("GamePadConfigGuider", 0);
	m_label.SetPos(-300, -250);
	m_label.SetAlpha(0.9f);
	m_label.UpdateVertex();

	for (int i = 0; i < 4; ++i)
	{
		m_guide[i].UseImage("GamePadConfigGuider", 1 + i);
		m_guide[i].SetAlpha(0.55f);
		m_guide[i].SetPos(-250, -150 + i * 100);
		m_guide[i].UpdateVertex();
	}

	m_guide[0].SetAlpha(0.95f);

	m_finishedLabel.UseImage("GamePadConfigGuider", 5);
	m_finishedLabel.SetAlpha(0);
	m_finishedLabel.SetPos(50, 0);
	m_finishedLabel.UpdateVertex();
}

void Activities::Title::MenuStack::TitleJoystickGuide::OnEntryToThis()
{
	Basic::Sound::SeManager::Play("timeout2", 0);
}

void Activities::Title::MenuStack::TitleJoystickGuide::OnDrawYSJD() const
{
	m_label.Draw();
	
	for (const auto& sp : m_guide)
		sp.Draw();

	m_finishedLabel.Draw();
}

void Activities::Title::MenuStack::TitleJoystickGuide::OnUpdate(float t)
{
	m_finLabAlpha.Update(t);
	m_tl.Update(t);
	m_finishedLabel.SetAlpha(m_finLabAlpha);
}

void Activities::Title::MenuStack::TitleJoystickGuide::OnJoystickEvent(int button)
{
	if (m_ignore)
	{
		m_ignore = false;
		return;
	}

	if (m_finished)
		return;

	if (m_buttons.size() <= 2)
	{
		m_buttons.push_back(button);

		for (int i = 0; i < 4; ++i)
		{
			m_guide[i].UseImage("GamePadConfigGuider", 1 + i);
			m_guide[i].SetAlpha(0.55f);
			m_guide[i].SetPos(-250, -150 + i * 100);
			m_guide[i].UpdateVertex();
		}
		m_guide[m_buttons.size()].SetAlpha(0.95f);

		Basic::Sound::SeManager::Play("timeout", 0);
	}
	else
	{
		Basic::Sound::SeManager::Play("timeout", 0);
		m_label.SetAlpha(0);
		for (auto& sp : m_guide)
			sp.SetAlpha(0);
		
		m_finished = true;

		m_finLabAlpha.Tween(0.85f, 0.25f, 1);

		extern void __JoystickClearKeyConfig();
		extern void __JoystickSetKey(int button, Basic::Input::Key key);

		__JoystickClearKeyConfig();
		__JoystickSetKey(m_buttons.at(0), Basic::Input::Key::Shoot);
		__JoystickSetKey(m_buttons.at(1), Basic::Input::Key::Bomb);
		__JoystickSetKey(m_buttons.at(2), Basic::Input::Key::Slow);
		__JoystickSetKey(button, Basic::Input::Key::Esc);
		Basic::Input::WriteJoystickButtonSetting();

		m_tl.AddTask([this] {
			m_finLabAlpha.Tween(0.0f, 0.25f, 1);
		}, 2.5f);

		m_tl.AddTask([this] {
			m_mainMenu->BackThisMenu();
		},3.0f);
	}
}
