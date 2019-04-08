#include "Precompiled.h"
#include "FadingWater.h"
#include "Game.h"
#include "Shaders.h"
AfterEffect::FadingWater::FadingWater(int time)
{
	m_dataActor.Add(m_texFactor, 32, time, 1);
	m_dataActor.Add(m_ampFactor, 0.05, time, 1);
	Game::GetGame().GetTaskList().AddTask([this]() {
		m_killed = true;
	}, time);
}
void AfterEffect::FadingWater::Draw() const
{
	Basic::Shaders::WaterWave(m_cnt, m_texFactor, m_ampFactor, 0, 0);
	glBegin(GL_QUADS); {
		glTexCoord2d(0, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(0, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);
	}
	glEnd();
	glUseProgram(0);
}

void AfterEffect::FadingWater::Update()
{
	m_cnt -= 0.25;
	m_dataActor.Update();
}

bool AfterEffect::FadingWater::Killed()
{
	return m_killed;
}