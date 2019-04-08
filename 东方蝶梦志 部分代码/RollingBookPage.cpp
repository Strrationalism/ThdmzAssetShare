#include "Precompiled.h"
#include "RollingBookPage.h"
#include "Game.h"
#include "Shaders.h"

AfterEffect::RollingBookPage::RollingBookPage(int frame, int slowMotion, const std::shared_ptr<Basic::Video::LowerRenderer::GLTexture>& ss):
	m_screenshot(ss)
{
	m_data.Add(m_shaderTime, 1.5, frame, slowMotion);
	Game::GetGame().GetTaskList().AddTask([this]() {
		m_finished = true;
	}, frame);

}

bool AfterEffect::RollingBookPage::Killed()
{
	return m_finished;
}

void AfterEffect::RollingBookPage::Draw() const
{
	Basic::Shaders::PaperRoll(m_shaderTime);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *m_screenshot);

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
	glActiveTexture(GL_TEXTURE0);
}

void AfterEffect::RollingBookPage::Update()
{
	m_data.Update();
}