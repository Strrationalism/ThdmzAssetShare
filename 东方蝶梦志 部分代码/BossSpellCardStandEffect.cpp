#include "Precompiled.h"
#include "BossSpellCardStandEffect.h"

Boss::BossSpellCardStandEffect::BossSpellCardStandEffect(std::shared_ptr<Basic::Video::Renderer2D::Sprite2D> & s)
{
	m_lh = s;
	m_died = false;

	m_dataActor.Add(m_x, 100, 20, 2);
	m_dataActor.Add(m_y, -25, 20, 2);
	m_dataActor.Add(m_alpha, 1, 20, 2);

	m_tasks.AddTask([this]() {
		m_dataActor.Add(m_x, 0, 100, 2);
		m_dataActor.Add(m_y, 0, 100, 2);
	}, 20);

	m_tasks.AddTask([this]() {
		m_dataActor.Add(m_x, -100, 20, 1);
		m_dataActor.Add(m_y, 25, 20, 1);
		m_dataActor.Add(m_alpha, 0, 20, 1);
	}, 120);

	m_tasks.AddTask([this]() {
		m_died = true;
	}, 280);
}

void Boss::BossSpellCardStandEffect::Draw() const
{
	m_lh->Draw();
}

void Boss::BossSpellCardStandEffect::Update()
{
	m_tasks.Update();
	m_dataActor.Update();
	m_lh->SetAlpha(m_alpha);
	m_lh->SetPos(m_x, m_y);
	m_lh->UpdateVertex();
}

void Boss::BossSpellCardStandEffect::Kill()
{
	m_died = true;
}