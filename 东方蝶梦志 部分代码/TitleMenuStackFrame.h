#pragma once
#include "BasicInput.h"

namespace Activities::Title
{
	class TitleMenuStackFrame
	{
	public:
		virtual ~TitleMenuStackFrame() {}
		virtual void OnEntryToThis() {}				//进入时显示
		virtual void OnEntryOther() {}				//进入其他子菜单时隐藏
		virtual void OnBackToThisShow() {}			//从子菜单返回到本菜单时
		virtual void OnWannaBackToThisShow() {}		//即将返回到本菜单但是仍未返回时

		virtual void OnDraw() const {}				//普通渲染时
		virtual void OnDrawYSJD() const {}			//颜色减淡渲染时
		virtual void OnDrawPost() const {}			//后处理渲染时

		virtual void OnUpdate(float deltaTime) {}	//每帧

		virtual void OnEvent(const Basic::Input::Message&) {}	//事件
	};
}
