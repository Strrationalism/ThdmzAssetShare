#pragma once
#include "StageTask.h"
#include <vector>
#include <queue>
namespace StageTask {
	class StageTaskRunner
	{
	private:
		std::vector<std::queue<std::pair<int, std::shared_ptr<BaseTask>>>> m_runningTasks;
	public:
		void Clear();
		void AddTask(int time, std::shared_ptr<BaseTask> task);
		void AddTaskAfterLast(int afterTime, std::shared_ptr<BaseTask> task);
		void Update();
	};
}