#include "NuzPrecompiled.h"
#include "TitleToGame.h"
#include "Shaders.h"
#include "Window.h"
#include "ActivityEngine.h"

Activities::Title::MenuStack::TitleToGame::TitleToGame(const std::shared_ptr<Basic::ActivityEngine::Activity>& gact):
	m_gact { gact },
	m_game { std::make_shared<Basic::Video::LowerRenderer::GLTexture>() },
	m_menu { std::make_shared<Basic::Video::LowerRenderer::GLTexture>() }
{
	m_rollTimer = -0.25f;
}

void Activities::Title::MenuStack::TitleToGame::OnEntryToThis()
{
	m_rollTimer.Tween(1.25f, 0.5f, 1);
	m_tl.AddTask([this] {
		Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Goto(m_gact);
	}, 0.6f);
}

void Activities::Title::MenuStack::TitleToGame::OnDraw() const
{
}

void Activities::Title::MenuStack::TitleToGame::OnDrawYSJD() const
{
}

void Activities::Title::MenuStack::TitleToGame::OnDrawPost() const
{
	int w, h;
	SDL_GetWindowSize(Basic::Video::Window::GetWindow(), &w, &h);

	m_menu->Bind();
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0);

	glClear(GL_COLOR_BUFFER_BIT);
	m_gact->OnDraw();
	m_game->Bind();
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0);

	Basic::Shaders::PaperRoll(m_rollTimer);
	m_menu->Bind(1);
	m_game->Bind(0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 1); glVertex2f(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2f(1, 1); glVertex2f(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2f(1, 0); glVertex2f(WIDTH / 2, HEIGHT / 2);
		glTexCoord2f(0, 0); glVertex2f(-WIDTH / 2, HEIGHT / 2);
	}
	glEnd();
	glUseProgram(0);
}

void Activities::Title::MenuStack::TitleToGame::OnUpdate(float deltaTime)
{
	m_tl.Update(deltaTime);
	m_rollTimer.Update(deltaTime);
	m_gact->OnUpdate(deltaTime);
}

void Activities::Title::MenuStack::TitleToGame::OnEvent(const Basic::Input::Message & msg)
{
	m_gact->OnEvent(msg);
}
