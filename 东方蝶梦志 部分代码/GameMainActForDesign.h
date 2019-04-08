#pragma once
#include "Activity.h"
#include "Game.h"
#include "GameActBridge.h"
#include "RecoderInputer.h"
#include "ReplayerInputer.h"
#include "SnowRVReader.h"
#include "Sprite2DWStreamLayout.h"
#include "GamePausedActivity.h"

/* 游戏活动 */
/* 用来给游戏逻辑垫底 */

//#define REPLAYMODE

namespace Activities {


	class GameMainActForDesign final :public Basic::ActivityEngine::Activity
	{
	private:
		class BridgeForDesign :public GameActBridge {
		private:
			GameMainActForDesign* m_act = nullptr;
		public:
			void OnPlayerAllDied() override;	//玩家死亡时
			void OnStageClear() override;	//当关卡通过时

			void SetActivity(GameMainActForDesign* act) {
				m_act = act;
			}
		};

		std::shared_ptr<StageData> m_stageData;

		std::shared_ptr<Activities::GamePausedActivity> m_pauseActivity;

/*#ifdef REPLAYMODE
		Inputer::ReplayerInputer m_inputer;
#else
		Inputer::RecoderInputer m_inputer;
		GameInitData m_gameInitData;
#endif*/
		bool m_replayMode = false;

		std::unique_ptr<Inputer::BaseInputer> m_inputer;
		GameInitData m_gameInitData;

		bool m_debug_3xSpeed = false;	//调试：F1是否按下->是否开启三倍速运行速度

		BridgeForDesign m_bridge;

		std::optional<Basic::SnowRVReader> m_debugInfo;
	public:
		GameMainActForDesign(bool replayMode = false);
		~GameMainActForDesign();
		
		void OnEntry() override;
		void OnDraw() const override;
		void OnUpdate(float deltaTime) override;
		void OnEvent(const Basic::Input::Message&) override;
	};

}