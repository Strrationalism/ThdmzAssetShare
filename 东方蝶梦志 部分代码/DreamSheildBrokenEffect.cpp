#include "Precompiled.h"
#include "DreamSheildBrokenEffect.h"
#include "Shaders.h"

AfterEffect::DreamSheildBrokenEffect::DreamSheildBrokenEffect(float x, float y, Mode mode) :
	m_x(x),
	m_y(y),
	m_mode(mode)
{
	m_dataActor.Add(m_time, 1, 50, 5);
}

void AfterEffect::DreamSheildBrokenEffect::Draw() const
{
	glDisable(GL_BLEND);
	Basic::Shaders::DreamSheildBroken(m_x, m_y, m_time,int(m_mode));
	glBegin(GL_QUADS); {
		glTexCoord2d(0, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(0, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);
	}
	glEnd();

	glEnable(GL_BLEND);
	glUseProgram(0);
}

void AfterEffect::DreamSheildBrokenEffect::Update()
{
	m_dataActor.Update();
}

bool AfterEffect::DreamSheildBrokenEffect::Killed()
{
	return m_time >= 1.0f;
}
