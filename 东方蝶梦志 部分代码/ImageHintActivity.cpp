#include "NuzPrecompiled.h"
#include "ImageHintActivity.h"
#include "ActivityEngine.h"
#include "TitleMainMenu.h"

Activities::ImageHintActivity::ImageHintActivity(const std::string & texName, int num,float zoom,std::function<bool()>* func):
	m_func{ func }
{
	m_img.UseImage(texName, num);
	m_img.SetPos(0, 0);
	m_img.SetAlpha(0);
	m_img.SetZoom(zoom);
	m_img.UpdateVertex();
}

void Activities::ImageHintActivity::OnEntry()
{
	m_alpha = 0;
	m_alpha.Tween(1, 0.5f, 1);

	if (m_func == nullptr)
	{
		m_tl.AddTask([this] {
			m_alpha.Tween(0, 0.5f, 1);

			m_tl.AddTask([this] {
				Basic::ActivityEngine::ActivityEngine::GetActivityEngine()
					.Goto(
						std::make_shared<Activities::Title::TitleMainMenu>()
					);
			}, 0.5f);
		}, 5);
	}
}

void Activities::ImageHintActivity::OnUpdate(float t)
{

	m_alpha.Update(t);
	m_img.SetAlpha(m_alpha);
	m_tl.Update(t);

	if (m_func)
	{
		if ((*m_func)())
		{
			m_tl.AddTask([this] {
				m_alpha.Tween(0, 0.5f, 1);

				m_tl.AddTask([this] {
					Basic::ActivityEngine::ActivityEngine::GetActivityEngine()
						.Goto(
							std::make_shared<Activities::Title::TitleMainMenu>()
						);
				}, 0.5f);
			}, 3);

			m_func = nullptr;
		}
	}
}

void Activities::ImageHintActivity::OnDraw() const
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	m_img.Draw();
}
