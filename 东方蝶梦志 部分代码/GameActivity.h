#pragma once
#include "Activity.h"
#include "GameActBridge.h"
#include "BaseInputer.h"
#include "StageData.h"
#include "GamePausedActivity.h"
#include <queue>
#include "BasicChar.h"
#include "MovieData.h"
#include "SecTweenData.h"
#include "SecTaskList.h"
namespace Activities
{
	class GameActivity : public Basic::ActivityEngine::Activity
	{
	public:
		using GameActivityParam =
			std::tuple<std::queue<std::string>, StageLevel, Char::BasicChar*,bool>;

		class Bridge : public GameActBridge
		{
		private:
			GameActivity * m_act;
			std::shared_ptr<Activities::GamePausedActivity> m_pauser;

			bool m_canSaveReplay = true;

			const bool m_practice;
		public:
			Bridge(GameActivity&, const std::shared_ptr<Activities::GamePausedActivity>&,bool practice);
			void OnPlayerAllDied() override;	//玩家死亡时
			void OnStageClear() override;	//当关卡通过时
			void FinishSpell(int spellHash, bool bonusGot) override;
			inline auto GetAct()
			{
				return m_act;
			}

			inline auto CanSaveAct()
			{
				return m_canSaveReplay;
			}
		};

		friend class Bridge;
	private:
		std::shared_ptr<Activities::GamePausedActivity> m_pauseActivity;
		std::unique_ptr<Inputer::BaseInputer> m_inputer;

		std::string m_stageName;
		GameInitData m_stageInitData;
		std::unique_ptr<MovieData::MovieData> m_movieData = std::make_unique<MovieData::MovieData>();

		std::queue<std::string> m_stages;
		const StageLevel m_gameLevel;		

		Char::BasicChar& m_charater;

		Bridge m_bridge;
		void nextStage();	//载入下一关

		GameActivityParam m_param;

		Basic::SecTaskList m_taskList;
		Basic::SecTweenData<float> m_darkness;

		bool m_running = true;

		void saveStageMovieToMovieData();

		bool m_accMode = false;
	public:
		GameActivity(const std::queue<std::string>& stages,StageLevel level,Char::BasicChar& charater,bool isPractice);
		~GameActivity();
		void OnEntry() override;
		void OnDraw() const override;
		void OnUpdate(float deltaTime) override;
		void pause();
		void OnEvent(const Basic::Input::Message&) override;

		GameActivityParam GetParam();

		static std::shared_ptr<GameActivity> CreateGameActivity(GameActivityParam&& param);
	};


}