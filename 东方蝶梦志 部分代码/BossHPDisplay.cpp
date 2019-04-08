#include "Precompiled.h"
#include "BossHPDisplay.h"
#include "glew.h"
#include "Sprite2D.h"
#include "Game.h"

std::optional<Basic::Video::LowerRenderer::GLCallList> Boss::BossHPDisplay::m_drawShell;
Basic::Video::LowerRenderer::GLShader Boss::BossHPDisplay::m_shader;

static GLuint alphaLoc, m_hpLoc;

Boss::BossHPDisplay::BossHPDisplay()
{
	m_died = false;
	m_drawOnTop = true;
	m_coll.radius = 168;
	m_alpha = 0;
	m_dataActor.Add(m_alpha, 1.0, 20, 1);
}

#define CIRCLE_POINTS 128
#pragma warning(disable:4244)

void Boss::BossHPDisplay::Init()
{
	m_shader.Load("BossHPDisplay", "BossHPDisplay");
	alphaLoc = m_shader.UniformLocation("alpha");
	m_hpLoc = m_shader.UniformLocation("m_hp");

	m_drawShell.emplace();
	m_drawShell->DefineBegin(); {
		glDisable(GL_TEXTURE_2D);

		glLineWidth(3.0f);
		glBegin(GL_LINE_STRIP); {
			for (int i = 0; i <= CIRCLE_POINTS; ++i)
				glVertex2d(i, 0);
		}
		glEnd();
		m_shader.Unuse();
		glLineWidth(1.0f);

		glBegin(GL_LINE_STRIP); {
			for (int i = 0; i <= CIRCLE_POINTS; ++i)
				glVertex2d(-131 * sin(2 * M_PI / CIRCLE_POINTS * i), -131 * cos(2 * M_PI / CIRCLE_POINTS * i));
		}
		glEnd();

		glBegin(GL_LINE_STRIP); {
			for (int i = 0; i <= CIRCLE_POINTS; ++i)
				glVertex2d(-125 * sin(2 * M_PI / CIRCLE_POINTS * i), -125 * cos(2 * M_PI / CIRCLE_POINTS * i));
		}
		glEnd();

		glEnable(GL_TEXTURE_2D);
	}
	m_drawShell->DefineEnd();
}

void Boss::BossHPDisplay::Draw() const
{
	m_shader.Use();
	glUniform1f(m_hpLoc, m_hp);
	glUniform1f(alphaLoc, m_alpha);

	glTranslated(m_coll.x, m_coll.y, 0);

	glColor4d(1, 0, 0, m_alpha);

	(*m_drawShell)();

	glLoadIdentity();
	for (auto& p : m_hpPoints) {
		p.sp->Draw();
	}
}

void Boss::BossHPDisplay::Update()
{
	m_dataActor.Update();

	for (auto& p : m_hpPoints) {
		p.sp->SetPos(m_coll.x + p.x, m_coll.y + p.y);
		p.sp->UpdateVertex();
	}

	bool b = Collision::CheckC2C(Game::GetGame().GetPlayer().GetGrazeCollision(), m_coll);

	if (m_playerCover != b) {
		m_playerCover = b;
		if (m_playerCover) {
			//Alphaœ¬Ωµ
			m_dataActor.Add(m_alpha, 0.5, 20, 1);
		}
		else {
			//Alpha…œ…˝
			m_dataActor.Add(m_alpha, 1.0, 20, 1);
		}
	}
}

void Boss::BossHPDisplay::Kill()
{
	m_died = true;
}

void Boss::BossHPDisplay::SetHPPoint(double hp)
{
	auto p = std::make_unique<Basic::Video::Renderer2D::Sprite2D>();
	p->UseImage("GameUI/BossHPPoint");
	p->SetAngle(-360 * (hp+0.025));
	if (hp > 0) m_hpPoints.push_back(
	{
		hp,
		-128 * sin(2 * M_PI*(hp+0.025)),
		-128 * cos(2 * M_PI*(hp+0.025)),
		std::move(p)
	}
	);
}