#include "Precompiled.h"
#include "BackgroundStage6.h"


void Background::BackgroundStage6::drawImageFullScreen(GLuint tex, const float color[4], const float cameraCenter[2], float cameraZoom) const
{
	glColor4fv(color);
	glBindTexture(GL_TEXTURE_2D, tex);

	const float cameraCalcedRadH = cameraZoom / 1280.f * 720.f;
	glBegin(GL_QUADS); {
		glTexCoord2f(cameraCenter[0] - cameraZoom, cameraCenter[1] - cameraCalcedRadH); glVertex2f(-640, -360);
		glTexCoord2f(cameraCenter[0] - cameraZoom, cameraCenter[1] + cameraCalcedRadH); glVertex2f(-640, 360);
		glTexCoord2f(cameraCenter[0] + cameraZoom, cameraCenter[1] + cameraCalcedRadH); glVertex2f(640, 360);
		glTexCoord2f(cameraCenter[0] + cameraZoom, cameraCenter[1] - cameraCalcedRadH); glVertex2f(640, -360);
	}glEnd();
}

void Background::BackgroundStage6::drawImageNegaFullScreen(GLuint tex, const float color[4], const float cameraCenter[2], float cameraZoom) const
{
	glColor4fv(color);
	glBindTexture(GL_TEXTURE_2D, tex);

	const float cameraCalcedRadH = cameraZoom / 1280.f * 720.f;

	glBegin(GL_QUADS); {
		glTexCoord2f(cameraCenter[0] - cameraZoom, cameraCenter[1] - cameraCalcedRadH); glVertex2f(-640, 360);
		glTexCoord2f(cameraCenter[0] - cameraZoom, cameraCenter[1] + cameraCalcedRadH); glVertex2f(-640, -360);
		glTexCoord2f(cameraCenter[0] + cameraZoom, cameraCenter[1] + cameraCalcedRadH); glVertex2f(640, -360);
		glTexCoord2f(cameraCenter[0] + cameraZoom, cameraCenter[1] - cameraCalcedRadH); glVertex2f(640, 360);
	}glEnd();
}

void Background::BackgroundStage6::drawSky() const
{

	{
		//云层
		const double clearNig[3] = { 0,0,0 };
		const double clearDay[3] = { 0.0666666666666667,0.52156862745098,0.615686274509804 };
		double clear[3];
		for (int i = 0; i < 3; ++i)
			clear[i] = Mix(clearNig[i], clearDay[i], SlowMotion(m_dayLerp));

		glClearColor(clear[0], clear[1], clear[2], 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);

		const double colorNig[3] = { 10 / 255.0,14 / 255.0,142 / 255.0 };
		const double colorDay[3] = { 0.0941176470588235,0.741176470588235,0.87843137254902 };
		float color[3];

		for (int i = 0; i < 3; ++i)
			color[i] = Mix(colorNig[i], colorDay[i], SlowMotion(m_dayLerp));

		const float m_starSkyAlpha = 1.0f;
		const double cloudSpeed = 0.00005;
		glBindTexture(GL_TEXTURE_2D, *m_cloud);
		glLoadIdentity();
		glBegin(GL_QUADS); {
			glColor4f(color[0], color[1], color[2], 0.5f);
			glTexCoord2d(m_time * cloudSpeed, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
			glTexCoord2d(1 + m_time * cloudSpeed, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
			glTexCoord2d(1 + m_time * cloudSpeed, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
			glTexCoord2d(m_time * cloudSpeed, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);

			glColor4f(color[0], color[1], color[2], 0.45f);
			glTexCoord2d(m_time * cloudSpeed * 4.0, 0); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
			glTexCoord2d(1 + m_time * cloudSpeed * 4.0, 0); glVertex2d(WIDTH / 2, -HEIGHT / 2);
			glTexCoord2d(1 + m_time * cloudSpeed * 4.0, 1); glVertex2d(WIDTH / 2, HEIGHT / 2);
			glTexCoord2d(m_time * cloudSpeed * 4.0, 1); glVertex2d(-WIDTH / 2, HEIGHT / 2);
		}glEnd();

		const float white[4] = { 1,1,1,m_dayLerp };
		const float cameraCenter[2] = { 0.5,0.5 };
		
	}

	{
		//星空
		const float starAlpha = 1.0 - 0.9*m_dayLerp;
		const float centerX = m_starCenterX;
		const float kiraSpeed = 128.0F;
		float color[4] = { 1,1,1,starAlpha * (0.5 + 0.5f * std::fabs(cos(m_time / kiraSpeed))) };
		float cameraCenter[2] = { centerX,0 };

		drawImageFullScreen(*m_starSkyTexture, color, cameraCenter, 0.45f);

		color[3] = starAlpha*(0.5 + 0.25f * std::fabs(sin(m_time / kiraSpeed)));
		cameraCenter[1] = 500.0f;
		drawImageNegaFullScreen(*m_starSkyTexture, color, cameraCenter, 0.65f);

		color[3] = starAlpha*(0.25 + 0.25f * std::fabs(cos(m_time / kiraSpeed)));
		cameraCenter[1] = 1000.0f;
		drawImageFullScreen(*m_starSkyTexture, color, cameraCenter, 0.85f);

		color[3] = starAlpha*(0.25f * std::fabs(cos(m_time / kiraSpeed)));
		cameraCenter[1] = 2000.0f;
		drawImageNegaFullScreen(*m_starSkyTexture, color, cameraCenter, 0.85f);
	}
}

inline double SunCurve(double t, double n = 0.2, double m = 0.4 )
{
	return t <= m ? Mix(0, 1, SlowMotion(t / m)) : Mix(1, n, SlowMotion((t - m) / (1 - m)));
}

void Background::BackgroundStage6::preDrawCloud() const
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

	glLoadIdentity();


	glFrustum(-640 / 16, 640 / 16, 360 / 16, -360 / 16, 1, 100);
	glTranslatef(320, m_cloudY, 0);



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, *m_cloudsTex);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	Basic::Shaders::Stage6VolumeCloudBefore();
	glBegin(GL_QUADS); {
		for (const auto& p : m_cloudParts)
			p.InsertToDrawOldCall();
	}
	glEnd();
	glUseProgram(0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	int w, h;
	SDL_GetWindowSize(Basic::Video::Window::GetWindow(), &w, &h);
	glBindTexture(GL_TEXTURE_2D, *m_cloudAEColorBuf);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0);
}

void Background::BackgroundStage6::drawCloud() const
{
	Basic::Shaders::Stage6VolumeCloud();

	const double sunAlphaCloud = SunCurve(m_dayLerp, 0.1);	//云给出的颜色贡献
	const double sunAlphaSky = m_dayLerp < 0.4 ? 0 : Clamp(0,1,1.0 - SunCurve(m_dayLerp, 0.1));	//天空给出的颜色贡献

	const float lightColorNight[4] = { 62 / 255.0 , 63 / 255.0,128 / 255.0,1 };
	const float lightColorDay[4] = { 255/255.0 , 254/255.0, 53/255.0,1 };
	const float lightColorDaySky[4] = { 0.937254901960784f,0.988235294117647f,0.996078431372549f,1 };
	float lightColor[4];

	for (int i = 0; i < 4; ++i) 
	{
		lightColor[i] = Mix(lightColorNight[i], lightColorDay[i], sunAlphaCloud);
		lightColor[i] = Mix(lightColor[i], lightColorDaySky[i], sunAlphaSky);
	}

	glColor4fv(lightColor);
	glBindTexture(GL_TEXTURE_2D, *m_cloudAEColorBuf);
	glBegin(GL_QUADS); {
		glTexCoord2f(0, 0); glVertex2f(-640, 360);
		glTexCoord2f(0, 1); glVertex2f(-640, -360);
		glTexCoord2f(1, 1); glVertex2f(640, -360);
		glTexCoord2f(1, 0); glVertex2f(640, 360);
	}glEnd();

	glUseProgram(0);
}

void Background::BackgroundStage6::updateCloud()
{
	auto rnd0 = rand();
	auto rnd1 = rand();
	auto rnd2 = rand();

	if (!UsingBeautifulBG()) return;

	const auto cloudSpeed = m_cloudSpeed;

	//产生云彩
	if (m_time % int(8.0f/ cloudSpeed) == 0)
	{
		CloudParticle newPart;
		newPart.pos[0] = 2080 * 2 * float(rnd0) / RAND_MAX - 2080;
		newPart.pos[1] = 360/8 + 360/8 * float(rnd1) / RAND_MAX;
		newPart.pos[2] = -50;

		const int imgNum = rnd2 % 8;
		newPart.uvRect = m_cloudsTex->GetUVRect(imgNum);

		const float allPer = newPart.uvRect.w + newPart.uvRect.h;
		const float xPer = newPart.uvRect.w / allPer;
		const float yPer = newPart.uvRect.h / allPer;

		newPart.wh[0] = yPer * 600;
		newPart.wh[1] = xPer * 600;

		newPart.alpha = 0.00001f;

		m_cloudParts.push_back(newPart);

		const auto nend = std::remove_if(m_cloudParts.begin(),m_cloudParts.end(),
			[](const auto& p) {
			return p.pos[2] >= -1.0f || p.alpha <= 0;
		});

		m_cloudParts.erase(nend, m_cloudParts.end());

		std::sort(m_cloudParts.begin(), m_cloudParts.end());
	}
	

	for (auto& p : m_cloudParts)
	{
		p.pos[2] += 0.05f * cloudSpeed;
		p.pos[0] -= 1.75f * cloudSpeed;
		p.pos[1] -= 0.001f * cloudSpeed;

		if (p.pos[2] < -10.0f)
			p.alpha = Clamp(0, 1, p.alpha + 0.001f * cloudSpeed);
		else
			p.alpha -= 0.0045f * cloudSpeed;
	}
}

Background::BackgroundStage6::BackgroundStage6()
{
	m_starSkyTexture = LowerRenderer::ImageCacheManager::LoadImageA("StageBackground/Stage3_MainBG_StarSky");
	m_cloud = LowerRenderer::ImageCacheManager::LoadImageA("Cloud");
	m_sun.UseImage("StageBackground/Stage6_Sun");
	m_sunLight.UseImage("StageBackground/Stage6_SunLight");
	m_sunLightCircle.UseImage("StageBackground/Stage6_SunLightCircle");

	if (UsingBeautifulBG())
	{

		m_cloudsTex = LowerRenderer::ImageCacheManager::LoadImageA("StageBackground/Stage6_Clouds");

		m_cloudAEColorBuf = std::make_shared<LowerRenderer::GLTexture>();
	}

}

Background::BackgroundStage6::~BackgroundStage6()
{
}

void Background::BackgroundStage6::Draw() const
{
	if (UsingBeautifulBG())
		preDrawCloud();

	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawSky();

	if (UsingBeautifulBG()) {

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		m_sunLightCircle.OldDraw();
		m_sun.OldDraw();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		drawCloud();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		m_sunLight.OldDraw();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

}


void Background::BackgroundStage6::Update()
{
	m_time++;
	m_dataActor.Update();
	m_taskList.Update();

	{
		//天空

		//星空计算
		m_starCenterX += m_starSpeed;

		if (UsingBeautifulBG()) {
			const double sunAlpha = SunCurve(m_dayLerp);

			//计算太阳和日光光晕
			m_sun.SetPos(240, 520 - m_dayLerp * 620);
			m_sun.UpdateVertex();
			m_sunLight.SetAlpha(sunAlpha);
			m_sunLight.SetPos(240, 520 - m_dayLerp * 620);
			m_sunLight.UpdateVertex();

			m_sunLightCircle.SetAlpha(sunAlpha);
			m_sunLightCircle.SetPos(240, 520 - m_dayLerp * 620);
			m_sunLightCircle.UpdateVertex();
		}
	}

	updateCloud();
}

void Background::BackgroundStage6::Init()
{


}

void Background::BackgroundStage6::Reset()
{
	m_time = 0;

	m_starCenterX = 0;
	m_starSpeed = 0.0075;
	m_dayLerp = 0;
	m_cloudSpeed = 2;
	m_cloudY = -560;

	m_dataActor.Add(m_starSpeed, 0.00051, 100, 1);

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_dayLerp, 1.0, 2000, 1);
	}, 7687);

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_cloudY, 90, 5000, 1);
	}, 2400);
}

void Background::BackgroundStage6::SkipToBossMode()
{
	m_taskList.Clear();

	m_dayLerp = 1;
	m_cloudY = 90;
}

void Background::BackgroundStage6::CloudParticle::InsertToDrawOldCall() const
{
	glColor4f(1, 1, 1, alpha / 4.0);
	glTexCoord2d(uvRect.x, uvRect.y);								glVertex3f(pos[0] - wh[0] / 2, pos[1] - wh[1] / 2,pos[2]);
	glTexCoord2d(uvRect.x + uvRect.w, uvRect.y);					glVertex3f(pos[0] + wh[0] / 2, pos[1] - wh[1] / 2,pos[2]);
	glTexCoord2d(uvRect.x + uvRect.w, uvRect.y + uvRect.h);			glVertex3f(pos[0] + wh[0] / 2, pos[1] + wh[1] / 2,pos[2]);
	glTexCoord2d(uvRect.x, uvRect.y + uvRect.h);					glVertex3f(pos[0] - wh[0] / 2, pos[1] + wh[1] / 2,pos[2]);
}
