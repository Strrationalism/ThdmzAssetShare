#include "Precompiled.h"
#include "KeepingBarrelBlur.h"
#include "Game.h"
#include "Shaders.h"

namespace AfterEffect {
	KeepingBarrelBlurWithPlayer::KeepingBarrelBlurWithPlayer(int inFrame, int keepFrame, int endFrame, double highestPower, int fadeInSlowMotion, int fadeOutSlowMotion, double offsetX, double offsetY)
	{
		m_power = 0;
		m_dataActor.Add(m_power, highestPower, inFrame, fadeInSlowMotion);

		m_taskList.AddTask([this, endFrame, fadeOutSlowMotion]() {
			m_dataActor.Add(m_power, 0, endFrame, fadeOutSlowMotion);
			m_taskList.AddTask([this]() {
				m_finished = true;
			}, endFrame);
		}, inFrame + keepFrame);

		m_offsetX = offsetX;
		m_offsetY = offsetY;
	}

	void KeepingBarrelBlurWithPlayer::Draw() const
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		double x, y;
		Game::GetGame().GetPlayer().GetPos(x, y);
		Basic::Shaders::BarrelBlur(m_power, x + m_offsetX, y + m_offsetY);
		glDisable(GL_BLEND);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(0, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);
		glEnd();
		glUseProgram(0);
		glEnable(GL_BLEND);
	}

	void KeepingBarrelBlurWithPlayer::Update()
	{
		m_taskList.Update();
		m_dataActor.Update();
	}

	bool KeepingBarrelBlurWithPlayer::Killed()
	{
		return m_finished;
	}
}