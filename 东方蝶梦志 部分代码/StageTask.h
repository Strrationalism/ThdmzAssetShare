#pragma once
#include "CSVReader.h"
#include <memory>
struct StageData;

namespace StageTask {
	class BaseTask {
	public:
		virtual ~BaseTask() {}
		virtual void Create(Basic::CSVReader&, const StageData&) = 0;	//从C列开始读取参数
		virtual void Proc() = 0;	//执行任务
		virtual bool Finished() = 0;	//是否已完成任务,一定会执行一次。
	};

	std::shared_ptr<BaseTask> CreateTaskObject(const std::string& taskName);
}
