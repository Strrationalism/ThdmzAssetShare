#include "Precompiled.h"
#include "DreamWaterWave.h"
#include "Shaders.h"

AfterEffect::DreamWaterWave::DreamWaterWave(double x, double y)
{
	m_dataActor.Add(m_cnt, M_PI, 100, 1);
	m_cntX = x;
	m_cntY = y;
}

void AfterEffect::DreamWaterWave::Draw() const
{
	Basic::Shaders::WaterWave(-m_cnt * 8, 8 * sin(m_cnt), 0.03 * sin(m_cnt), m_cntX, m_cntY);
	glColor4d(m_r, m_g, m_b, m_a);
	glBlendFunc(m_blendModeSrc, m_blendMode);
	glBegin(GL_QUADS); {
		glTexCoord2d(0, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(0, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);
	}
	glEnd();
	glUseProgram(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void AfterEffect::DreamWaterWave::Update()
{
	m_dataActor.Update();
}

bool AfterEffect::DreamWaterWave::Killed()
{
	return m_cnt >= M_PI;
}