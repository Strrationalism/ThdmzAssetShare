#include "NuzPrecompiled.h"
#include "SpellGotTimesDisplayer.h"
#include "PlayerProfile.h"

Activities::Title::MenuStack::SpellGotTimesDisplayer::SpellGotTimesDisplayer(int spellHash):
	m_spellHash { spellHash },
	m_num1 { "GameUI/Number2" },
	m_num2 { "GameUI/Number2" }
{
	m_num1.SetDisplayFormat(2);
	m_num2.SetDisplayFormat(2);

	auto [gotTimes,spellTimes] = SaveSystem::PlayerProfile::Get().GetSpellGotNumber(spellHash);
	m_num1.SetNumber(std::clamp(gotTimes, 0u, 99u));
	m_num2.SetNumber(std::clamp(spellTimes, 0u, 99u));

	m_num1.SetAlpha(0);
	m_num2.SetAlpha(0);

	m_num1.SetZoom(1.5);
	m_num2.SetZoom(1.5);

}

void Activities::Title::MenuStack::SpellGotTimesDisplayer::OnDraw() const
{
	m_num1.Draw();
	m_num2.Draw();
}

void Activities::Title::MenuStack::SpellGotTimesDisplayer::SetPos(float x, float y)
{
	m_num1.LeftTopPos(true);
	m_num2.LeftTopPos(true);

	m_num1.SetPos(x + 20, y - 20);
	m_num2.SetPos(x + 80, y - 20);
}

void Activities::Title::MenuStack::SpellGotTimesDisplayer::SetAlpha(float alpha)
{
	m_num1.SetAlpha(alpha);
	m_num2.SetAlpha(alpha);
}
