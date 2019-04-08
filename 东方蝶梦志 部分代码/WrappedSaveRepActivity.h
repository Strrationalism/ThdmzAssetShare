#pragma once
#include "Activity.h"
#include "MovieData.h"
#include "BackgroundTitle.h"
#include "SecTweenData.h"
#include "SecTaskList.h"
#include "Menu.h"
#include "VecMatcher.h"
namespace Activities
{
	class WrappedSaveRepActivity : public Basic::ActivityEngine::Activity
	{
	private:
		std::unique_ptr<MovieData::MovieData> m_movieData;

		Title::BackgroundTitle m_bk;

		Basic::Video::Renderer2D::Sprite2D m_menuTitle, m_titleShadow, m_titleSpec, m_titleAddtive;

		Basic::SecTweenData<float> m_alpha;
		Basic::SecTaskList m_tl;

		Basic::Menu::Menu m_menu;
		Basic::Input::VecMatcher m_mat;
		bool m_reciveEvent = true;

		std::shared_ptr<Basic::Menu::MenuItem> createMenuItem(int imgID, const std::function<void()>& action);
	public:
		WrappedSaveRepActivity(std::unique_ptr<MovieData::MovieData>&& movieData);
		void OnEntry() override;
		void OnDraw() const override;
		void OnUpdate(float deltaTime) override;

		void OnEvent(const Basic::Input::Message& e) override;
	};
}