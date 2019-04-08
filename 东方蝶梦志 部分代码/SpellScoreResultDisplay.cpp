#include "Precompiled.h"
#include "SpellScoreResultDisplay.h"

Boss::SpellScoreResultDisplay::SpellScoreResultDisplay(unsigned score):
	m_num{"GameUI/Number1"}
{
	m_died = false;

	m_num.SetAlpha(1);
	m_num.SetDisplayFormat(7);
	m_num.SetPos(-95, -140);
	m_num.SetNumber(score);


	m_tl.AddTask([this] {
		m_dataActor.Add(m_alpha, 1, 40, 1);
	}, 40);

	m_tl.AddTask([this]() {
		m_dataActor.Add(m_alpha, 0, 40, 0);
		m_tl.AddTask([this] {
			Kill();
		}, 40);
	}, 180);
}

void Boss::SpellScoreResultDisplay::Kill()
{
	m_died = true;
}

void Boss::SpellScoreResultDisplay::Update()
{
	m_tl.Update();
	m_dataActor.Update();
	m_num.SetAlpha(m_alpha);
}

void Boss::SpellScoreResultDisplay::Draw() const
{
	m_num.Draw();
}
