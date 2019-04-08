#pragma once
#include <functional>
#include "TitleMenuStackFrame.h"

namespace Activities::Title::MenuStack
{
	class TitleActionMenu : public TitleMenuStackFrame
	{
	private:
		float m_timer;
		std::function<void()> m_func;
	public:
		TitleActionMenu(std::function<void()>&& func,float timer);

		void OnUpdate(float t) override;
	};
}
