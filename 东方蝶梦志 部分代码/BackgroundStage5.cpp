#include "Precompiled.h"
#include "BackgroundStage5.h"
#include "Game.h"

Background::BackgroundStage5::BackgroundStage5()
{
	//加载纹理
	m_cloud = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("Cloud");
	m_layer = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage3_MainBG_Layer1");
	m_layer3 = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage3_MainBG_Layer3");
	m_birds = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage3_MainBG_Birds");
	m_birds_blur = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage3_MainBG_Birds_Blur");
	m_starSky = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage3_MainBG_StarSky");

	m_moon.UseImage("StageBackground/Stage3_MainBG_Moon", 0);
	m_moon.SetPos(WIDTH / 4, HEIGHT / 8);
	m_moon.UpdateVertex();

}

void Background::BackgroundStage5::Draw() const
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//计算时间系数
	double time = m_timer / 100.0;
	double moonAlpha = (0.1 * sin(time / 10.0) + 0.3)*m_moonAlpha;



	//云层背景
	const double cloudSpeed = 0.05;
	glBindTexture(GL_TEXTURE_2D, *m_cloud);
	glLoadIdentity();
	glBegin(GL_QUADS); {
		glColor4f(10 / 255.0, 14 / 255.0, 142 / 255.0, 0.5 * m_starSkyAlpha);
		glTexCoord2d(time * cloudSpeed, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1 + time * cloudSpeed, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1 + time * cloudSpeed, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(time * cloudSpeed, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);

		glColor4f(10 / 255.0, 14 / 255.0, 142 / 255.0, 0.45 * m_starSkyAlpha);
		glTexCoord2d(time * cloudSpeed * 4.0, 0); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1 + time * cloudSpeed * 4.0, 0); glVertex2d(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1 + time * cloudSpeed * 4.0, 1); glVertex2d(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(time * cloudSpeed * 4.0, 1); glVertex2d(-WIDTH / 2, HEIGHT / 2);
	}glEnd();

	//星空背景
	glColor4f(1, 1, 1, 0.5 * m_starSkyAlpha);
	glBindTexture(GL_TEXTURE_2D, *m_starSky);
	glLoadIdentity();
	glBegin(GL_QUADS); {
		//glColor4f(10 / 255.0, 14 / 255.0, 142 / 255.0, 0.5);
		glTexCoord2d(time * cloudSpeed, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1.35 + time * cloudSpeed, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1.35 + time * cloudSpeed, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(time * cloudSpeed, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);
	}glEnd();

	glColor4f(1, 1, 1, 1);

	//月亮
	m_moon.OldDraw();


	//加载3D环境
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	//glLoadMatrixd(m_projection);
	//glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	glFrustum(-WIDTH / 2, WIDTH / 2, HEIGHT / 2, -HEIGHT / 2, 1.0, 10000.0);

	//晃动摄像机
	glRotated(0.03 * sin(m_timer / 1000.0), 1, 0, 1);
	glTranslated(0, 135 * sin(m_timer / 500.0), 0.5 * sin(m_timer / 750.0));

	//网格层
	const double quadSize = 9.0;
	const double coordSize = 2.0;
	const double zOffset = 6.0;
	const double speed = 0.1;
	const double layer1YOffset = 360;
	const double layer2YOffset = 120;
	glColor4f(1, 1, 1, 1.0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, *m_layer);
	glBegin(GL_QUADS); {
		glTexCoord2d(0, speed * time * coordSize); glVertex3d(-WIDTH*quadSize, HEIGHT*quadSize + layer1YOffset, -1.0);
		glTexCoord2d(0.5 * coordSize, speed * time * coordSize); glVertex3d(-WIDTH*quadSize, -HEIGHT*quadSize + layer1YOffset, -1.0);
		glTexCoord2d(0.5 * coordSize, (1 + speed * time) * coordSize); glVertex3d(WIDTH*quadSize, -HEIGHT*quadSize + layer1YOffset, -6.0);
		glTexCoord2d(0, coordSize * (1 + speed * time)); glVertex3d(WIDTH*quadSize, HEIGHT*quadSize + layer1YOffset, -6.0);

		//glColor4f(1, 1, 1, 0.75);
		glTexCoord2d(0, speed * time * coordSize); glVertex3d(-WIDTH * quadSize, layer2YOffset + HEIGHT * quadSize, -1.0 - zOffset);
		glTexCoord2d(0.5*coordSize, speed * time * coordSize); glVertex3d(-WIDTH * quadSize, layer2YOffset - HEIGHT * quadSize, -1.0 - zOffset);
		glTexCoord2d(0.5*coordSize, coordSize*(1 + speed * time)); glVertex3d(WIDTH * quadSize, layer2YOffset - HEIGHT * quadSize, -6.0 - zOffset);
		glTexCoord2d(0, coordSize*(1 + speed*time)); glVertex3d(WIDTH * quadSize, layer2YOffset + HEIGHT * quadSize, -6.0 - zOffset);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, *m_layer3);
	glBegin(GL_QUADS); {
		//	glColor4f(1, 1, 1, 1.0);
		glTexCoord2d(0, speed*time*coordSize); glVertex3d(-WIDTH * quadSize * 1.2, HEIGHT * quadSize, -1.0 - zOffset * 2);
		glTexCoord2d(0.5*coordSize, speed*time*coordSize); glVertex3d(-WIDTH * quadSize * 1.2, -HEIGHT * quadSize, -1.0 - zOffset * 2);
		glTexCoord2d(0.5*coordSize, coordSize*(1 + speed*time)); glVertex3d(WIDTH * quadSize * 1.2, -HEIGHT * quadSize, -6.0 - zOffset * 2);
		glTexCoord2d(0, coordSize*(1 + speed*time)); glVertex3d(WIDTH * quadSize * 1.2, HEIGHT * quadSize, -6.0 - zOffset * 2);
	}
	glEnd();

	//鸟
	const double birdsYOffset = -3000;
	glBlendFunc(GL_DST_COLOR, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, *m_birds);
	glColor4f(0.75 * m_birdsAlpha, 0.75 * m_birdsAlpha, m_birdsAlpha, 0.5*m_birdsAlpha);
	glBegin(GL_QUADS); {
		glTexCoord2d(0, speed * time * coordSize * 1.75); glVertex3d(-WIDTH * quadSize, HEIGHT * quadSize + birdsYOffset, -1.0 - zOffset);
		glTexCoord2d(0.5*coordSize * 1.75, speed * time * coordSize * 1.75); glVertex3d(-WIDTH * quadSize, -HEIGHT * quadSize + birdsYOffset, -1.0 - zOffset);
		glTexCoord2d(0.5*coordSize * 1.75, coordSize*(1 + speed * time) * 1.75); glVertex3d(WIDTH * quadSize, -HEIGHT * quadSize + birdsYOffset, -6.0 - zOffset);
		glTexCoord2d(0, coordSize*(1 + speed*time) * 1.75); glVertex3d(WIDTH * quadSize, +HEIGHT * quadSize + birdsYOffset, -6.0 - zOffset);

		/*glTexCoord2d(0, speed * time * coordSize * 1.75); glVertex3d(-WIDTH * quadSize, HEIGHT * quadSize + birdsYOffset, -1.0 - zOffset);
		glTexCoord2d(0.5*coordSize * 1.75, speed * time * coordSize * 1.75); glVertex3d(-WIDTH * quadSize, -HEIGHT * quadSize + birdsYOffset, -1.0 - zOffset);
		glTexCoord2d(0.5*coordSize * 1.75, coordSize*(1 + speed * time) * 1.75); glVertex3d(WIDTH * quadSize, -HEIGHT * quadSize + birdsYOffset, -6.0 - zOffset);
		glTexCoord2d(0, coordSize*(1 + speed*time) * 1.75); glVertex3d(WIDTH * quadSize, +HEIGHT * quadSize + birdsYOffset, -6.0 - zOffset);*/
	}glEnd();

	glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, *m_birds_blur);
	glColor4f(0.75 * m_m_birdsAlphaBeats * m_birdsAlpha, 0.75 * m_m_birdsAlphaBeats * m_birdsAlpha, m_m_birdsAlphaBeats * m_birdsAlpha, 0.5*m_birdsAlpha);
	glBegin(GL_QUADS); {
		glTexCoord2d(0, speed * time * coordSize * 1.75); glVertex3d(-WIDTH * quadSize, HEIGHT * quadSize + birdsYOffset, -1.0 - zOffset);
		glTexCoord2d(0.5*coordSize * 1.75, speed * time * coordSize * 1.75); glVertex3d(-WIDTH * quadSize, -HEIGHT * quadSize + birdsYOffset, -1.0 - zOffset);
		glTexCoord2d(0.5*coordSize * 1.75, coordSize*(1 + speed * time) * 1.75); glVertex3d(WIDTH * quadSize, -HEIGHT * quadSize + birdsYOffset, -6.0 - zOffset);
		glTexCoord2d(0, coordSize*(1 + speed*time) * 1.75); glVertex3d(WIDTH * quadSize, +HEIGHT * quadSize + birdsYOffset, -6.0 - zOffset);
	}glEnd();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//卸载3D环境
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	//glDisable(GL_DEPTH_TEST);
}

void Background::BackgroundStage5::Update()
{
	m_timer++;

	double time = m_timer / 100.0;

	m_taskList.Update();
	m_dataActor.Update();

	m_moon.SetAlpha((0.1 * sin(time / 10.0) + 0.3)*m_moonAlpha);

	if (Basic::Sound::BgmPlayer::GetBeatNum() == 0 || Basic::Sound::BgmPlayer::GetBeatNum() == 2)
		m_taskList.AddTask([this]() {
		m_m_birdsAlphaBeats = 0.16;
	}, 35);

	m_m_birdsAlphaBeats *= 0.95;
}

void Background::BackgroundStage5::Init()
{

}

void Background::BackgroundStage5::Reset()
{
	m_timer = 0;
	m_dataActor.Clear();
	m_taskList.Clear();

	m_m_birdsAlphaBeats = 0;

	m_starSkyAlpha = m_birdsAlpha = m_moonAlpha = 0;

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_starSkyAlpha, 1.0, 120, 1);
	}, 500);

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_birdsAlpha, 1.0, 20, 1);
	}, 2011);

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_moonAlpha, 1.0, 20, 1);
	}, 12155);
}

void Background::BackgroundStage5::SkipToBossMode()
{
	m_starSkyAlpha = 1;
	m_birdsAlpha = 1;
	m_moonAlpha = 1;
}
