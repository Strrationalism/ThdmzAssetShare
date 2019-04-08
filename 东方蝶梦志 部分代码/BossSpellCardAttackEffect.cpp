#include "Precompiled.h"
#include "BossSpellCardAttackEffect.h"
#include "ImageCacheManager.h"
#include "glew.h"
#include "Sprite2D.h"
#include "Log.h"

void Boss::BossSpellCardAttackEffect::drawTwoLines() const
{
	glBegin(GL_QUADS);
	{
		glTexCoord2d(m_cnt, 0); glVertex2d(-WIDTH / 2, -32);
		glTexCoord2d(6 + m_cnt, 0); glVertex2d(WIDTH / 2, -32);
		glTexCoord2d(6 + m_cnt, 1); glVertex2d(WIDTH / 2, 0);
		glTexCoord2d(m_cnt, 1); glVertex2d(-WIDTH / 2, 0);

		glTexCoord2d(-m_cnt, 0); glVertex2d(-WIDTH / 2, 0);
		glTexCoord2d(6 - m_cnt, 0); glVertex2d(WIDTH / 2, 0);
		glTexCoord2d(6 - m_cnt, 1); glVertex2d(WIDTH / 2, 32);
		glTexCoord2d(-m_cnt, 1); glVertex2d(-WIDTH / 2, 32);
	}
	glEnd();
}

Boss::BossSpellCardAttackEffect::BossSpellCardAttackEffect()
{
	m_died = false;
	m_hint = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("GameUI/BossSpellCardAttack");

	m_dataActor.Add(m_alpha, 0.5f, 20, 1);

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_alpha, 0, 20, 1);
	}, 120);

	m_taskList.AddTask([this]() {
		m_died = true;
	}, 140);
}

Boss::BossSpellCardAttackEffect::~BossSpellCardAttackEffect()
{
}

void Boss::BossSpellCardAttackEffect::Draw() const
{
	glColor4d(1, 1, 1, m_alpha);
	glBindTexture(GL_TEXTURE_2D, *m_hint);

	glRotated(-30, 0, 0, 1);
	glScaled(1.13f, 1, 1);

	glTranslated(0, -2 * 64, 0);

	for (int i = 0; i < 6; ++i) {
		drawTwoLines();
		glTranslated(0, 64, 0);
	}
	glLoadIdentity();
}

void Boss::BossSpellCardAttackEffect::Update()
{
	m_cnt += 0.02f;
	m_dataActor.Update();
	m_taskList.Update();
}

void Boss::BossSpellCardAttackEffect::Kill()
{
	m_died = true;
}