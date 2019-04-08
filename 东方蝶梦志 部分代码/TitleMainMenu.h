#pragma once
#include "Activity.h"
#include "SecTweenData.h"
#include "SecTaskList.h"
#include "GLTexture.h"
#include "BackgroundTitle.h"
#include "TitleMenuStackFrame.h"
#include <vector>

namespace Activities::Title
{
	class TitleMainMenu : public Basic::ActivityEngine::Activity
	{
	private:
		Basic::SecTaskList m_taskList;
		
		Title::BackgroundTitle m_background;

		std::vector<std::unique_ptr<TitleMenuStackFrame>> m_menuStack;
		bool m_menuStackEventPass = true;
		int m_backMenu = 0;
		std::optional<std::unique_ptr<TitleMenuStackFrame>> m_prepAdd;
	public:
		TitleMainMenu();
		void OnEntry() override;
		void OnDraw() const override;
		void OnUpdate(float) override;
		void OnEvent(const Basic::Input::Message&) override;

		inline auto& GetBackground()
		{
			return m_background;
		}

		void CallSubMenu(std::unique_ptr<TitleMenuStackFrame>&&);
		void BackThisMenu();
		void WannaBackThisMenu();
	};
}