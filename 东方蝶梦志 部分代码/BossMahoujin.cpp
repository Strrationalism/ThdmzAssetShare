#include "Precompiled.h"
#include "BossMahoujin.h"
#include "Sprite2D.h"
#include "ImageCacheManager.h"
#include "Globals.h"
#include "Shaders.h"
#include "Game.h"

Boss::BossMahoujin::BossMahoujin()
{
	m_died = false;
	m_drawOnTop = true;
	m_sp.UseImage("GameUI/BossMahoujin");
	m_shadow.UseImage("GameUI/BossMahoujin", 1);
	m_dataActor.Add(m_progress, 1, 30, 1);

	glBindTexture(GL_TEXTURE_2D, m_circleWaveFB);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void Boss::BossMahoujin::Hide()
{
	m_dataActor.Add(m_alpha, 0, 20, 1);
}

void Boss::BossMahoujin::Show()
{
	m_dataActor.Add(m_alpha, 1, 20, 1);
}

void Boss::BossMahoujin::Update()
{
	m_angle += 1.0f;
	m_sp.SetAngle(m_angle);
	m_dataActor.Update();

	m_sp.SetAlpha(m_alpha * m_progress * (0.7F - sinf((float)m_cnt) * 0.3F));
	m_sp.SetZoom((1.95f - m_progress)*m_size);

	m_shadow.SetZoom(m_progress * 1.3 * m_alpha);
	m_shadow.SetAlpha(m_progress * 0.5);

	m_size = 1.0f + sin(m_cnt) * 0.15f;
	if (m_effectSinSize) {
		m_cnt += 0.1f;
		m_angle += 1.5f;
	}
	else {
		m_cnt += 0.025f;
	}
	m_frames++;
	m_sp.UpdateVertex();
	m_shadow.UpdateVertex();
}

void Boss::BossMahoujin::Draw() const
{
	int screenW, screenH;
	Game::GetGame().GetScreenSize(screenW, screenH);

	glBindTexture(GL_TEXTURE_2D, m_circleWaveFB);

	//坐标转换
	float clipSizeW, clipSizeH;	//截取区域大小
	float clipX, clipY;

	clipSizeW = 512.0 / WIDTH * screenW;
	clipSizeH = 512.0 / HEIGHT * screenH;
	clipX = (WIDTH / 2 + m_x - 256) / WIDTH * screenW;
	clipY = (HEIGHT / 2 - m_y - 256) / HEIGHT * screenH;

	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, clipX, clipY, clipSizeW, clipSizeH, 0);

	Basic::Shaders::CircleWave(m_frames*0.125, m_progress * m_alpha);

	glBegin(GL_QUADS); {
		glTexCoord2f(0, 1); glVertex2f(m_x - 256, m_y - 256);
		glTexCoord2f(1, 1); glVertex2f(m_x + 256, m_y - 256);
		glTexCoord2f(1, 0); glVertex2f(m_x + 256, m_y + 256);
		glTexCoord2f(0, 0); glVertex2f(m_x - 256, m_y + 256);
	}
	glEnd();

	glUseProgram(0);

	m_shadow.Draw();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_sp.Draw();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(0);
}

void Boss::BossMahoujin::Kill()
{
	m_dataActor.Add(m_progress, 0, 30, 2);
	Game::GetGame().GetTaskList().AddTask([this]() {
		m_died = true;
	}, 60);
}