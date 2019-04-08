#pragma once
#include <queue>
//#include <mutex>
#include <functional>
namespace Basic {
	class TaskList
	{
	private:
		struct Task {
			int first;
			std::function<void()> second;
			inline bool operator < (const Task& t) const {
				return t.first < first;
			}
		};
		std::priority_queue<Task> m_tasks;
		//std::mutex m_addLock;
		int m_time = 0;
	public:
		void AddTask(std::function<void()>,int time);
		void Update();
		void DoAllNow();
		void Clear();
	};

}