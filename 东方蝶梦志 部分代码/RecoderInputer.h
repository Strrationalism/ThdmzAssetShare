#pragma once
#include "BaseInputer.h"
#include "Movie.h"

namespace Inputer {
	class RecoderInputer :
		public BaseInputer
	{
	private:
		Movie m_movie;
		std::queue<Basic::Input::Message> m_msgs;
	public:
		bool PopMessage(Basic::Input::Message&) override;
		void PushMessage(const Basic::Input::Message&) override;

		inline auto & GetMovie() {
			return m_movie;
		}

		int GetFrameBeat() override;

		unsigned GetRandSeed() override;
	};
}
