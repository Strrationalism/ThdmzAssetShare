#pragma once
#include "BasicInput.h"
namespace Basic {
	namespace ActivityEngine {

		//活动类
		//概念同Snow::Activity
		class Activity {
		public:
			virtual ~Activity() {}
			virtual void OnUpdate(float deltaTime) = 0;	//逻辑更新时
			virtual void OnDraw() const = 0;	//需要绘制时
			virtual void OnFadeIn(int frame) {}	//使用Goto切入时，传入预计总耗时（帧）
			virtual void OnFadeInUpdate(double finished) {}	//使用Goto切入后（正在淡入，传入动画完成百分比。）
			virtual void OnFadeOut(int frame) {}	//使用Goto切出时，传入预计总耗时（帧）
			virtual void OnFadeOutUpdate(double finished) {}	//使用Goto切出后（正在淡出，传入动画完成百分比。）
			virtual void OnEntry() {}	//使用Goto进入活动时（OnFadeIn前）
			virtual void OnLeave() {}	//使用Goto离开活动时（OnFadeOut且动画结束后）
			virtual void OnEvent(const Basic::Input::Message&) {}	//玩家输入时，只有栈顶的活动才能收到消息。
		};
	}
}