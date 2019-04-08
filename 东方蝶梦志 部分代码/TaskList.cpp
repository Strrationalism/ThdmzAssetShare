#include "NuzPrecompiled.h"
#include "TaskList.h"





void Basic::TaskList::AddTask(std::function<void()> f, int time)
{
	//std::lock_guard<std::mutex> g(m_addLock);
	m_tasks.push(
		{ time + m_time, f }
	);
}

void Basic::TaskList::Update()
{
	m_time++;

	if (m_tasks.empty()) return;
	while (m_time >= m_tasks.top().first) {
		m_tasks.top().second();
		m_tasks.pop();
		if (m_tasks.empty()) return;
	}
}

void Basic::TaskList::DoAllNow()
{
	while (!m_tasks.empty()) {
		m_tasks.top().second();
		m_tasks.pop();
	}
}

void Basic::TaskList::Clear() {
	while (!m_tasks.empty())
		m_tasks.pop();
}
