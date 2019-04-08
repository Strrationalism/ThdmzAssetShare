#include "Precompiled.h"
#include "BackgroundStage3.h"
#include "Game.h"

using namespace Basic::Video::LowerRenderer;

Background::BackgroundStage3::BackgroundStage3()
{
	if (UsingBeautifulBG()) {
		m_gs1 = std::make_shared<Basic::Video::LowerRenderer::GLTexture>();
		m_gs2 = std::make_shared<Basic::Video::LowerRenderer::GLTexture>();
	}

	//加载纹理
	//m_bk0 = ImageCacheManager::LoadImage("StageBackground/Stage3R_BK0");
	m_bk1 = ImageCacheManager::LoadImage("StageBackground/Stage3R_BK1");
	m_flamer = ImageCacheManager::LoadImage("StageBackground/Stage3R_FLAMER");
	m_sTex = ImageCacheManager::LoadImage("StageBackground/Stage3R_S");
	m_cloud = ImageCacheManager::LoadImage("Cloud");
}

Background::BackgroundStage3::~BackgroundStage3()
{
}

void Background::BackgroundStage3::stageDraw() const
{
	//载入摄像机

	glRotated(20.0, 0.0, 1.0, 0);
	glFrustum(-640, 640, 360, -360, 0.1, 1000.0);

	glTranslated(-400.0, 0, 0.0);

	glPushMatrix();
	glRotated(time / 4.0, 0.0, 0.0, 1.0);

	//环境
	glEnable(GL_FOG);

	//绘制开始
	glColor4f(1.0, 1.0, 1.0, m_global_alpha);
	if (UsingBeautifulBG())
		Basic::Shaders::Stage3RWall(19.0, time / 50.0, m_playerX, m_playerY);
	glBindTexture(GL_TEXTURE_2D, *m_bk1);
	const double xOffset = time / 155.0;
	const double xMul = 80.0;
	const double VERTEX_X = 1800;
	const double VERTEX_DEEP_Z = -100.0;

	const double TEXCOORD_TOP = 1.0;
	glBegin(GL_QUADS); {
		glTexCoord2d(xOffset, 0.0); glVertex3d(-VERTEX_X, -VERTEX_X, -0.1);
		glTexCoord2d(xOffset, TEXCOORD_TOP); glVertex3d(-VERTEX_X, VERTEX_X, -0.1);
		glTexCoord2d(xMul + xOffset, TEXCOORD_TOP); glVertex3d(-VERTEX_X, VERTEX_X, VERTEX_DEEP_Z);
		glTexCoord2d(xMul + xOffset, 0.0); glVertex3d(-VERTEX_X, -VERTEX_X, VERTEX_DEEP_Z);

		glTexCoord2d(xOffset, TEXCOORD_TOP); glVertex3d(VERTEX_X, -VERTEX_X, -0.1);
		glTexCoord2d(xOffset, 0.0); glVertex3d(VERTEX_X, VERTEX_X, -0.1);
		glTexCoord2d(xMul + xOffset, 0.0); glVertex3d(VERTEX_X, VERTEX_X, VERTEX_DEEP_Z);
		glTexCoord2d(xMul + xOffset, TEXCOORD_TOP); glVertex3d(VERTEX_X, -VERTEX_X, VERTEX_DEEP_Z);

		glTexCoord2d(xOffset, TEXCOORD_TOP); glVertex3d(-VERTEX_X, VERTEX_X, -0.1);
		glTexCoord2d(xMul + xOffset, TEXCOORD_TOP); glVertex3d(-VERTEX_X, VERTEX_X, VERTEX_DEEP_Z);
		glTexCoord2d(xMul + xOffset, 0.0); glVertex3d(VERTEX_X, VERTEX_X, VERTEX_DEEP_Z);
		glTexCoord2d(xOffset, 0.0); glVertex3d(VERTEX_X, VERTEX_X, -0.1);

		glTexCoord2d(xOffset, 0.0); glVertex3d(-VERTEX_X, -VERTEX_X, -0.1);
		glTexCoord2d(xMul + xOffset, 0.0); glVertex3d(-VERTEX_X, -VERTEX_X, VERTEX_DEEP_Z);
		glTexCoord2d(xMul + xOffset, TEXCOORD_TOP); glVertex3d(VERTEX_X, -VERTEX_X, VERTEX_DEEP_Z);
		glTexCoord2d(xOffset, TEXCOORD_TOP); glVertex3d(VERTEX_X, -VERTEX_X, -0.1);
	}
	glEnd();

	//切换到粒子环境
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, *m_sTex);
	glColor4f(1, 1, 1, 0.35f);

	glBegin(GL_QUADS);
	for (auto& p : m_particles)
		p.Draw(m_sTex.get());
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_FOG);
	glUseProgram(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//卸载环境

	//卸载摄像机
}

void Background::BackgroundStage3::bossDraw() const
{
	glTranslated(-m_playerX / 10000, m_playerY / 10000, 0.0);
	glFrustum(-640, 640, 360, -360, 0.1, 1.0);

	//火焰

	glColor4f(1.0, 1.0, 1.0, m_frameAlpha);
	const double offsetY = 350;
	Basic::Shaders::FlameOnBoard(time);
	m_cloud->Bind(0);
	glBegin(GL_QUADS); {
		glTexCoord2d(1.0, 1.0); glVertex3d(-600, -360 + offsetY, -0.4);
		glTexCoord2d(1.0, 0.0); glVertex3d(-600, 360 + offsetY, -0.4);
		glTexCoord2d(0.0, 0.0); glVertex3d(600, 360 + offsetY, -0.4);
		glTexCoord2d(0.0, 1.0); glVertex3d(600, -360 + offsetY, -0.4);
	}
	glEnd();
	glUseProgram(0);

	//正面墙
	glColor4f(1.0, 1.0, 1.0, m_global_alpha * m_boss_roomlight);
	const double bk0Mul = 1.5;
	glBindTexture(GL_TEXTURE_2D, *m_flamer);
	glBegin(GL_QUADS); {
		glTexCoord2d(0.0, 0.0); glVertex3d(-1280 * bk0Mul, -720 * bk0Mul, -0.5);
		glTexCoord2d(1.0, 0.0); glVertex3d(1280 * bk0Mul, -720 * bk0Mul, -0.5);
		glTexCoord2d(1.0, 1.0); glVertex3d(1280 * bk0Mul, 720 * bk0Mul, -0.5);
		glTexCoord2d(0.0, 1.0); glVertex3d(-1280 * bk0Mul, 720 * bk0Mul, -0.5);
	}
	glEnd();

	//其他墙
	glBindTexture(GL_TEXTURE_2D, *m_bk1);
	glBegin(GL_QUADS); {
		//地板
		const double floorTexCoordMulY = 1.75;
		glTexCoord2d(1.0, floorTexCoordMulY); glVertex3d(1280 * bk0Mul, 720 * bk0Mul, -0.5);
		glTexCoord2d(0.0, floorTexCoordMulY); glVertex3d(-1280 * bk0Mul, 720 * bk0Mul, -0.5);
		glTexCoord2d(0.0, 0.0); glVertex3d(-1280 * bk0Mul, 720 * bk0Mul, 0.5);
		glTexCoord2d(1.0, 0.0); glVertex3d(1280 * bk0Mul, 720 * bk0Mul, 0.5);

		//天花板
		glTexCoord2d(1.0, floorTexCoordMulY); glVertex3d(1280 * bk0Mul, -720 * bk0Mul, -0.5);
		glTexCoord2d(0.0, floorTexCoordMulY); glVertex3d(-1280 * bk0Mul, -720 * bk0Mul, -0.5);
		glTexCoord2d(0.0, 0.0); glVertex3d(-1280 * bk0Mul, -720 * bk0Mul, 0.5);
		glTexCoord2d(1.0, 0.0); glVertex3d(1280 * bk0Mul, -720 * bk0Mul, 0.5);

		//左侧
		const double floorTexCoordMulX = 0.25;
		glTexCoord2d(floorTexCoordMulX, 1.0); glVertex3d(-1280 * bk0Mul, 720 * bk0Mul, -0.5);
		glTexCoord2d(0.0, 1.0); glVertex3d(-1280 * bk0Mul, -720 * bk0Mul, -0.5);
		glTexCoord2d(0.0, 0.0); glVertex3d(-1280 * bk0Mul, -720 * bk0Mul, 0.5);
		glTexCoord2d(floorTexCoordMulX, 0.0); glVertex3d(-1280 * bk0Mul, 720 * bk0Mul, 0.5);

		//右侧
		glTexCoord2d(floorTexCoordMulX, 1.0); glVertex3d(1280 * bk0Mul, 720 * bk0Mul, -0.5);
		glTexCoord2d(0.0, 1.0); glVertex3d(1280 * bk0Mul, -720 * bk0Mul, -0.5);
		glTexCoord2d(0.0, 0.0); glVertex3d(1280 * bk0Mul, -720 * bk0Mul, 0.5);
		glTexCoord2d(floorTexCoordMulX, 0.0); glVertex3d(1280 * bk0Mul, 720 * bk0Mul, 0.5);
	}
	glEnd();
}

void Background::BackgroundStage3::stageUpdate()
{
	if (time % 200 == 1 && m_switch_particle) {
		int r = rand();

		m_particles.push_back(Particle{
			r % 11 ,-5.0
		});
	}

	auto nend = std::remove_if(m_particles.begin(), m_particles.end(), [](auto& i) {return i.zDepth >= 0.5; });
	m_particles.erase(nend, m_particles.end());

	for (auto& p : m_particles)p.Update();
}

void Background::BackgroundStage3::Draw() const
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	if (m_scene == Stage) stageDraw();
	else bossDraw();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);


	if (UsingBeautifulBG() && m_gauss > 0.001)
	{
		glColor4f(1, 1, 1, 1);
		

		glBindTexture(GL_TEXTURE_2D, *m_gs2);
		int w, h;
		Game::GetGame().GetScreenSize(w, h);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0);

		glBindTexture(GL_TEXTURE_2D, *m_gs1);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		Basic::Shaders::GaussBlurW(m_gauss);
		glBegin(GL_QUADS); {
			glTexCoord2d(0.0, 0.0); glVertex2d(-320, 180);
			glTexCoord2d(1.0, 0.0); glVertex2d(320, 180);
			glTexCoord2d(1.0, 1.0); glVertex2d(320, -180);
			glTexCoord2d(0.0, 1.0); glVertex2d(-320, -180);

		}
		glEnd();

		//glBindTexture(GL_TEXTURE_2D, *m_gs1);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		Basic::Shaders::GaussBlurH(m_gauss);
		glBegin(GL_QUADS); {
			glTexCoord2d(0.0, 0.0); glVertex2d(-320, 180);
			glTexCoord2d(1.0, 0.0); glVertex2d(320, 180);
			glTexCoord2d(1.0, 1.0); glVertex2d(320, -180);
			glTexCoord2d(0.0, 1.0); glVertex2d(-320, -180);

		}
		glEnd();
		glUseProgram(0);

		//glBindTexture(GL_TEXTURE_2D, *m_gs1);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0);

		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBindTexture(GL_TEXTURE_2D, *m_gs1);
		glBegin(GL_QUADS); {

			glTexCoord2d(0.25 + 0.125, 0.25 + 0.125); glVertex2d(-640, 360);
			glTexCoord2d(0.75 - 0.125, 0.25 + 0.125); glVertex2d(640, 360);
			glTexCoord2d(0.75 - 0.125, 0.75 - 0.125); glVertex2d(640, -360);
			glTexCoord2d(0.25 + 0.125, 0.75 - 0.125); glVertex2d(-640, -360);

		}
		glEnd();

		glColor4d(1, 1, 1, 1.0 - m_gauss / 8.0);
		glBindTexture(GL_TEXTURE_2D, *m_gs2);
		glBegin(GL_QUADS); {
			glTexCoord2d(0.0, 0.0); glVertex2d(-640, 360);
			glTexCoord2d(1.0, 0.0); glVertex2d(640, 360);
			glTexCoord2d(1.0, 1.0); glVertex2d(640, -360);
			glTexCoord2d(0.0, 1.0); glVertex2d(-640, -360);

		}
		glEnd();

	}

	glLoadIdentity();
}

void Background::BackgroundStage3::Update()
{
	time++;

	if (!Game::GetGame().GetPlayer().Died())
	{
		Game::GetGame().GetPlayer().GetPos(m_playerX, m_playerY);
	}

	m_taskList.Update();
	m_dataActor.Update();

	if (m_scene == Stage) stageUpdate();
	else {
		m_boss_roomlight = 0.8 + 0.025 * sin(time + 0.005) + 0.05*cos(time / 2.0 + 0.005);
	}
}

void Background::BackgroundStage3::UpdateWithoutGame()
{
	time++;

	m_playerX = 0;
	m_playerY = 0;

	m_taskList.Update();
	m_dataActor.Update();

	stageUpdate();
}

void Background::BackgroundStage3::Init()
{

	//雾
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 0);
	glFogf(GL_FOG_END, 5.0);
	float fogColor[] = { 0.0,0.0,0.0,1.0 };
	glFogfv(GL_FOG_COLOR, fogColor);
}

void Background::BackgroundStage3::Reset()
{
	m_playerX = 0;
	m_playerY = 0;

	time = 0;
	m_frameAlpha = 0;
	m_particles.clear();

	m_taskList.Clear();
	m_dataActor.Clear();

	m_switch_particle = false;
	m_global_alpha = 1.0;

	m_scene = Stage;

	m_taskList.AddTask([this]() {
		m_switch_particle = true;
	}, 1500);

	m_taskList.AddTask([this]() {
		m_switch_particle = false;
	}, 18000);

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_global_alpha, 0, 1000, 1);
	}, 21300);

	m_taskList.AddTask([this]() {
		m_scene = Boss;
		m_gauss = 8;
		m_dataActor.Add(m_gauss, 6, 23748 - 22000, 1);
		m_dataActor.Add(m_frameAlpha, 0.4, 23748 - 22000, 1);
		//m_dataActor.Add(m_global_alpha, 0.45, 23748 - 22000, 1);
	}, 22000);

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_gauss, 0, 100, 1);
		m_dataActor.Add(m_frameAlpha, 1, 100, 1);
		m_dataActor.Add(m_global_alpha, 1, 100, 1);
	}, 24748);
}

void Background::BackgroundStage3::SkipToBossMode()
{
	m_taskList.Clear();
	m_switch_particle = false;
	m_global_alpha =1;
	m_scene = Boss;
	m_gauss = 0;
	m_frameAlpha = 1;
}

void Background::BackgroundStage3::Particle::Draw(Basic::Video::LowerRenderer::GLTexture* stex) const
{
	const double VERTEX_X = 1800;

	//获取贴图UV框
	const auto& UVRect = stex->GetUVRect(number);
	const double y = UVRect.h * 3000;

	glTexCoord2d(UVRect.x, UVRect.y);							glVertex3d(-VERTEX_X, -y, zDepth);
	glTexCoord2d(UVRect.x, UVRect.y + UVRect.h);				glVertex3d(-VERTEX_X, y, zDepth);
	glTexCoord2d(UVRect.x + UVRect.w, UVRect.y + UVRect.h);		glVertex3d(-VERTEX_X, y, zDepth - UVRect.w);
	glTexCoord2d(UVRect.x + UVRect.w, UVRect.y);				glVertex3d(-VERTEX_X, -y, zDepth - UVRect.w);
}