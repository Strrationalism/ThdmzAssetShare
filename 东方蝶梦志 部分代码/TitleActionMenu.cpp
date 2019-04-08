#include "NuzPrecompiled.h"
#include "TitleActionMenu.h"
#include "BGMPlayer.h"
Activities::Title::MenuStack::TitleActionMenu::TitleActionMenu(std::function<void()>&& func, float timer):
	m_func{std::move(func)},
	m_timer{timer}
{
	Basic::Sound::BgmPlayer::Stop();
}

void Activities::Title::MenuStack::TitleActionMenu::OnUpdate(float t)
{
	m_timer -= t;
	if (m_timer <= 0)
	{
		m_func();
		m_func = [] {};
	}
}
