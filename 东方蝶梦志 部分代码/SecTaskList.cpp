#include "NuzPrecompiled.h"
#include "SecTaskList.h"

void Basic::SecTaskList::Update(float deltaTime)
{
	auto nend = std::remove_if(m_tasks.begin(), m_tasks.end(),
		[](SecTask& task) {
		return task.timer <= 0;
	});

	m_tasks.erase(nend, m_tasks.end());

	for (auto& func : m_tasks)
	{
		func.timer-= deltaTime;
		if (func.timer <= 0)
			func.func();
	}
}

void Basic::SecTaskList::Clear()
{
	m_tasks.clear();
}

void Basic::SecTaskList::AddTask(std::function<void()> func, float time)
{
	m_tasks.push_back({ func,time });
}
