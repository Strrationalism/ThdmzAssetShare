#pragma once
#include "Activity.h"
#include "GameActBridge.h"
#include "MovieData.h"
#include "ReplayerInputer.h"
#include "Sprite2D.h"
#include "GamePausedActivity.h"

namespace Activities
{
	class ReplayerActivity : public Basic::ActivityEngine::Activity
	{
	public:
		class Bridge : public GameActBridge
		{
		private:
			ReplayerActivity * m_act;

		public:
			Bridge(ReplayerActivity*);
			void OnPlayerAllDied() override;
			void OnStageClear() override;

			inline auto GetAct() const
			{
				return m_act;
			}

			void FinishSpell(int, bool) override;
		};
	private:
		friend class Bridge;

		

		Bridge m_bridge;
		std::shared_ptr<Activities::GamePausedActivity> m_pause;

		void loadStageAndMovie();

		std::unique_ptr<MovieData::MovieData> m_movieData;

		Inputer::ReplayerInputer m_inputer;

		std::queue<std::string> m_stageQueue;

		bool m_accmode = false;
		bool m_superacc = false;
		bool m_slowmode = false;

		Basic::Video::Renderer2D::Sprite2D m_repMark;
		float m_timer = 0;
		int m_frameTimer = 0;

		std::string m_path;
	public:
		ReplayerActivity(const std::string& path);

		void OnDraw() const override;
		void OnUpdate(float deltaTime) override;

		void OnEvent(const Basic::Input::Message& msg) override;

		inline const auto& GetPath() const
		{
			return m_path;
		}
	};
}
