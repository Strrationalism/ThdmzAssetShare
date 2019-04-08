#include "NuzPrecompiled.h"
#include "WrappedSaveRepActivity.h"
#include "glew.h"
#include "HRollingFadingMenuItem.h"
#include "ActivityEngine.h"
#include "TitleMainMenu.h"
#include "SaveRepActivity.h"
#include "ImageHintActivity.h"
using namespace std;

std::shared_ptr<Basic::Menu::MenuItem> Activities::WrappedSaveRepActivity::createMenuItem(int imgID, const std::function<void()>& action)
{
	return std::make_shared<Menus::HRollingFadingMenuItem>(
		"GameUI/PauseActivity",
		imgID,
		0-25,
		50,
		50,
		action
		);
}

Activities::WrappedSaveRepActivity::WrappedSaveRepActivity(std::unique_ptr<MovieData::MovieData>&& movieData):
	m_movieData {move(movieData)},
	m_menu{
		{
			createMenuItem(10,[this] {
				m_alpha.Tween(0,0.25f,1);
				m_menu.For<Menus::HRollingFadingMenuItem>(
					[](auto& i)
				{
					i.GetAlpha().Tween(0, 0.25f, 1);
				});

				Basic::Sound::SeManager::Play("cardget", 0, 110);


				Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Call(make_shared<Activities::SaveRepActivity>(
					*m_movieData,
					[this] {this->OnDraw(); },
					[this](float t) {this->OnUpdate(t); },
					[this] {
						Basic::Sound::BgmPlayer::Stop();
						m_bk.FadeToBlackScreen();
						m_tl.AddTask([] {
							auto p = make_shared<Title::TitleMainMenu>();
							Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Goto(p);
						},0.5f);
					}
				));
			}),
			createMenuItem(11,[this] {
				m_alpha.Tween(0,0.25f,1);
				m_menu.For<Menus::HRollingFadingMenuItem>(
					[](auto& i)
				{
					i.GetAlpha().Tween(0, 0.25f, 1);
				});
				Basic::Sound::BgmPlayer::Stop();
				Basic::Sound::SeManager::Play("exit",0,110);
				m_bk.FadeToBlackScreen();
				m_tl.AddTask([] {
					auto p = make_shared<Title::TitleMainMenu>();
					Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Goto(p);
				},0.45f);
			})
		}
	},
	m_mat{
		[this] { m_menu.Down(); },
		[this] { m_menu.Up(); }
	}
{
	m_bk.FadeInNow();

	m_menuTitle.UseImage("GameUI/PauseActivity", 9);
	m_menuTitle.SetPos(0, -200);
	m_menuTitle.SetAlpha(0);
	m_menuTitle.UpdateVertex();

	m_alpha = 0;
	
	m_tl.AddTask([this] {
		m_alpha.Tween(1.0f, 1, 1);

		m_menu.For<Menus::HRollingFadingMenuItem>(
			[](auto& i)
		{
			i.GetAlpha().Tween(1.0f,1,1);
		});

	}, 1.0f);

	auto p = [] { Basic::Sound::SeManager::Play("timeout2", 0); };
	m_menu.SetOnDown(p);
	m_menu.SetOnUp(p);
	m_menu.SetEnabled(true);

	m_menu.For<Menus::HRollingFadingMenuItem>(
		[](auto& i)
	{
		i.GetAlpha() = 0;
	});


	m_titleShadow.UseImage("SSLogo", 1);
	m_titleSpec.UseImage("SSLogo", 2);
	m_titleAddtive.UseImage("SSLogo", 3);

	m_titleShadow.SetZoom(0.5f);
	m_titleShadow.SetPos(400, 280);
	m_titleShadow.UpdateVertex();

	m_titleSpec.SetZoom(0.5f);
	m_titleSpec.SetPos(400, 280);
	m_titleSpec.UpdateVertex();

	m_titleAddtive.SetZoom(0.5f);
	m_titleAddtive.SetPos(400, 280);
	m_titleAddtive.UpdateVertex();

}

			void Activities::WrappedSaveRepActivity::OnEntry()
			{
				Basic::Sound::BgmPlayer::Play(525, true);
			}

void Activities::WrappedSaveRepActivity::OnDraw() const
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	
	
	m_bk.Draw([this] {
		m_titleSpec.Draw();
	}, 
		[this] {
		m_titleShadow.Draw(); 
	});

	m_titleAddtive.Draw();

	m_menuTitle.Draw();
	m_menu.Draw();
}

void Activities::WrappedSaveRepActivity::OnUpdate(float deltaTime)
{
	m_tl.Update(deltaTime);
	m_alpha.Update(deltaTime);
	m_bk.Update(deltaTime);

	m_menuTitle.SetAlpha(m_alpha);

	m_menu.Update(deltaTime);

	m_titleSpec.SetAlpha(m_alpha);
	m_titleAddtive.SetAlpha(m_alpha);
	m_titleShadow.SetAlpha(m_alpha);
}

void Activities::WrappedSaveRepActivity::OnEvent(const Basic::Input::Message & e)
{
	if (m_reciveEvent)
	{
		switch (e.type)
		{
		case decltype(e.type)::VecMotion:
			m_mat.OnEvent(e.vec.y);
			break;

		case decltype(e.type)::KeyDown:
			switch (e.key)
			{
			case decltype(e.key)::Shoot:
			case decltype(e.key)::Enter:
				m_reciveEvent = false;
				m_menu.Enter();
				break;
			}
			break;
		}
	}
}
