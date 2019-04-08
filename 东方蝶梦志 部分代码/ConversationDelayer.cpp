#include "Precompiled.h"
#include "ConversationDelayer.h"

const static int CLICK_PL = 11;

bool Conversation::ConversationDelayer::NowClickable() const
{
	const int dog = m_clickWatchDog.value_or(CLICK_PL);
	return dog <= CLICK_PL;
}

void Conversation::ConversationDelayer::Click()
{
	if (m_clickWatchDog.has_value())
	{
		if (m_clickWatchDog.value() < CLICK_PL)
			m_clickWatchDog.value() = CLICK_PL;
	}
	else
	{
		m_clickWatchDog = 70;
	}
}

void Conversation::ConversationDelayer::Update()
{
	if (m_clickWatchDog.has_value())
	{
		//Log("Dog:%d",m_clickWatchDog.value());
		const auto& dog = m_clickWatchDog.value()--;
		if (dog == 0)
			m_clickWatchDog.reset();
	}
}
