#pragma once
#include "TitleMainMenu.h"
#include "TitleMenuStackFrame.h"
#include "SecTaskList.h"
#include "TitleCharaterSelectMenu.h"
#include "VecMatcher.h"
#include "TitlePlayerDataSpellCardItem.h"
#include "WRollingPage.h"

namespace Activities::Title::MenuStack
{
	class TitlePlayerData : public TitleMenuStackFrame
	{
	private:
		TitleMainMenu * const m_mainMenu;
		Basic::SecTaskList m_tl;

		Basic::Menu::Menu m_charSelect;

		Basic::Menu::Menu m_lvSelect;
		Basic::Input::VecMatcher m_lvMat;
		std::shared_ptr<Basic::Menu::MenuItem> makeLevelMenuItem(int imgID, const std::function<void()>& func);

		void fadeIn();
		void fadeOut();
		void menuVisible(bool);
		

		//角色名、难度、是否为Extra难度
		std::tuple<int, StageLevel,bool> getSelected() const;

		using SpellPage = Menus::WRollingPage<TitlePlayerDataSpellCardItem>;
		std::optional<Basic::Menu::Menu> m_spellList;
		Basic::Input::VecMatcher m_scMat;

		std::vector<std::shared_ptr<Basic::Menu::MenuItem>> makePages(const std::tuple<int, StageLevel, bool>&);

		Basic::Video::Renderer2D::Sprite2D m_hint;
		Basic::SecTweenData<float> m_alpha;
	public:
		TitlePlayerData(TitleMainMenu* mainMenu);
		void OnEntryToThis() override;
		void OnUpdate(float deltaTime) override;
		void OnDrawYSJD() const override;
		void OnDrawPost() const;
		void OnEvent(const Basic::Input::Message&) override;
		
	};
}