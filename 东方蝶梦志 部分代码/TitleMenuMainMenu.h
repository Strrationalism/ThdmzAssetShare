#pragma once
#include "TitleMenuStackFrame.h"
#include "Sprite2D.h"
#include "SecTweenData.h"
#include "Menu.h"
#include "SecTaskList.h"
#include "TitleMainMenu.h"
#include "VecMatcher.h"
#include "Sprite2DWStreamLayout.h"

namespace Activities::Title::MenuStack
{
	class TitleMenuMainMenu : public TitleMenuStackFrame
	{
	private:
		Basic::Video::Renderer2D::Sprite2D m_titleShadow, m_titleSpec, m_titleAddtive, m_ssLogo;
		Basic::SecTweenData<float> m_titleZoom, m_titleX, m_titleY;

		Basic::Video::Renderer2D::Sprite2D m_awanljanCharLight, m_awanljanChar;
		Basic::SecTweenData<float> m_awanljanAlpha = 1;

		Basic::Menu::Menu m_menu;

		std::shared_ptr<Basic::Menu::MenuItem> makeMainMenu(int imgID, const std::function<void()>& func);

		Basic::SecTaskList m_taskList;

		Basic::SecTweenData<float> m_logoAlpha = 0;

		TitleMainMenu* const m_menuManager;

		float m_orgMenuX = 0;

		Basic::Input::VecMatcher m_vmacther;

		Basic::Video::Renderer2D::Sprite2DWStreamLayout m_version;
	public:
		TitleMenuMainMenu(TitleMainMenu*);
		void OnEntryToThis() override;				//进入时显示
		void OnEntryOther() override;				//进入其他子菜单时隐藏
		void OnWannaBackToThisShow() override;			//从子菜单返回到本菜单时

		void OnDraw() const override;
		void OnDrawYSJD() const override;
		void OnDrawPost() const override;

		void OnUpdate(float deltaTime) override;

		void OnEvent(const Basic::Input::Message&) override;
	};
}
