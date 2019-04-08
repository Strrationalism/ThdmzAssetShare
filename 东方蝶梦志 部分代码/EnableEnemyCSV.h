#pragma once
#include "StageTask.h"
namespace StageTask {
	class EnableEnemyCSV :
		public StageTask::BaseTask
	{
	private:
		bool m_finished = false;
		bool m_enable;
	public:
		void Create(Basic::CSVReader&, const StageData&) override;	//从C列开始读取参数
		void Proc() override;	//执行任务
		bool Finished() override;	//是否已完成任务,一定会执行一次。
	};
}