#pragma once
#include "StageTask.h"

namespace StageTask {
	class ClearTask :
		public StageTask::BaseTask
	{
	private:
		unsigned m_baseScore;
		bool m_first = true;
	public:
		void Create(Basic::CSVReader&, const StageData&) override;
		void Proc() override;	//执行任务
		bool Finished() override;	//是否已完成任务,一定会执行一次。
	};
}
