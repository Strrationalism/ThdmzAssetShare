#include "Precompiled.h"
#include "BackgroundStage4.h"
#include "Game.h"

BackgroundStage4::BackgroundStage4()
{
	m_screenShotForGrass = std::make_shared<Basic::Video::LowerRenderer::GLTexture>();
	m_grass = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("Grass");
	m_shui = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage1_Water");
	m_clouds = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage4_Clouds");
}

void BackgroundStage4::Draw() const
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);

	glColor4f(0, 0.619607843137255 * 0.35 * m_blueMul, 0.776470588235294 * 0.35 * m_blueMul,1);
	glVertex2f(-WIDTH / 2, -HEIGHT / 2);
	glVertex2f(WIDTH / 2, -HEIGHT / 2);

	glColor4f(0.25 * m_blueMul,0.25 * m_blueMul,0.25 * m_blueMul,1);
	glVertex2f(WIDTH / 2, 110);
	glVertex2f(-WIDTH / 2, 110);

	glVertex2f(-WIDTH / 2, 110);
	glVertex2f(WIDTH / 2, 110);

	glColor4f(0.0274509803921569* 0.1 * m_blueMul, 0.466666666666667* 0.1 * m_blueMul, 0.650980392156863* 0.1 * m_blueMul, 1);
	glVertex2f(WIDTH / 2, HEIGHT / 2);
	glVertex2f(-WIDTH / 2, HEIGHT / 2);


	glEnd();
	glEnable(GL_TEXTURE_2D);

	const auto cloudOffset = m_time / 10000.0;

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glBindTexture(GL_TEXTURE_2D, *m_clouds);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_QUADS); {
		glTexCoord2d(cloudOffset, 0); glVertex2d(-640, -360);
		glTexCoord2d(1 + cloudOffset, 0); glVertex2d(640, -360);
		glTexCoord2d(1 + cloudOffset, 1); glVertex2d(640, 180);
		glTexCoord2d(cloudOffset, 1); glVertex2d(-640, 180);
	}
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	
	if (UsingBeautifulBG()) {
		//用于后期的反射效果
		Basic::Shaders::Stage4Relfection(m_time / 100.0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, *m_shui);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *m_clouds);
		glBegin(GL_QUADS); {
			glTexCoord2d(cloudOffset, -1); glVertex3d(-640, 360,0);
			glTexCoord2d(1 + cloudOffset, -1); glVertex3d(640, 360,0);
			glTexCoord2d(1 + cloudOffset, 0); glVertex3d(640, 110,-1);
			glTexCoord2d(cloudOffset, 0); glVertex3d(-640, 110,-1);
		}
		glEnd();

		// 用于后期的玻璃破碎效果
		Basic::Shaders::Stage4Grass(static_cast<float>(m_glassDepth));
		//在这绑定一个屏幕截图
		int w, h;
		SDL_GetWindowSize(Basic::Video::Window::GetWindow(), &w, &h);
		glBindTexture(GL_TEXTURE_2D, *m_screenShotForGrass);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, *m_grass);
		glBegin(GL_QUADS); {
			glTexCoord2d(0, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
			glTexCoord2d(1, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
			glTexCoord2d(1, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
			glTexCoord2d(0, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);
		}
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(0);
}

void BackgroundStage4::Update()
{
	m_time++;

	m_dataActor.Update();
	m_taskList.Update();
}

void BackgroundStage4::Init()
{

}

void BackgroundStage4::Reset()
{
	m_dataActor.Clear();
	m_taskList.Clear();

	m_time = 0;
	m_glassDepth = 0;

	m_blueMul = 0;

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_glassDepth, 1.0, 10, 1);
	}, 1753);

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_blueMul, 1, 50, 1);
	}, 17000);
}

void BackgroundStage4::SkipToBossMode()
{
	m_glassDepth = 1;
	m_blueMul = 1;
}
