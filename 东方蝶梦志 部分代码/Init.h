#pragma once
#include <future>
#include <any>
#include "TaskList.h"

namespace Basic::Init
{
	void SimpleInit();
	void InitMustInMainThread();
	void InitOpenGL();

	//返回值为最少停留帧数
	int CacheTexture(Basic::TaskList& cacheTaskList,std::any& cache);
}