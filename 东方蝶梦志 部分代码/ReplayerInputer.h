#pragma once
#include "BaseInputer.h"
#include "Movie.h"
namespace Inputer {
	class ReplayerInputer :
		public BaseInputer
	{
	private:
		Movie m_movie;

		int m_nowFrame = -1;

		std::queue<Basic::Input::Message> m_msg;

		void clearQueues();
		void loadFrameQueue();

		void prep();
	public:
		bool PopMessage(Basic::Input::Message&) override;
		void PushMessage(const Basic::Input::Message&) override;

		inline auto& GetMovie() {
			return m_movie;
		}

		int GetFrameBeat() override;

		unsigned GetRandSeed() override;

		void Reset();
	};
}