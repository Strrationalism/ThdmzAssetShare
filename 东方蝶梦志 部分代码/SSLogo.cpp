#include "SSLogo.h"
#include "NuzPrecompiled.h"
#include "SSLogo.h"
#include "Shaders.h"
#include "Game.h"
#include "ActivityEngine.h"
#include "Init.h"

using namespace Activities::Title;
using namespace std;

int SSLogo::initGameSystem()
{
	Basic::Init::SimpleInit();
	Basic::Init::InitMustInMainThread();

	return
#ifdef _DEBUG
	0;
#else
	Basic::Init::CacheTexture(m_frameTaskList, m_textureCache);
#endif
}

SSLogo::SSLogo(std::any& texCache, const std::function<void()>& runGame) :
	m_runGame(runGame), m_textureCache(texCache)
{
	m_sslogo.UseImage("SSLogo");
	m_sslogo.SetPos(0, 0);
	m_sslogo.SetZoom(0.5);
	m_sslogo.UpdateVertex();
}

void SSLogo::OnEntry()
{
	m_alpha.Tween(1, 0.5f, 5);

	m_taskList.AddTask([this] {
		const auto stayFrames = initGameSystem();

		m_frameTaskList.AddTask([this] {
			constexpr auto logoSecs = 0;
			auto run = [this]
			{
				m_useShader = true;
				m_alpha.Tween(0, 0.5f, 1);
				m_taskList.AddTask([this]
				{
					m_runGame();
				}, 0.5f);
			};

			m_taskList.AddTask(run,0.5f);
		}, stayFrames);
	}, 0.6f);
}

void SSLogo::OnDraw() const
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	if (m_alpha > 0 && m_alpha < 1 && m_useShader)
		Basic::Shaders::GaussBlurW((1 - m_alpha) * 64);

	if (m_alpha > 0)
	{
		m_sslogo.Draw();
		glUseProgram(0);
	}
}

void SSLogo::OnUpdate(float deltaTime)
{
	m_alpha.Update(deltaTime);
	m_taskList.Update(deltaTime);
	m_frameTaskList.Update();
	m_sslogo.SetAlpha(m_alpha);
}