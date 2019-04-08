#pragma once
#include "TitleMenuStackFrame.h"
#include "Sprite2D.h"
#include "BasicInput.h"
#include "SecTaskList.h"
#include "TitleMainMenu.h"
#include "SecTweenData.h"

namespace Activities::Title::MenuStack
{
	class TitleJoystickGuide : public TitleMenuStackFrame
	{
	private:
		Basic::Video::Renderer2D::Sprite2D m_label,m_finishedLabel;

		Basic::Video::Renderer2D::Sprite2D m_guide[4];

		Basic::Input::JoystickSetupper m_setter;

		std::vector<int> m_buttons;

		bool m_ignore = true;
		bool m_finished = false;

		Basic::SecTaskList m_tl;

		TitleMainMenu* const m_mainMenu;

		Basic::SecTweenData<float> m_finLabAlpha = 0;
	public:
		TitleJoystickGuide(TitleMainMenu* mainMenu);
		void OnEntryToThis() override;
		void OnDrawYSJD() const override;

		void OnUpdate(float t) override;

		void OnJoystickEvent(int);
	};
}