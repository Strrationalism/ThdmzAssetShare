#pragma once
#include "TitleMainMenu.h"
#include "SecTaskList.h"
#include "Menu.h"
#include "VecMatcher.h"
#include "TitleJoystickGuide.h"

namespace Activities::Title::MenuStack
{
	class TitleJoystickConfig : public TitleMenuStackFrame
	{
	private:
		Basic::SecTaskList m_taskList;
		TitleMainMenu* const m_mainMenu;

		bool m_connected = true;

		Basic::Menu::Menu m_menu;

		Basic::Input::VecMatcher m_matcher;

		TitleJoystickGuide* m_guide = nullptr;
	public:
		TitleJoystickConfig(TitleMainMenu* mainMenu);
		void OnBackToThisShow() override;
		void OnEntryToThis() override;
		void OnDrawYSJD() const;
		void OnUpdate(float deltaTime) override;

		void OnEvent(const Basic::Input::Message&) override;

		void Hide();
	};
}
