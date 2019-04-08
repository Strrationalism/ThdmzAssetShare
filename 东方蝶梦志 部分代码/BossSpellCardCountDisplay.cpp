#include "Precompiled.h"
#include "BossSpellCardCountDisplay.h"
#include "Globals.h"
#include "Game.h"

Boss::BossSpellCardCountDisplay::BossSpellCardCountDisplay()
{
	m_died = false;
	m_visible = true;
}

void Boss::BossSpellCardCountDisplay::InitStart(int spellNum)
{
	double x = WIDTH / 2 - 52;
	for (int i = 0; i < spellNum; ++i) {
		auto p = new ASpell;
		p->Init(x, i);
		x -= 16;
		Game::GetGame().GetUILayerGameObjectManager().Insert(p);
		m_spells.push(p);
	}
	m_dataActor.Add(m_alpha, 1, 40, 1);
	m_sp.UseImage("GameUI/SmallUI", 0);
	m_sp.SetPos(WIDTH / 2 - 70, HEIGHT / 2 - 32);
	m_sp.UpdateVertex();
}

void Boss::BossSpellCardCountDisplay::KillASpell()
{
	m_spells.top()->Kill();
	m_spells.pop();
	if (m_spells.empty())
		m_dataActor.Add(m_alpha, 0, 40, 1);
}

void Boss::BossSpellCardCountDisplay::Draw() const
{
	m_sp.Draw();
}

void Boss::BossSpellCardCountDisplay::Update()
{
	m_dataActor.Update();
	m_sp.SetAlpha(m_alpha);
}

void Boss::BossSpellCardCountDisplay::Kill()
{
	m_died = true;
}

void Boss::BossSpellCardCountDisplay::ASpell::Update()
{
	m_dataActor.Update();
	m_sp.SetPos(m_x, m_y);
	m_sp.SetAlpha(m_alpha);
	m_sp.UpdateVertex();
}

void Boss::BossSpellCardCountDisplay::ASpell::Draw() const
{
	m_sp.Draw();
}

void Boss::BossSpellCardCountDisplay::ASpell::Kill()
{
	m_dataActor.Add(m_alpha, 0, 20, 1);
	m_dataActor.Add(m_y, HEIGHT / 2 - 64, 20, 1);
	Game::GetGame().GetTaskList().AddTask([this]() {
		m_died = true;
	}, 20);
}

void Boss::BossSpellCardCountDisplay::ASpell::Init(double x, int i)
{
	m_died = false;
	m_drawOnTop = true;
	m_sp.UseImage("GameUI/BossSpellMark");
	m_x = x;
	m_y = HEIGHT / 2;

	Game::GetGame().GetTaskList().AddTask([this]() {
		m_dataActor.Add(m_y, HEIGHT / 2 - 32, 30, 1);
		m_dataActor.Add(m_alpha, 1, 30, 2);
	}, 10 * i);
}