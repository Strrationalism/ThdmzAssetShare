#include "Precompiled.h"
#include "BackgroundStage2.h"
#include "Game.h"

inline void Background::BackgroundStage2::Particle::Update(bool isGrass)
{
	z -= 0.05 / 2 / 2;
	x -= 4.9010752688172043010752688172043 / 1.78 / 2;
	if (down) {
		alpha -= isGrass ? 0.05/2/2/1.2 : 0.05 / 2 / 2;
	}
	else {
		if (alpha < (5.5)) alpha += 0.025 / 2;
		else down = true;
	}
}

void Background::BackgroundStage2::addTreeToDrawCall(const Particle & p) const
{
	glColor4f(1, 1, 1, p.alpha);
	glTexCoord2d(0, 1); glVertex3f(p.x - 90, HEIGHT / 2 + 900, p.z);
	glTexCoord2d(0, 0.01); glVertex3f(p.x - 90, HEIGHT / 2 - 900, p.z);
	glTexCoord2d(1, 0.01); glVertex3f(p.x + 90, HEIGHT / 2 - 900, p.z);
	glTexCoord2d(1, 1); glVertex3f(p.x + 90, HEIGHT / 2 + 900, p.z);
}

void Background::BackgroundStage2::addGrassToDrawCall(const Particle & p) const
{
	auto  yOffset = 900;
	glColor4f(1, 1, 1, p.alpha);
	glTexCoord2d(0, 1); glVertex3f((p.x - 90), (HEIGHT / 2 + 180)+ yOffset, p.z);
	glTexCoord2d(0, 0.01); glVertex3f((p.x - 90), (HEIGHT / 2 - 180)+ yOffset, p.z);
	glTexCoord2d(1, 0.01); glVertex3f((p.x + 90), (HEIGHT / 2 - 180)+ yOffset, p.z);
	glTexCoord2d(1, 1); glVertex3f((p.x + 90), (HEIGHT / 2 + 180) + yOffset, p.z);
}

void Background::BackgroundStage2::addBoxToDrawCall(const Particle & p) const
{
	const auto yOffset = 900;
	const auto height = -30;
	const auto zOffset = 0.5;

	//前部
	glColor4f(0.5, 0.5, 0.5, p.alpha);
	glTexCoord2d(0, 0.1); glVertex3f((p.x - 30), (HEIGHT / 2 + 80) + yOffset, p.z + zOffset);
	glTexCoord2d(0, 0.01); glVertex3f((p.x - 30), (HEIGHT / 2 - height) + yOffset, p.z + zOffset);
	glTexCoord2d(1, 0.01); glVertex3f((p.x + 30), (HEIGHT / 2 - height) + yOffset, p.z);
	glTexCoord2d(1, 0.1); glVertex3f((p.x + 30), (HEIGHT / 2 + 80) + yOffset, p.z);


	//顶部
	glColor4f(1, 1, 1, p.alpha);
	//顶部面板长度
	const auto topLength = -0.25;
	//顶部面板远端X修正
	const auto farXFix = -30;
	glTexCoord2d(0, 1); glVertex3f((p.x - 30), (HEIGHT / 2 - height) + yOffset, p.z + zOffset);
	glTexCoord2d(1, 1); glVertex3f((p.x + 30), (HEIGHT / 2 - height) + yOffset, p.z);
	glTexCoord2d(1, 0); glVertex3f((p.x + 30) + farXFix, (HEIGHT / 2 - height) + yOffset, topLength + p.z);
	glTexCoord2d(0, 0); glVertex3f((p.x - 30) + farXFix, (HEIGHT / 2 - height) + yOffset, topLength + p.z + zOffset);
	
	//左部
	glColor4f(0.75, 0.75, 0.75, p.alpha);
	glTexCoord2d(0, 0); glVertex3f((p.x - 30), (HEIGHT / 2 + 80) + yOffset, p.z + zOffset);
	glTexCoord2d(0, 0.1); glVertex3f((p.x - 30), (HEIGHT / 2 - height) + yOffset, p.z + zOffset);
	glTexCoord2d(1, 0.1); glVertex3f((p.x - 30) + farXFix, (HEIGHT / 2 - height) + yOffset, topLength+p.z + zOffset);
	glTexCoord2d(1, 0); glVertex3f((p.x - 30) + farXFix, (HEIGHT / 2 + 80) + yOffset, topLength+p.z + zOffset);
}

Background::BackgroundStage2::BackgroundStage2()
{

	//创建3D摄像机

	//创建ModelView矩阵
	glScaled(8.0f, 1, 1);
	glGetDoublev(GL_MODELVIEW_MATRIX, m_modelView);

	//加载纹理
	m_cloud = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("Cloud");
	m_kabe = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage2_MainBG_Kabe");
	m_dimian = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage(UsingBeautifulBG() ? "StageBackground/Stage2_MainBG_Dimian" : "StageBackground/Stage2_MainBG_Dimian_LOW");

	m_noise64 = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("Noise64");

	//建立FB
	if (UsingBeautifulBG()) {
		int w, h;
		SDL_GetWindowSize(Basic::Video::Window::GetWindow(), &w, &h);

		m_grassAndMoonFB.emplace();
		m_box = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage2_Box_Top");
		m_tree = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage2_MainBG_Banboo");
		m_tree2 = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage2_MainBG_Banboo2");
		m_grass = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage2Grass");
	}
}

void Background::BackgroundStage2::Draw() const
{
	double xOffset = m_timer * 0.0025;

	glFogf(GL_FOG_START, m_fogStart);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *m_noise64);
	Basic::Shaders::GrassAndMoon(xOffset);
	glLoadIdentity();
	glBegin(GL_QUADS);
	glTexCoord2d(0, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
	glTexCoord2d(1, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
	glTexCoord2d(1, 0.25); glVertex2d(WIDTH / 2, HEIGHT / 4);
	glTexCoord2d(0, 0.25); glVertex2d(-WIDTH / 2, HEIGHT / 4);
	glEnd();

	glUseProgram(0);

	if (UsingBeautifulBG() && m_godRayAlpha > 0) {
		int w, h;
		SDL_GetWindowSize(Basic::Video::Window::GetWindow(), &w, &h);
		glBindTexture(GL_TEXTURE_2D, *m_grassAndMoonFB);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0);
	}
	glEnable(GL_FOG);

	glDisable(GL_TEXTURE_2D);

	glDisable(GL_BLEND);

	glLoadMatrixd(m_modelView);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	//摄像机
	glLoadIdentity();
	glFrustum(-WIDTH / 2, WIDTH / 2, HEIGHT / 2, -HEIGHT / 2, 1, 100000);
	glRotated(6 * sin(m_timer / 400.0), 0.00025, 0.00015, -1);


	//墙面和地面的雾化修正
	glFogf(GL_FOG_END, m_fogEnd);

	//墙面
	const double kabeSpeedMul = 2.65 * 1.4;
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, *m_kabe);
	glColor4f(0.9f, 0.9f, 0.9f, 1);
	glBegin(GL_QUADS); {
		glTexCoord2d(14 + xOffset * kabeSpeedMul, 1); glVertex3f(WIDTH / 12, HEIGHT / 2 + 150, -1.0);
		glTexCoord2d(14 + xOffset * kabeSpeedMul, 0); glVertex3f(WIDTH / 12, -HEIGHT / 2 + 150, -1.0);
		glTexCoord2d(xOffset * kabeSpeedMul, 0); glVertex3f(WIDTH / 12 - 800, -HEIGHT / 2 + 150, -8.5);
		glTexCoord2d(xOffset * kabeSpeedMul, 1); glVertex3f(WIDTH / 12 - 800, HEIGHT / 2 + 150, -8.5);
	}glEnd();

	//地面
	const double dimianSpeedMul = -0.72 * 1.4;
	glBindTexture(GL_TEXTURE_2D, *m_dimian);
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUADS); {
		glTexCoord2d(2.0, xOffset * dimianSpeedMul); glVertex3f(WIDTH / 12, HEIGHT / 2 + 150, -1.0);
		glTexCoord2d(2.0, 4 + xOffset * dimianSpeedMul); glVertex3f(WIDTH / 12 - 800, HEIGHT / 2 + 150, -8.5);
		glTexCoord2d(0, 4 + xOffset * dimianSpeedMul); glVertex3f(WIDTH / 12 - 1200, HEIGHT / 2 + 150, -8.5);
		glTexCoord2d(0, xOffset * dimianSpeedMul); glVertex3f(WIDTH / 12 - 400, HEIGHT / 2 + 150, -1.0);
	}glEnd();
	glEnable(GL_BLEND);

	if (UsingBeautifulBG()) {
		glFogf(GL_FOG_END, m_fogEnd * 2);
		glBindTexture(GL_TEXTURE_2D, *m_box);
		glBegin(GL_QUADS); {
			for (auto& p : m_boxes)
				addBoxToDrawCall(p);
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, *m_grass);
		glBegin(GL_QUADS); {
			for (auto& p : m_grasses)
				addGrassToDrawCall(p);
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, *m_tree);
		glBegin(GL_QUADS); {
			for (auto& p : m_trees)
				addTreeToDrawCall(p);
		}
		glEnd();

		glBindTexture(GL_TEXTURE_2D, *m_tree2);
		glBegin(GL_QUADS); {
			for (auto& p : m_tree2s)
				addTreeToDrawCall(p);
		}
		glEnd();

	}


	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_FOG);
	glLoadIdentity();

	//圣光
	//glClear(GL_COLOR_BUFFER_BIT);
	if (UsingBeautifulBG() && m_godRayAlpha > 0.01) {
		Basic::Shaders::GodRay(150.0f, 0.65f, -404, -193);

		glColor4f(1, 1, 1, m_godRayAlpha);



		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBindTexture(GL_TEXTURE_2D, *m_grassAndMoonFB);
		glBegin(GL_QUADS);
		glTexCoord2d(0, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(0, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);
		glEnd();
		glUseProgram(0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	glColor4f(1, 1, 1, m_texFogAlpha);
	glBindTexture(GL_TEXTURE_2D, *m_cloud);
	glBegin(GL_QUADS); {
		glTexCoord2d(1 + xOffset, 1); glVertex2f(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(xOffset, 1); glVertex2f(-WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(xOffset, 0); glVertex2f(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1 + xOffset, 0); glVertex2f(WIDTH / 2, -HEIGHT / 2);

		glTexCoord2d(1 - xOffset / 2, 1); glVertex2f(-WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(-xOffset / 2, 1); glVertex2f(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(-xOffset / 2, 0); glVertex2f(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1 - xOffset / 2, 0); glVertex2f(-WIDTH / 2, -HEIGHT / 2);
	}
	glEnd();
}

void Background::BackgroundStage2::Update()
{
	m_timer++;
	m_taskList.Update();
	m_dataActor.Update();



	int rnd = rand();
	double dRnd = Rand();
	double dRnd2 = Rand();

	m_partTimer++;

	if (UsingBeautifulBG()) {
		auto time = m_partTimer;
		if (time % (4*(time < 17302 ? 24 : 72)) == 0) {
			double x = WIDTH / 12 + 285 - dRnd * 300;
			if (x >= WIDTH / 12 + 100 - 300)
				m_trees.push_back({
				x,
				0,
				-0.7,
				0,
				false
			});

			x = WIDTH / 12 + 285 - dRnd2 * 300;
			if (x >= WIDTH / 12 + 100 - 300)
				m_tree2s.push_back({
				x,
				0,
				-0.7,
				0,
				false
			});
		}

		if (time % 5 == 0)
		{
			double x = WIDTH / 12 + 285 - (dRnd + dRnd2) / 2 * 400;
			if (x >= WIDTH / 12 + 100 - 400)
			{
				m_grasses.push_back({
					x,
					0,
					-0.7,
					0,
					false
					});
			}
		}

		if (time % 30 == 0)
		{
			double x = WIDTH / 12 + 315;
			m_boxes.push_back({
					x,
					0,
					-0.7,
					0,
					false
			});
			
		}

		for (auto& p : m_trees)
			p.Update();

		for (auto& p : m_tree2s)
			p.Update();

		for (auto& p : m_grasses)
			p.Update(true);

		for (auto& p : m_boxes)
			p.Update(true);

		m_trees.erase(std::remove_if(m_trees.begin(), m_trees.end(), [](auto& p) {
			return p.alpha < 0;
		}), m_trees.end());

		m_tree2s.erase(std::remove_if(m_tree2s.begin(), m_tree2s.end(), [](auto& p) {
			return p.alpha < 0;
		}), m_tree2s.end());

		m_grasses.erase(std::remove_if(m_grasses.begin(), m_grasses.end(), [](auto& p) {
			return p.alpha < 0;
		}), m_grasses.end());

		m_boxes.erase(std::remove_if(m_boxes.begin(), m_boxes.end(), [](auto& p) {
			return p.alpha < 0;
		}), m_boxes.end());
	}
}

void Background::BackgroundStage2::Init()
{

}

void Background::BackgroundStage2::Reset()
{
	m_timer = 0;

	m_dataActor.Clear();
	m_taskList.Clear();

	m_partTimer = 0;
	for (int i = 0; i < 900; ++i)
		Update();

	//选定雾的类型
	glFogi(GL_FOG_MODE, GL_LINEAR);
	//确定雾的渲染方式
	glHint(GL_FOG_HINT, GL_NICEST);

	float fogColor[4];
	fogColor[0] = 0;
	fogColor[1] = 0.05f;
	fogColor[2] = 0.03f;
	fogColor[3] = 1.0f;
	glFogfv(GL_FOG_COLOR, fogColor);

	m_fogStart = -7;
	m_fogEnd = 8;
	m_texFogAlpha = 0.05;
	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_fogStart, -1, 9174 - 2635, 4);
		m_dataActor.Add(m_texFogAlpha, 0.25, 9174 - 2635, 4);
	}, 2635);
	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_fogEnd, 3.75, 17031 - 14359, 4);
		m_dataActor.Add(m_texFogAlpha, 0.55, 9174 - 2635, 4);
	}, 14359);

	if (UsingBeautifulBG()) {
		m_taskList.AddTask([this]() {
			m_dataActor.Add(m_godRayAlpha, 0.0, 1000, 0);
		}, 16320);
		m_godRayAlpha = 1.0;
	}
}