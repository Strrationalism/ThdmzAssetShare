#include "NuzPrecompiled.h"
#include "VecMatcher.h"

Basic::Input::VecMatcher::VecMatcher(std::function<void()>&& nega, std::function<void()>&& posi):
	m_nega{std::move(nega)},
	m_posi{std::move(posi)}
{
}

void Basic::Input::VecMatcher::OnEvent(int vec)
{
	State now = State::Zero;
	if (vec > 0) now = State::Posi;
	else if (vec < 0) now = State::Nega;

	if (now != m_lastState)
	{
		m_lastState = now;
		if (now == State::Nega)
			m_nega();
		else if (now == State::Posi)
			m_posi();
	}
}
