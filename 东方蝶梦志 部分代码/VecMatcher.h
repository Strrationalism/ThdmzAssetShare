#pragma once
#include <functional>

namespace Basic::Input {
	class VecMatcher final
	{
	private:
		enum class State
		{
			Nega,
			Zero,
			Posi
		};

		State m_lastState = State::Zero;

		std::function<void()> m_nega, m_posi;
	public:
		VecMatcher(std::function<void()>&& nega, std::function<void()>&& posi);
		void OnEvent(int vec);
	};
}
