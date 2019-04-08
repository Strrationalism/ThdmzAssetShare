#include "Precompiled.h"
#include "FlashObject.h"
#include "glew.h"
#include "Sprite2D.h"

Utils::FlashObject::FlashObject(int leftTime, int leftSlowMotion, int rightTime, int rightSlowMotion, int keep, double highestAlpha)
{
	//×ó¶Ë
	m_dataActor.Add(m_alpha, highestAlpha, leftTime, leftSlowMotion);

	//ÖÐ¼ä
	m_taskList.AddTask([this]() {
		m_call();
	}, leftTime);

	//ÓÒ¶Ë
	m_taskList.AddTask([this, rightSlowMotion, rightTime]() {
		m_dataActor.Add(m_alpha, 0, rightTime, rightSlowMotion);
	}, keep + leftTime);

	//½áÊø
	m_taskList.AddTask([this]() {
		m_died = true;
	}, leftTime + keep + rightTime);

	m_died = false;
	m_drawOnTop = true;
}

void Utils::FlashObject::Draw() const
{
	glDisable(GL_TEXTURE_2D);

	glColor4d(m_r, m_g, m_b, m_alpha);
	glBegin(GL_QUADS);
	glVertex2d(-WIDTH / 2, -HEIGHT / 2);
	glVertex2d(WIDTH / 2, -HEIGHT / 2);
	glVertex2d(WIDTH / 2, HEIGHT / 2);
	glVertex2d(-WIDTH / 2, HEIGHT / 2);
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

void Utils::FlashObject::Update()
{
	m_taskList.Update();
	m_dataActor.Update();
}

void Utils::FlashObject::Kill()
{
	m_died = true;
}