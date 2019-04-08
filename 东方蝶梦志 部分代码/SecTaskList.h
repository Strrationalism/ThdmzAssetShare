#pragma once
#include <functional>
#include <vector>

namespace Basic
{
	class SecTaskList
	{
	public:
		void Update(float deltaTime);
		void Clear();
		void AddTask(std::function<void()> func, float time);
	private:
		struct SecTask
		{
			std::function<void()> func;
			float timer;
		};

		std::vector<SecTask> m_tasks;
	};
}