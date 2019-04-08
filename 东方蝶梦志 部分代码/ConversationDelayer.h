#pragma once
#include <optional>
namespace Conversation {
	class ConversationDelayer
	{
		std::optional<int> m_clickWatchDog;
	public:

		bool NowClickable() const;	//当前是否可点击
		void Click();	//点击一次
		void Update();	//更新一帧
	};

}