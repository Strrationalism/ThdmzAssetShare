#include "Precompiled.h"
#include "BackgroundStage1.h"
#include "FlashObject.h"
#include "Game.h"

Background::BackgroundStage1::BackgroundStage1()
{
	m_shan = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage(UsingBeautifulBG() ? "StageBackground/Stage1_shan2" : "StageBackground/Stage1_shan2_LOW");
	m_shui = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage1_Water");
	m_cloud = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("Cloud");
	m_starSky = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage3_MainBG_StarSky");

	m_starSkyBlured = std::make_shared<Basic::Video::LowerRenderer::GLTexture>();

	//创建3D摄像机

	//创建ModelView矩阵
	glScaled(8.0f, 1, 1);
	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelView);

	//创建Projection矩阵
	glLoadIdentity();
	glFrustum(-WIDTH / 2, WIDTH / 2, HEIGHT / 2, -HEIGHT / 2, 1, 100000);
	glGetDoublev(GL_MODELVIEW_MATRIX, m_projection);

	glLoadIdentity();
}

Background::BackgroundStage1::~BackgroundStage1()
{
}

#define skyDepth -8.5f

void Background::BackgroundStage1::DrawVSLightRef() const
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	Basic::Shaders::VistaLight(m_timer / 40.0);
	glBindTexture(GL_TEXTURE_2D, *m_cloud);
	glBegin(GL_QUADS);
	glColor4f(1, 1, 1, 0.45F);
	glTexCoord2d(0, 0);
	glVertex3d(-WIDTH / 2, HEIGHT / 2, skyDepth + 0.3);
	glTexCoord2d(1, 0);
	glVertex3d(WIDTH / 2, HEIGHT / 2, skyDepth + 0.3);
	glTexCoord2d(1, 0.25);
	glColor4f(1, 1, 1, 0);
	glVertex3d(WIDTH / 2, HEIGHT / 4, skyDepth + 0.3);
	glTexCoord2d(0, 0.25);
	glVertex3d(-WIDTH / 2, HEIGHT / 4, skyDepth + 0.3);
	glEnd();
	glUseProgram(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Background::BackgroundStage1::DrawVSLight() const
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	Basic::Shaders::VistaLight(m_timer / 40.0);
	glBindTexture(GL_TEXTURE_2D, *m_cloud);
	glBegin(GL_QUADS);
	glColor4f(1, 1, 1, 1.2F);
	glTexCoord2d(0, 0);
	glVertex3d(-WIDTH / 2, -HEIGHT / 2, skyDepth + 0.3);
	glTexCoord2d(1, 0);
	glVertex3d(WIDTH / 2, -HEIGHT / 2, skyDepth + 0.3);
	glTexCoord2d(1, 1);
	glVertex3d(WIDTH / 2, HEIGHT / 2, skyDepth + 0.3);
	glTexCoord2d(0, 1);
	glVertex3d(-WIDTH / 2, HEIGHT / 2, skyDepth + 0.3);
	glEnd();
	glUseProgram(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Background::BackgroundStage1::Draw() const
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (UsingBeautifulBG())
		Basic::Shaders::GaussBlurW(2.5f);

	const double SkyOffset = m_timer * 0.00001;

	glColor4f(1, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, *m_starSky);
	glBegin(GL_QUADS);
	glTexCoord2d(SkyOffset, 0);
	glVertex3d(-WIDTH / 2, -HEIGHT / 2, skyDepth + 0.3);
	glTexCoord2d(1.2f + SkyOffset, 0);
	glVertex3d(WIDTH / 2, -HEIGHT / 2, skyDepth + 0.3);
	glTexCoord2d(1.2f + SkyOffset, 1);
	glVertex3d(WIDTH / 2, HEIGHT / 2, skyDepth + 0.3);
	glTexCoord2d(SkyOffset, 1);
	glVertex3d(-WIDTH / 2, HEIGHT / 2, skyDepth + 0.3);
	glEnd();

	if (UsingBeautifulBG())
	{
		int screenW, screenH;
		SDL_GetWindowSize(Basic::Video::Window::GetWindow(), &screenW, &screenH);
		m_starSkyBlured->Bind();
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, screenW, screenH, 0);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(0);
		const double cloudSpeed = 1;
		glBindTexture(GL_TEXTURE_2D, *m_cloud);
		glLoadIdentity();
		glBegin(GL_QUADS); {
			glColor4f(10 / 255.0f, 14 / 255.0f, 142 / 255.0f, 0.5f);
			glTexCoord2d(SkyOffset * cloudSpeed, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
			glTexCoord2d(1 + SkyOffset * cloudSpeed, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
			glTexCoord2d(1 + SkyOffset * cloudSpeed, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
			glTexCoord2d(SkyOffset * cloudSpeed, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);

			glColor4f(10 / 255.0f, 14 / 255.0f, 142 / 255.0f, 0.45f);
			glTexCoord2d(SkyOffset * cloudSpeed * 4.0, 0); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
			glTexCoord2d(1 + SkyOffset * cloudSpeed * 4.0, 0); glVertex2d(WIDTH / 2, -HEIGHT / 2);
			glTexCoord2d(1 + SkyOffset * cloudSpeed * 4.0, 1); glVertex2d(WIDTH / 2, HEIGHT / 2);
			glTexCoord2d(SkyOffset * cloudSpeed * 4.0f, 1); glVertex2d(-WIDTH / 2, HEIGHT / 2);
		}glEnd();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glColor4f(1, 1, 1, 1);
		m_starSkyBlured->Bind();
		Basic::Shaders::GaussBlurH(4.0f);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 0);
		glVertex3d(-WIDTH / 2, -HEIGHT / 2, skyDepth + 0.3);
		glTexCoord2d(1.0f, 0);
		glVertex3d(WIDTH / 2, -HEIGHT / 2, skyDepth + 0.3);
		glTexCoord2d(1.0f, 1);
		glVertex3d(WIDTH / 2, HEIGHT / 2, skyDepth + 0.3);
		glTexCoord2d(0, 1);
		glVertex3d(-WIDTH / 2, HEIGHT / 2, skyDepth + 0.3);
		glEnd();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(0);
	}

	//}
	if (UsingBeautifulBG())
		DrawVSLight();

	glColor4d(0.101960784313725, 0.0862745098039216, 0.580392156862745, 1.0);

	glBindTexture(GL_TEXTURE_2D, *m_shan);
	const double moreHigh = -80;
	const double shanSpeed = 0.0005f;
	const double moutainDepth = -8.0f;
	glBegin(GL_QUADS); {
		glTexCoord2d(shanSpeed * m_cnt, 0.0012f); glVertex3d(-WIDTH / 2, -100 + moreHigh, moutainDepth);
		glTexCoord2d(0.58394f + shanSpeed * m_cnt, 0.0012f); glVertex3d(WIDTH / 2, -100 + moreHigh, moutainDepth);
		glTexCoord2d(0.58394f + shanSpeed * m_cnt, 1); glVertex3d(WIDTH / 2, 130 + moreHigh, moutainDepth);
		glTexCoord2d(shanSpeed * m_cnt, 1); glVertex3d(-WIDTH / 2, 130 + moreHigh, moutainDepth);
	}
	glEnd();

	glLoadMatrixd(m_modelView);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();
	glFrustum(-WIDTH / 2, WIDTH / 2, HEIGHT / 2, -HEIGHT / 2, 1, 100000);
	glTranslated(0, 0, 0.25 * (1 + sin(m_timer / 100.0)));
	glRotated(0.0025 * (1 + sin(m_timer / 100.0)), 1, 0, 0.1);

	//绘制水面
	const double shuiGoSpeed = 0.005f;

	{
		//水面

		const double waterAlpha = std::clamp((1 - m_waterOrIce), 0.0, 0.5) * 2;

		glEnable(GL_FOG);
		const float fogColor[] = { 0.00392156862745098f,0.00784313725490196f,0.09411764705882352f };
		glFogfv(GL_FOG_COLOR, fogColor);

		glBindTexture(GL_TEXTURE_2D, *m_shui);
		glBegin(GL_QUADS);
		{
			glColor4d(0x85 / 256.0f / 4, 204.0f / 256.0f / 4, 253.0f / 256.0f / 4, waterAlpha);
			glTexCoord2d(-0.001f * m_cnt + shuiGoSpeed * m_cnt, 2.5f - 0.001f * m_cnt); glVertex3d(-WIDTH / 2, HEIGHT / 2, -1);
			glTexCoord2d(-0.001f * m_cnt + shuiGoSpeed * m_cnt, -0.001f * m_cnt); glVertex3d(-WIDTH / 2, HEIGHT / 2, -8);
			glTexCoord2d(2.5f - 0.001f * m_cnt + shuiGoSpeed * m_cnt, -0.001f * m_cnt); glVertex3d(WIDTH / 2, HEIGHT / 2, -8);
			glTexCoord2d(2.5f - 0.001f * m_cnt + shuiGoSpeed * m_cnt, 2.5f - 0.001f * m_cnt); glVertex3d(WIDTH / 2, HEIGHT / 2, -1);

			glColor4d(0x85 / 256.0f / 4, 204.0f / 256.0f / 4, 253.0f / 256.0f / 4, 0.5 * waterAlpha);
			glTexCoord2d(0.001f * m_cnt + shuiGoSpeed * m_cnt, 2.5f + 0.001f * m_cnt); glVertex3d(-WIDTH / 2, HEIGHT / 2 - 10, -1);
			glTexCoord2d(0.001f * m_cnt + shuiGoSpeed * m_cnt, 0.001f * m_cnt); glVertex3d(-WIDTH / 2, HEIGHT / 2 - 10, -8);
			glTexCoord2d(2.5f + 0.001f * m_cnt + shuiGoSpeed * m_cnt, 0.001f * m_cnt); glVertex3d(WIDTH / 2, HEIGHT / 2 - 10, -8);
			glTexCoord2d(2.5f + 0.001f * m_cnt + shuiGoSpeed * m_cnt, 2.5f + 0.001f * m_cnt); glVertex3d(WIDTH / 2, HEIGHT / 2 - 10, -1);
		}
		glEnd();
		glFogf(GL_FOG_DENSITY, m_fog_density);
	}

	{
		const double iceAlpha = std::clamp(m_waterOrIce, 0.0, 0.5) * 2;
		glDisable(GL_TEXTURE_2D);

		glEnable(GL_FOG);
		const float fogColor[] = { 0.00392156862745098f,0.00784313725490196f,0.09411764705882352f };
		glFogfv(GL_FOG_COLOR, fogColor);
		glColor4d(0x85 / 256.0f / 4, 204.0f / 256.0f / 4, 253.0f / 256.0f / 4, iceAlpha);
		glBegin(GL_QUADS);
		{
			glVertex3d(-WIDTH / 2, HEIGHT / 2, -1);
			glVertex3d(-WIDTH / 2, HEIGHT / 2, -8);
			glVertex3d(WIDTH / 2, HEIGHT / 2, -8);
			glVertex3d(WIDTH / 2, HEIGHT / 2, -1);
		}
		glEnd();
		glEnable(GL_TEXTURE_2D);
		glDisable(GL_FOG);
	}

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	glDisable(GL_FOG);

	//山的倒影
	if (UsingBeautifulBG()) {
		glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1, 1, 1, 0.095f);
		glBindTexture(GL_TEXTURE_2D, *m_shan);
		glBegin(GL_QUADS); {
			glTexCoord2d(shanSpeed * m_cnt, 0.0007f); glVertex3d(-WIDTH / 2, 360 + moreHigh, moutainDepth);
			glTexCoord2d(0.58394f + shanSpeed * m_cnt, 0.0007f); glVertex3d(WIDTH / 2, 360 + moreHigh, moutainDepth);
			glTexCoord2d(0.58394f + shanSpeed * m_cnt, 1); glVertex3d(WIDTH / 2, 128 + moreHigh, moutainDepth);
			glTexCoord2d(shanSpeed * m_cnt, 1); glVertex3d(-WIDTH / 2, 128 + moreHigh, moutainDepth);
		}
		glEnd();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (UsingBeautifulBG())
			DrawVSLightRef();
	}

	//体积雾
	{
		glColor4f(1, 1, 1, 0.05 * (1 - m_waterOrIce));
		double xOffset = m_timer * 0.01;
		glBindTexture(GL_TEXTURE_2D, *m_cloud);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBegin(GL_QUADS); {
			glTexCoord2d(1 - xOffset, 1); glVertex2f(-WIDTH / 2, HEIGHT / 2);
			glTexCoord2d(-xOffset, 1); glVertex2f(WIDTH / 2, HEIGHT / 2);
			glTexCoord2d(-xOffset, 0); glVertex2f(WIDTH / 2, -HEIGHT / 2);
			glTexCoord2d(1 - xOffset, 0); glVertex2f(-WIDTH / 2, -HEIGHT / 2);

			glTexCoord2d(1 + xOffset / 2, 1); glVertex2f(WIDTH / 2, HEIGHT / 2);
			glTexCoord2d(xOffset / 2, 1); glVertex2f(-WIDTH / 2, HEIGHT / 2);
			glTexCoord2d(xOffset / 2, 0); glVertex2f(-WIDTH / 2, -HEIGHT / 2);
			glTexCoord2d(1 + xOffset / 2, 0); glVertex2f(WIDTH / 2, -HEIGHT / 2);
		}
		glEnd();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glEnable(GL_BLEND);
}

void Background::BackgroundStage1::Update()
{
	m_timer++;
	m_dataActor.Update();
	m_taskList.Update();
	m_cnt += 0.25;

	float fogColor[4] = {
		(float)m_fogColor[0],
		(float)m_fogColor[1],
		(float)m_fogColor[2],
		(float)m_fogColor[3]
	};
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, m_fog_density);
}

void Background::BackgroundStage1::Init()
{
}

void Background::BackgroundStage1::Reset()
{
	m_timer = 0;
	m_taskList.Clear();
	m_dataActor.Clear();

	m_drawWaterLight = true;
	//m_drawMountainShadow = true;
	m_cnt = 0;
	m_waterOrIce = 0;
	m_waterFogMul = 1.5;

	for (auto& p : m_fogColor)
		p = 1;

	//选定雾的类型
	glFogi(GL_FOG_MODE, GL_EXP2);
	//确定雾的渲染方式
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, 0);
	glFogf(GL_FOG_END, 12);
	m_fog_density = 0.085;

	//断奏部分开始出雾
	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_fog_density, 0.2, 2484 * 2, 0);
		for (auto& p : m_fogColor)
			m_dataActor.Add(p, 0.8, 2484 * 2, 0);
	}, 2655 * 2);

	//齐鲁诺来了！
	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_waterOrIce, 1.0, 50, 1);
		m_drawWaterLight = false;

		/*auto p = new Utils::FlashObject(10, 1, 10, 1, 0);
		p->SetColor(0, 0, 0);
		p->SetDrawOnTop(true);
		p->SetCall([this]() {
		});
		Game::GetGame().GetMiscManagerBottom().Insert(p);*/
	}, 5130 * 2);

	//齐鲁诺走了
	m_taskList.AddTask([this]() {
		/*auto p = new Utils::FlashObject(10, 1, 10, 1, 0);
		p->SetColor(0, 0, 0);
		p->SetDrawOnTop(true);
		p->SetCall([this]() {
		});
		Game::GetGame().GetMiscManagerBottom().Insert(p);*/

		m_dataActor.Add(m_waterOrIce, 0, 50, 1);
		m_fog_density = 0.3;
		m_waterFogMul = 1.0;

		for (auto& p : m_fogColor)
			p = 0.9f;
	}, 7785 * 2);

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_fog_density, 0.2, 500 * 2, 0);
	}, 7791 * 2);
}