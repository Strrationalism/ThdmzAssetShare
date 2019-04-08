#include "NuzPrecompiled.h"
#include "BackgroundTitle.h"

void Activities::Title::BackgroundTitle::drawFadingBg(float leftUV, float topUV, float rightUV, float bottomUV,float alpha) const
{
	glBegin(GL_QUADS); 
	{
		glColor4f(0.5f, 0.5f, 0.5f, 0);
		glTexCoord2f(leftUV, topUV);
		glVertex2f(-WIDTH / 2, -HEIGHT / 2);

		glColor4f(1, 1, 1, 1.25F*alpha);
		glTexCoord2f(leftUV, bottomUV);
		glVertex2f(-WIDTH / 2, HEIGHT / 2);

		glColor4f(1, 1, 1, 1.25F*alpha);
		glTexCoord2f(rightUV, bottomUV);
		glVertex2f(WIDTH / 2, HEIGHT / 2);

		glColor4f(0.5f, 0.5f, 0.5f,0);
		glTexCoord2f(rightUV, topUV);
		glVertex2f(WIDTH / 2, -HEIGHT / 2);
	}
	glEnd();
}

void Activities::Title::BackgroundTitle::drawFullBgUsingGLColor(float leftUV, float topUV, float rightUV, float bottomUV) const
{
	glBegin(GL_QUADS);
	{
		glTexCoord2f(leftUV, topUV);
		glVertex2f(-WIDTH / 2, -HEIGHT / 2);

		glTexCoord2f(leftUV, bottomUV);
		glVertex2f(-WIDTH / 2, HEIGHT / 2);

		glTexCoord2f(rightUV, bottomUV);
		glVertex2f(WIDTH / 2, HEIGHT / 2);

		glTexCoord2f(rightUV, topUV);
		glVertex2f(WIDTH / 2, -HEIGHT / 2);
	}
	glEnd();
}

void Activities::Title::BackgroundTitle::reset()
{
	m_bkCloudAlpha = 0;
	m_secTasks.AddTask([this] {
		m_bkCloudAlpha.Tween(1.0, 3.0f, 1);
	}, 2.9);

	m_starSkyAlpha = 0;
	m_starSkyAlpha.Tween(0.5, 2, 1);
}

void Activities::Title::BackgroundTitle::drawSpecBlendLayer(const std::function<void()>& drawcallsInYSJD) const
{
	drawcallsInYSJD(); 
}

Activities::Title::BackgroundTitle::BackgroundTitle()
{
	m_cloudTexture = Basic::Video::LowerRenderer::ImageCacheManager::LoadImageA("CloudWB");

	m_noBluredStarSky = Basic::Video::LowerRenderer::ImageCacheManager::LoadImageA("StageBackground/Stage3_MainBG_StarSky");
	m_bluredStarSky1 = Basic::Video::LowerRenderer::ImageCacheManager::LoadImageA("StarSkyBlur1");
	m_bluredStarSky2 = Basic::Video::LowerRenderer::ImageCacheManager::LoadImageA("StarSkyBlur2");
	m_gridTex = Basic::Video::LowerRenderer::ImageCacheManager::LoadImageA("TitleGrid");
	m_starWind = Basic::Video::LowerRenderer::ImageCacheManager::LoadImageA("StarWind");
	
	m_mainScreen = std::make_shared<Basic::Video::LowerRenderer::GLTexture>();
	m_specScreen = std::make_shared<Basic::Video::LowerRenderer::GLTexture>();

	reset();
}

void Activities::Title::BackgroundTitle::Update(float t)
{
	m_time += t;

	m_secTasks.Update(t);
	m_bkCloudAlpha.Update(t);
	m_starSkyAlpha.Update(t);
}

void Activities::Title::BackgroundTitle::FadeInNow()
{
	m_bkCloudAlpha = 0;
	m_starSkyAlpha = 0;

	m_secTasks.Clear();

	m_bkCloudAlpha.Tween(1, 1.5f,1);
	m_starSkyAlpha.Tween(1, 1.5f, 1);
}

void Activities::Title::BackgroundTitle::FadeToBlackScreen()
{
	m_bkCloudAlpha.Tween(0, 0.5f, 1);
	m_starSkyAlpha.Tween(0, 0.5f, 1);
}
