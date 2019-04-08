#pragma once
#include "Activity.h"
#include "GLTexture.h"
#include "DataActor.h"
#include "TaskList.h"
#include "SecTweenData.h"
#include "SecTaskList.h"
#include "Menu.h"
#include "VecMatcher.h"
#include "MovieData.h"

namespace Activities
{
	class GamePausedActivity :
		public Basic::ActivityEngine::Activity
	{
	public:
		enum class BGM
		{
			None = 0,
			GameOver = 1,
			GameOverWin = 2
		};
	private:
		std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> m_gameScreenShot = std::make_shared<Basic::Video::LowerRenderer::GLTexture>();
		void refreshGameScreenshot();
		void onKey(Basic::Input::Key,bool down);

		void resumeGame();

		void hideComponent();

		Basic::SecTaskList m_taskList;
		Basic::SecTweenData<double> m_blurDepth = 0.0;

		bool m_eventEnabled = false;

		std::optional<Basic::Menu::Menu> m_menu;

		std::shared_ptr<Basic::Menu::MenuItem> createMenuItem(
			int imageID,
			std::function<void()>&& func);

		Basic::Input::VecMatcher m_matcher;

		Basic::SecTweenData<float> m_mainAlpha = 1,m_titleAlpha = 0,m_gameTitleAlpha = 1;
		Basic::Video::Renderer2D::Sprite2D m_title, m_gameTitle, m_gameTitle2;

		std::shared_ptr<Basic::Menu::MenuItem> m_menuItems[6];

		bool m_seEnabled = false;

		BGM m_bgm = BGM::None;

		MovieData::MovieData* m_movieData = nullptr;

	public:
		GamePausedActivity();

		enum class MenuItem : int
		{
			Restart = 0,
			Resume = 1,
			Exit = 2,
			SaveReplay = 3,
			ResumeRep = 4,
			RestartRep = 5
		};



		void BuildMenu(std::vector<MenuItem>&& items,bool seEnabled, BGM bgm,int title);

		void OnEntry() override;
		void OnLeave() override;

		void OnDraw() const override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(const Basic::Input::Message&) override;
		
		void SetMovieData(MovieData::MovieData* data)
		{
			m_movieData = data;
		}
	};
}