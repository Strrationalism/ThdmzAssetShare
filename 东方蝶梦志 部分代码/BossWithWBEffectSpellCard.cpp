#include "Precompiled.h"
#include "BossWithWBEffectSpellCard.h"
#include "BossWBSpellBackground.h"
#include "Game.h"

void Boss::BossWithWBEffectSpellCard::WBBGWasKilled()
{
	m_bk = nullptr;
}

void Boss::BossWithWBEffectSpellCard::SetWB(const double wb)
{
	if (m_bk == nullptr)
	{
		m_bk = new BossWBSpellBackground;
		Game::GetGame().GetMiscManagerBottom().Insert(m_bk);
	}

	static_cast<BossWBSpellBackground*>(m_bk)->SetBW(wb);
}

void Boss::BossWithWBEffectSpellCard::Update()
{
	Boss::Update();

	double x, y;
	GameObjectWithDmslControl::GetPos(x, y);
	if (m_stage5DoremiBook.has_value())
		m_stage5DoremiBook->Update(x, y + 5 * std::sin(Game::GetGame().GetGameTime() / 30.0));

	if (m_isTimeSkill)
		m_stage5DoremiBook.reset();
}

void Boss::BossWithWBEffectSpellCard::Draw() const
{
	if (m_stage5DoremiBook.has_value())
	{
		if (m_stage5DoremiBook->BossFront())
		{
			m_enemyImage.Draw();
			m_stage5DoremiBook->Draw();
		}
		else
		{
			m_stage5DoremiBook->Draw();
			m_enemyImage.Draw();
		}
	}
	else
	{
		Boss::Draw();
	}
}

void Boss::BossWithWBEffectSpellCard::SpellCardOver()
{
	Boss::SpellCardOver();
	if(m_bk)
		m_bk->Kill();
}
