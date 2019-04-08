#include "NuzPrecompiled.h"
#include "TitleJoystickNotConnectedHint.h"
#include "SeManager.h"

extern bool JoystickConnected();

void Activities::Title::MenuStack::TitleJoystickNotConnectedHint::onConnected()
{
	m_hintAlpha.Tween(0, 0.25f, 1);
	m_taskList.AddTask([this] {
		m_mainMenu->CallSubMenu(std::make_unique<TitleJoystickConfig>(m_mainMenu));
	}, 0.25f);
}

void Activities::Title::MenuStack::TitleJoystickNotConnectedHint::onDisconnected()
{
	m_taskList.AddTask([this] {
		m_hintAlpha.Tween(1, 0.25f, 1);
	}, 0.25f);
}

Activities::Title::MenuStack::TitleJoystickNotConnectedHint::TitleJoystickNotConnectedHint(TitleMainMenu * mainMenu):
	m_mainMenu{mainMenu}
{
	m_hintIcon.UseImage("GamePadConfig", 0);
	m_hint.UseImage("GamePadConfig", 1);
	m_hint.SetPos(0, 100);
	m_hint.UpdateVertex();

	m_hintIcon.SetAlpha(0);
	m_hint.SetAlpha(0);
}

void Activities::Title::MenuStack::TitleJoystickNotConnectedHint::OnWannaBackToThisShow()
{
	if(!JoystickConnected())
		m_hintAlpha.Tween(1, 0.25f, 1);
}

void Activities::Title::MenuStack::TitleJoystickNotConnectedHint::OnBackToThisShow()
{
	if (JoystickConnected())
	{
		m_mainMenu->WannaBackThisMenu();

		m_taskList.AddTask([this] {
			m_mainMenu->BackThisMenu();
		}, 0.25f);
	}
}

void Activities::Title::MenuStack::TitleJoystickNotConnectedHint::OnEntryToThis()
{
	m_joyStickConnected = JoystickConnected();

	if (m_joyStickConnected)
		onConnected();
	else
		onDisconnected();
}

void Activities::Title::MenuStack::TitleJoystickNotConnectedHint::OnDrawYSJD() const
{
	m_hintIcon.Draw();
	m_hint.Draw();
}

void Activities::Title::MenuStack::TitleJoystickNotConnectedHint::OnUpdate(float deltaTime)
{
	m_taskList.Update(deltaTime);
	m_hintAlpha.Update(deltaTime);

	if (JoystickConnected() != m_joyStickConnected)
	{
		m_joyStickConnected = JoystickConnected();
		if (m_joyStickConnected)
		{
			//²åÈë
			Basic::Sound::SeManager::Play("cardget",0,120);
			onConnected();
		}
		else
		{
			//°Î³ö
			Basic::Sound::SeManager::Play("exit",0,120);
			onDisconnected();
		}
	}

	m_timer += deltaTime;
	m_hintIcon.SetPos(0, -175 + 25 * sin(m_timer * 5));
	m_hintIcon.SetAlpha(0.75f * m_hintAlpha);
	m_hintIcon.UpdateVertex();
	m_hint.SetAlpha(0.75f * m_hintAlpha);
}

void Activities::Title::MenuStack::TitleJoystickNotConnectedHint::OnEvent(const Basic::Input::Message & e)
{
	if (e.type == Basic::Input::Message::Type::KeyDown)
	{
		Basic::Sound::SeManager::Play("exit",0,130);
		m_mainMenu->WannaBackThisMenu();

		m_hintAlpha.Tween(0, 0.25f, 1);
		m_taskList.AddTask([this] {
			m_mainMenu->BackThisMenu();
		}, 0.25f);
	}
}
