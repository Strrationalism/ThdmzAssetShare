#include "Precompiled.h"
#include "BackgroundStageEx.h"
#include "Game.h"
#include "Matrix.h"
using namespace Basic::Video::LowerRenderer;

Background::BackgroundStageEx::BackgroundStageEx()
{
	m_jb = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/StageEx_JB");
	m_bk = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/StageEx_BK");
	{
		m_isNvidiaTrust = false;
		//检查是否为N卡
		const std::string vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
		if (vendor.find("NVIDIA") != std::string::npos)
			m_isNvidiaTrust = true;
	}

	if (UsingBeautifulBG())
	{
		m_snowTex = ImageCacheManager::LoadImageA("StageBackground/StageEx_Snow");
		m_shan = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage1_shan2");
		m_shui = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage1_Water");



		m_pencilFBO = std::make_shared<Basic::Video::LowerRenderer::GLTexture>();
	}

	else
	{
		m_shan = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("StageBackground/Stage1_shan2_LOW");
	}
}

Background::BackgroundStageEx::~BackgroundStageEx()
{
}

void Background::BackgroundStageEx::drawImageFullScreen(GLuint tex, const float color[4], const float cameraCenter[2], float cameraZoom) const
{
	glColor4fv(color);
	glBindTexture(GL_TEXTURE_2D, tex);

	const float cameraCalcedRadH = cameraZoom;
	glBegin(GL_QUADS); {
		glTexCoord2f(cameraCenter[0] - cameraZoom, cameraCenter[1] - cameraCalcedRadH); glVertex2f(-640, -360);
		glTexCoord2f(cameraCenter[0] - cameraZoom, cameraCenter[1] + cameraCalcedRadH); glVertex2f(-640, 360);
		glTexCoord2f(cameraCenter[0] + cameraZoom, cameraCenter[1] + cameraCalcedRadH); glVertex2f(640, 360);
		glTexCoord2f(cameraCenter[0] + cameraZoom, cameraCenter[1] - cameraCalcedRadH); glVertex2f(640, -360);
	}glEnd();
}

void Background::BackgroundStageEx::drawSnow() const
{
	//准备3D摄像机
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	

	glScaled(4, 4, 1);
	glFrustum(-640,640,320,-320,0.1,1.0);
	

	//Draw Snow Particles
	glBindTexture(GL_TEXTURE_2D, *m_snowTex);
	glBegin(GL_QUADS);
	for (const auto& s : m_parts)
		s.InsertToDrawCall();
	for (const auto& s : m_partShadow)
		s.InsertToDrawCall();
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void Background::BackgroundStageEx::Draw() const
{
	if (UsingBeautifulBG())
	{
		//准备后处理
		//非N卡做法
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	const float white[] = { 1,1,1,1 };
	const float cam[] = { 0.5F,0.5F };
	drawImageFullScreen(*m_bk, white, cam, 0.5);

	const double shanSpeed = 0.0005f;
	const float shanOffset = shanSpeed * m_time;

	Basic::Shaders::StageExBW();
	glColor4f(1, 1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, *m_shan);
	const double moreHigh = -20;

	const double moutainDepth = -8.0f;
	glBegin(GL_QUADS); {
		glTexCoord2d(shanOffset, 0.0012f); glVertex2d(-WIDTH / 2, -100 + moreHigh);
		glTexCoord2d(0.58394f + shanOffset, 0.0012f); glVertex2d(WIDTH / 2, -100 + moreHigh);
		glTexCoord2d(0.58394f + shanOffset, 1); glVertex2d(WIDTH / 2, 130 + moreHigh);
		glTexCoord2d(shanOffset, 1); glVertex2d(-WIDTH / 2, 130 + moreHigh);
	}
	glEnd();
	glUseProgram(0);

	if (UsingBeautifulBG()) {
		//用于后期的反射效果
		
		Basic::Shaders::StageExRelfection(-m_time / 50.0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, *m_shui);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *m_shan);
		glBegin(GL_QUADS); {
			glTexCoord2d(shanOffset, -1.0); glVertex3d(-640, 390, 0);
			glTexCoord2d(1 + shanOffset, -1.0); glVertex3d(640, 390, 0);
			glTexCoord2d(1 + shanOffset, 0); glVertex3d(640, 110, -1);
			glTexCoord2d(shanOffset, 0); glVertex3d(-640, 110, -1);
		}
		glEnd();

		//执行后处理
		//非N卡做法
		glBindTexture(GL_TEXTURE_2D, *m_pencilFBO);
		int w, h;
		Game::GetGame().GetScreenSize(w, h);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, w, h, 0);

		glUseProgram(0);
		Basic::Shaders::StageExLineDraw();
		glBegin(GL_QUADS); {
			glTexCoord2d(0, 0); glVertex2d(-640, 360);
			glTexCoord2d(0, 1); glVertex2d(-640, -360);
			glTexCoord2d(1, 1); glVertex2d(640, -360);
			glTexCoord2d(1, 0); glVertex2d(640, 360);
		}
		glEnd();

		glUseProgram(0);
	}

	const float cam2[] = { 0.5F,0.4F };
	drawImageFullScreen(*m_jb, white, cam2, 0.5);

	if (UsingBeautifulBG())
		drawSnow();
}

void Background::BackgroundStageEx::Update()
{
	m_time++;
	m_dataActor.Update();


	if (m_time % 15 == 0)
	{
		auto rnd1 = rand() / static_cast<float>(RAND_MAX);
		auto rnd2 = rand() / static_cast<float>(RAND_MAX);
		auto rnd3 = rand() / static_cast<float>(RAND_MAX);
		auto rnd4 = rand() / static_cast<float>(RAND_MAX);

		if (UsingBeautifulBG()) {
			auto s = SnowParticle
			{
				{ 100 * rnd2 + 2800,650 * rnd1 - 470,-1.1f + 0.8F * (rnd3 - 0.5F) },
				1.0f,1.0f,1.0f,1.0F,
				{ 30,30 },
				rnd4 * 3 + 7,
				rnd1 * 2 - 1,
				rnd3 + rnd2,
				m_snowTex->GetUVRect(rnd4 * 1.999f)
			};

			auto nend = std::remove_if(m_parts.begin(), m_parts.end(), [](auto& p) { return p.pos[0] < -1280; });
			m_parts.erase(nend, m_parts.end());

			m_parts.push_back(s);
		}
	}
	if (UsingBeautifulBG()) {
		static const int UPDATE_FRAMES = 1;
		if (m_time % UPDATE_FRAMES == 0)
		{
			for (auto& p : m_parts)
				p.Update();

			for (auto& p : m_partShadow)
				p.ShadowUpdate();
		}

		if (m_time % 5 == 0)
		{
			auto nend = std::remove_if(m_partShadow.begin(), m_partShadow.end(), [](auto& p) { return p.a <= 0.01f; });
			m_partShadow.erase(nend, m_partShadow.end());

			for (const auto& p : m_parts)
				m_partShadow.push_back(p);
		}
	}
	//Log("Parts:%u,Shadow:%u", m_parts.size(),m_partShadow.size());

}

void Background::BackgroundStageEx::Init()
{

}

void Background::BackgroundStageEx::Reset()
{
	m_parts.clear();
	m_time = 0;
}

void Background::BackgroundStageEx::SnowParticle::Update()
{
	rotate += 0.2F * rotateSpeed;
	pos[0] -= speed;
	pos[1] += 0.45f;
	pos[2] += 0.002f;
	frames++;

	a = 1;
	if (pos[2] > -0.2) {
		auto zClamp = Clamp(pos[2] ,-0.1, -0.2);
		a *= ((-zClamp) - 0.1) * 10;
	}
	else {
		auto zClamp = Clamp(pos[2], -0.9, -1.1);
		a = (((-zClamp) - 0.9) * 5);
		a = 1.0 - a;
	}

	if (frames <= 1000)
	{
		a *= frames / 1000.0f;
	}
}

void Background::BackgroundStageEx::SnowParticle::ShadowUpdate()
{
	a = a * 0.9f;
}


void Background::BackgroundStageEx::SnowParticle::InsertToDrawCall() const
{
	glColor4f(r, g, b, a);

	double p[8]=
	{
		- wh[0] / 2, - wh[1] / 2,
		+ wh[0] / 2, - wh[1] / 2,
		+ wh[0] / 2, + wh[1] / 2,
		- wh[0] / 2, + wh[1] / 2
	};

	for (int i = 0; i < 4; ++i) {
		Basic::Math::Matrix::RotateVec2_2D(p[2*i], p[2*i + 1], rotate, p + 2*i, p + 2*i + 1);
		p[2*i] += pos[0];
		p[2*i + 1] += pos[1];
	}

	glTexCoord2d(uvRect.x, uvRect.y);								glVertex3f(p[0], p[1], pos[2]);
	glTexCoord2d(uvRect.x + uvRect.w, uvRect.y);					glVertex3f(p[2], p[3], pos[2]);
	glTexCoord2d(uvRect.x + uvRect.w, uvRect.y + uvRect.h);			glVertex3f(p[4], p[5], pos[2]);
	glTexCoord2d(uvRect.x, uvRect.y + uvRect.h);					glVertex3f(p[6], p[7], pos[2]);
}
