#pragma once
#include "TitleMainMenu.h"
#include "SecTaskList.h"
#include "TitleJoystickConfig.h"
#include "SecTweenData.h"

namespace Activities::Title::MenuStack
{
	class TitleJoystickNotConnectedHint : public TitleMenuStackFrame
	{
	private:

		Basic::Video::Renderer2D::Sprite2D m_hintIcon, m_hint;
		float m_timer = 0;
		Basic::SecTweenData<float> m_hintAlpha = 0;

		Basic::SecTaskList m_taskList;
		TitleMainMenu* const m_mainMenu;

		bool m_joyStickConnected = false;

		void onConnected();
		void onDisconnected();
	public:
		TitleJoystickNotConnectedHint(TitleMainMenu* mainMenu);
		void OnWannaBackToThisShow() override;
		void OnBackToThisShow() override;
		void OnEntryToThis() override;
		void OnDrawYSJD() const;
		void OnUpdate(float deltaTime) override;

		void OnEvent(const Basic::Input::Message&) override;
	};
}
