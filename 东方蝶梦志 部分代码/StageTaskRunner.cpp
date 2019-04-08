#include "Precompiled.h"
#include "StageTaskRunner.h"
#include <algorithm>

void StageTask::StageTaskRunner::Clear()
{
	m_runningTasks.clear();
}

void StageTask::StageTaskRunner::AddTask(int time, std::shared_ptr<BaseTask> task)
{
	m_runningTasks.push_back(std::queue <std::pair< int, std::shared_ptr<BaseTask>>>());
	m_runningTasks.back().push({ time,task });
}

void StageTask::StageTaskRunner::AddTaskAfterLast(int afterTime, std::shared_ptr<BaseTask> task)
{
	m_runningTasks.back().push({ afterTime,task });
}

void StageTask::StageTaskRunner::Update()
{
	for (auto& p : m_runningTasks) {
		if (p.front().first == 1)
			p.front().first = 0;
		if (p.front().first <= 0)
			p.front().second->Proc();
		else
			p.front().first--;
	}

	auto finished = std::remove_if(m_runningTasks.begin(), m_runningTasks.end(), [](auto& p) {
		if (p.front().first <= 0)
			if (p.front().second->Finished())
				p.pop();
		return p.empty();
	});

	m_runningTasks.erase(finished, m_runningTasks.end());
}