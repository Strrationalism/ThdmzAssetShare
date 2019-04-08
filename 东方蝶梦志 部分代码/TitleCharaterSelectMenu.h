#pragma once
#include "TitleMenuStackFrame.h"
#include "TitleMainMenu.h"
#include "Menu.h"
#include "SecTaskList.h"
#include "StageData.h"
#include "BasicChar.h"
#include "VecMatcher.h"

namespace Activities::Title::MenuStack
{
	class TitleCharaterSelectMenu : public TitleMenuStackFrame
	{
	public:
		class CharaterMenuItem : public Basic::Menu::MenuItem
		{
		private:
			Basic::Video::Renderer2D::Sprite2D m_charLight, m_char, m_charName, m_jt[2];
			bool m_selected = false;

			Basic::SecTweenData<float> m_alpha;

			float m_timer = 0;

			Char::BasicChar& m_charaterObject;

			const bool m_playerDataMode;
		public:
			CharaterMenuItem(int charImageBegin, int charNameImage, float cnr, float cng, float cnb, Char::BasicChar& charaterObject,bool playerDataMode = false);

			void Draw() const override;

			void Update(float t) override;

			void OnInitSelect(int offset) override;

			void OnSelectChanged(int offset) override;

			void OnEntry(int offset) override;

			bool IsTopDraw() const override;

			void FadeOut();

			void FadeIn();
		};

		//菜单确认回调
		//第一个参数为"是否为Extra难度"
		//第二个参数为难度，如果为Extra难度，则此项为Lunatic
		//第三个参数为角色
		using SelectCallback = std::function<void(bool, StageLevel, Char::BasicChar&)>;
	private:
		Title::TitleMainMenu* const m_menuManager;
		Basic::Menu::Menu m_menuCharater;
		Basic::Menu::Menu m_menuLevel;

		Basic::Video::Renderer2D::Sprite2D m_spReimu, m_spReimuSpec;

		Basic::SecTaskList m_taskList;

		std::shared_ptr<Basic::Menu::MenuItem> makeLevelMenuItem(int imgID);

		SelectCallback m_callback;

		Basic::Input::VecMatcher m_chMat, m_lvMat;

		std::vector<std::shared_ptr<Basic::Menu::MenuItem>> createLevelMenu(bool extra);
	public:
		TitleCharaterSelectMenu(Title::TitleMainMenu* m,bool showExtra = false);
		void OnEvent(const Basic::Input::Message&) override;

		void OnUpdate(float t) override;
		void OnDrawYSJD() const override;

		void OnEntryOther() override;
		void OnWannaBackToThisShow() override;

		inline void SetCallback(SelectCallback callback)
		{
			m_callback = callback;
		}
	};
}
