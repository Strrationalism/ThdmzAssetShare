#pragma once
#include "Conversation.h"
#include "Sprite2D.h"
#include "TaskList.h"
#include "GameObjectWithDmslControl.h"
#include "Dmsl.h"
#include "ConversationDelayer.h"

namespace Conversation {
	//文字
	class ConversationWord {
	private:
		bool m_killed = false;

		Basic::Video::Renderer2D::Sprite2D m_sprite;
		std::optional<Dmsl::DmslUnit> m_program;

		double m_alpha = 0;
		int m_afterFrameTime;
		Basic::DataActor m_dataActor;

		static std::unique_ptr<Dmsl::DmslVirtualMachine> m_textEffectProgram;
		static int m_locX, m_locY, m_locR, m_locG, m_locB, m_locAfterFrameTime;
	public:
		static void Init();

		inline void Draw() const {
			m_sprite.Draw();
		}
		inline void EndEffect() {
			m_dataActor.Clear();
			m_afterFrameTime = 0;
			m_alpha = 1;
		}
		void Update();
		void Create(wchar_t word, double x, double y, double r, double g, double b, int program, int afterFrameTime);

		inline bool Died() { return m_killed; }
		void Kill();
	};

	//立绘
	class ConversationStandImage :public GameObject {
	private:
		Basic::DataActor m_dataActor;

		double m_actX, m_unactX, m_hideX;
		enum {
			HIDDEN,
			ACT,
			UNACT
		}m_state = HIDDEN;

		std::string m_textureName;

		std::array<Basic::Video::Renderer2D::Sprite2D, 2> m_sp;
		double m_y, m_x, m_alpha[2] = { 0,0 };
		int m_usingSp = 0;

		inline int getAnotherSp() {
			if (m_usingSp) return 0;
			else return 1;
		}

		static std::map<std::string, int> m_ldData;
	public:
		static void Init();
		void SetXPos(double actX, double unactX, double hideX);	//设置激活状态下的X位置、未激活状态下的X、隐藏状态下的X
		void Act();	//激活
		void Unact();	//不激活
		void Hide();	//隐藏
		void SetLHNum(int num);	//设置立绘编号
		void SetCharater(const std::string&);	//设置人物

		void Update() override;
		void Draw() const override;
		void Kill() override;
	};

	//逻辑
	class ConversationLogic
	{
	private:
		ConversationStandImage* m_left = nullptr, *m_right = nullptr;
		
		Basic::TaskList m_tasks;

		std::queue<ConversationScript::Script::Event> m_script;	//完成任务后才能Pop

		ConversationScript::Script* m_scriptLoader;

		bool m_thisTaskFinished = true;
		bool m_clickEnabled;
		bool m_canUseClick;

		GameObject* m_nameCard = nullptr;

		std::function<void()> m_clickTask = [this]() {
			//m_skipping = true;
		};

		double m_nowX = -400, m_nowY = 200;

		int m_nowProgram = 0;
		double m_nowR = 1, m_nowG = 1, m_nowB = 1;

		std::vector<std::unique_ptr<ConversationWord>> m_spriteString;
		bool m_nameCardEnabled = false;

		ConversationDelayer m_delayer;
	public:
		static bool Exist();

		ConversationLogic(bool clickEnabled = true);
		~ConversationLogic();

		void SetLHObject(ConversationStandImage* left, ConversationStandImage* right);

		inline void EnableNameCard()
		{
			m_nameCardEnabled = true;
		}

		bool Update();
		void Draw() const;
		void UseScript(ConversationScript::Script&, const std::string& charName);
		inline void ClickEnabled(bool b) {
			m_canUseClick = b;
		}
		inline void Click() {
			if (m_clickEnabled && m_canUseClick) {
				if (m_delayer.NowClickable()) {
					m_delayer.Click();
					m_clickTask();
					m_clickTask = [this]() {
						//m_skipping = true;
					};
				}
			}
		}
	};

	//逻辑显示器，用于对接游戏平台和对话系统
	class ConversationLogicDisplay :public GameObject {
	private:
		const ConversationLogic* m_logic;	//Borrow!!!
	public:
		ConversationLogicDisplay(ConversationLogic* logic) :m_logic(logic) { m_died = false; }
		void Draw() const override;
		void Update() override;
		void Kill() override;
	};
}
