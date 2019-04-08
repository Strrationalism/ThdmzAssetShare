#pragma once
#include "Activity.h"
#include "MovieData.h"
#include "Sprite2D.h"
#include "Menu.h"
#include "VecMatcher.h"
#include "SecTweenData.h"
#include "SecTaskList.h"
namespace Activities
{
	//这是一个Rep保存界面
	//它应该作为子Activity调用得到
	class SaveRepActivity : public Basic::ActivityEngine::Activity
	{
	private:
		Basic::Menu::Menu m_ime;
		Basic::Input::VecMatcher m_udvm,m_lrvm;

		std::vector<std::shared_ptr<Basic::Menu::MenuItem>> createIme();

		std::vector<std::unique_ptr<std::tuple<Basic::Video::Renderer2D::Sprite2D, wchar_t>>> m_inputBox;

		bool m_reciveEvent = true;

		MovieData::MovieData& m_movie;

		std::wstring getInputBox();
		Basic::SecTweenData<float> m_mainAlpha;
		static std::wstring getDateString();

		Basic::SecTaskList m_taskList;

		const std::function<void()> m_drawExt,m_afterSave;
		const std::function<void(float)> m_updateExt;

		Basic::Video::Renderer2D::Sprite2D m_title,m_hint;
	public:
		SaveRepActivity(MovieData::MovieData& movieData,std::function<void()>&& drawExt,std::function<void(float)>&& updateExt, std::function<void()>&& afterSave);
		void OnUpdate(float deltaTime) override;
		void OnDraw() const override;

		void OnEvent(const Basic::Input::Message&) override;
	};
}