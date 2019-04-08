#pragma once
#include "NuzPrecompiled.h"
#include "TaskList.h"

namespace Basic {
	namespace ActivityEngine {
		class Activity;

		class ActivityEngine final
		{
		private:

			int m_cnt = 0;

			Basic::TaskList m_taskList;
			std::stack<std::shared_ptr<Activity>> m_now;	//当前活动中的活动
			int m_frameMode = 0;
			bool m_run;	//活动引擎保持运行
			ActivityEngine() = default;
		public:
			void SetFPS(int fps);	//设置帧率，0为无限制，开启垂直同步时必须设置为0
			void Goto(std::shared_ptr<Activity>);	//跳转到活动（平滑）
			void Call(std::shared_ptr<Activity>);	//保留当前活动同时调用新活动
			void Return();	//跳出当前活动并返回上一活动
			void Run(std::shared_ptr<Activity>);	//启动第一个活动,没有进入动画
			inline void Exit() {	//退出活动引擎
				m_run = false;
				//exit(0);
			}

			inline auto GetTopActivity() {
				return m_now.top();
			}

			inline auto& GetTaskList() {
				return m_taskList;
			}

			inline int GetFrameTime() { return m_cnt; }

			static ActivityEngine& GetActivityEngine();
		};

	}
}