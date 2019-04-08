#include "Precompiled.h"
#include "ScreenVibrator.h"
#include "Game.h"

AfterEffect::ScreenVibrator::ScreenVibrator(int leftTime, double leftX, double leftY, int leftSlowMotion, int keepTime, int rightTime)
{
	m_xSinArg = Rand() * M_PI;
	m_ySinArg = Rand() * M_PI;

	auto& tl = Game::GetGame().GetTaskList();
	auto& da = Game::GetGame().GetDataActor();

	m_xVibWidth = m_yVibHeight = 0;
	da.Add(m_xVibWidth, leftX, leftTime, leftSlowMotion);
	da.Add(m_yVibHeight, leftY, leftTime, leftSlowMotion);

	tl.AddTask([=]() {
		auto& da = Game::GetGame().GetDataActor();
		da.Add(m_xVibWidth, 0, rightTime, 0);
		da.Add(m_yVibHeight, 0, rightTime, 0);
	}, leftTime + keepTime);

	tl.AddTask([this]() {
		m_killed = true;
	}, leftTime + keepTime + rightTime + 10);
}

void AfterEffect::ScreenVibrator::Draw() const
{
	double xOffset = m_xVibWidth * sin(m_xSinArg) / WIDTH;
	double yOffset = m_yVibHeight * sin(m_ySinArg) / HEIGHT;
	glColor4f(1, 1, 1, 1);
	glDisable(GL_BLEND);
	glBegin(GL_QUADS);
	glTexCoord2d(xOffset, 1+ yOffset); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
	glTexCoord2d(1+ xOffset, 1+ yOffset); glVertex2d(WIDTH / 2, -HEIGHT / 2);
	glTexCoord2d(1+ xOffset, yOffset); glVertex2d(WIDTH / 2, HEIGHT / 2);
	glTexCoord2d(xOffset, yOffset); glVertex2d(-WIDTH / 2, HEIGHT / 2);
	glEnd();
	glEnable(GL_BLEND);
}

void AfterEffect::ScreenVibrator::Update()
{
	m_xSinArg += 1.4;
	m_ySinArg += 1.4;
}

bool AfterEffect::ScreenVibrator::Killed()
{
	return m_killed;
}