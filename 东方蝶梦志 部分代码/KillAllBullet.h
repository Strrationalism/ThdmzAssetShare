#pragma once
#include "StageTask.h"

namespace StageTask {
	class KillAllBullet :
		public StageTask::BaseTask
	{
	public:
		void Create(Basic::CSVReader&, const StageData&) override;	//从C列开始读取参数
		void Proc() override;	//执行任务
		bool Finished() override;	//是否已完成任务
	};
}
