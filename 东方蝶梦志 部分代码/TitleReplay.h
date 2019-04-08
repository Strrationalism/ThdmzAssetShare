#pragma once
#include "TitleMenuStackFrame.h"
#include "TitleMainMenu.h"
#include "Menu.h"
#include "VecMatcher.h"
#include "SecTweenData.h"
namespace Activities::Title::MenuStack
{
	class TitleReplay : public TitleMenuStackFrame
	{
	private:
		TitleMainMenu * const m_menuManager;
		Basic::SecTaskList m_tl;
		
		std::vector<std::shared_ptr<Basic::Menu::MenuItem>> createRepList();
		Basic::Menu::Menu m_repList;
		Basic::Input::VecMatcher m_vm;

		Basic::Video::Renderer2D::Sprite2D m_hint;
		Basic::SecTweenData<float> m_hintAlpha;
	public:
		TitleReplay(TitleMainMenu* menuManager);
		void OnEvent(const Basic::Input::Message&) override;

		void OnDrawPost() const override;
		void OnUpdate(float t) override;

		virtual void OnEntryToThis() override;
		virtual void OnEntryOther() override;
	};
}
