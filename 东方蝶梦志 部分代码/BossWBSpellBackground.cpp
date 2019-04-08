#include "Precompiled.h"
#include "BossWBSpellBackground.h"
#include "Globals.h"
#include "BossWithWBEffectSpellCard.h"
#include "Game.h"

Boss::BossWBSpellBackground::BossWBSpellBackground()
{
	m_died = false;
}

void Boss::BossWBSpellBackground::Draw() const
{
	glDisable(GL_TEXTURE_2D);

	glColor4d(1-m_bw, 1-m_bw, 1-m_bw, 1);
	glBegin(GL_QUADS);
	{
		glVertex2d(-WIDTH / 2, -HEIGHT / 2);
		glVertex2d(WIDTH / 2, -HEIGHT / 2);
		glVertex2d(WIDTH / 2, HEIGHT / 2);
		glVertex2d(-WIDTH / 2, HEIGHT / 2);
	}
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void Boss::BossWBSpellBackground::Update()
{
}

void Boss::BossWBSpellBackground::SetBW(const double bw)
{
	m_bw = bw;
}

void Boss::BossWBSpellBackground::Kill()
{
	static_cast<BossWithWBEffectSpellCard*>(Boss::Boss::GetBoss())->WBBGWasKilled();

	m_died = true;
}

