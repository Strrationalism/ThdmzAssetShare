#include "Precompiled.h"
#include "BossSpellBonusShow.h"
#include "SeManager.h"
Boss::BossSpellBonusShow::BossSpellBonusShow(bool getBonus)
{
	m_died = false;
	m_drawOnTop = true;
	m_sp.UseImage("GameUI/SpellBonus", getBonus ? 0 : 1);
	if (getBonus)
		Basic::Sound::SeManager::Play("cardget", 0,120);
	m_y = -320;
	m_dataActor.Add(m_alpha, 1, 40, 1);
	m_dataActor.Add(m_y, -200, 40, 1);
	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_alpha, 0, 40, 0);
	}, 180);
	m_taskList.AddTask([this]() {
		m_died = true;
	}, 220);
}

void Boss::BossSpellBonusShow::Draw() const
{
	m_sp.Draw();
}

void Boss::BossSpellBonusShow::Update()
{
	m_taskList.Update();
	m_dataActor.Update();
	m_sp.SetPos(0, m_y);
	m_sp.SetAlpha(m_alpha);
	m_sp.UpdateVertex();
}

void Boss::BossSpellBonusShow::Kill()
{
	m_died = true;
}