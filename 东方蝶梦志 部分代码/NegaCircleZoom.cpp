#include "Precompiled.h"
#include "NegaCircleZoom.h"
#include "Shaders.h"

AfterEffect::NegaCircleZoom::NegaCircleZoom(double x, double y)
{
	m_x = x;
	m_y = y;

	double len[] = {
		fabs(-WIDTH / 2 - x),	//左边界
		fabs(WIDTH / 2 - x),	//右边界
		fabs(-HEIGHT / 2 - y),	//上边界
		fabs(HEIGHT / 2 - y)	//下边界
	};

	double maxRadius = *std::max_element(len, len + 4);
	maxRadius *= 1.2;

	m_dataActor.Add(m_size, maxRadius, 50, 5);
	m_taskList.AddTask([this, maxRadius]() {
		m_size = 0;
		m_dataActor.Add(m_size, maxRadius, 50, 2);
		m_useShader1 = false;
	}, 50);
	m_taskList.AddTask([this]() {
		m_killed = true;
	}, 100);
}

void AfterEffect::NegaCircleZoom::Draw() const
{
	m_useShader1 ? Basic::Shaders::NegaCircle(m_x, m_y, m_size) : Basic::Shaders::NegaCircle2(m_x, m_y, m_size);
	glDisable(GL_BLEND);
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
	glTexCoord2d(1, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
	glTexCoord2d(1, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
	glTexCoord2d(0, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);
	glEnd();
	glUseProgram(0);
	glEnable(GL_BLEND);
}

void AfterEffect::NegaCircleZoom::Update()
{
	m_taskList.Update();
	m_dataActor.Update();
}

bool AfterEffect::NegaCircleZoom::Killed()
{
	return m_killed;
}