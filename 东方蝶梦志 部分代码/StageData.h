#pragma once
#include "BGMPlayer.h"
#include "Dmsl.h"
#include <string>
#include <array>
#include <memory>
#include "StageTask.h"
#include "DmslLib.h"
#include "BaseBackground.h"
enum class StageLevel {
	E = 0, N = 1, H = 2, L = 3
};

struct StageData {	//关卡数据
	std::string stageName;	//关卡名称

	//std::shared_ptr<Basic::Sound::Bgm> stageBgm, bossBgm;	//道中和Boss的BGM
	int stageBgm, bossBgm;

	int bulletLayerNum;
	std::vector<int> bulletNoKillLayerWhenPlayerDied;

	std::shared_ptr<Dmsl::DmslVirtualMachine> enemyScript;	//敌人行动脚本
	uint32_t enemyDiedProgramHandle;	//敌人死亡时执行的程序

	std::shared_ptr<Dmsl::GameObjectLibraryLinkTable> enemyScriptLinkTable;	//行动脚本变量链接表

	struct EnemyData {
		int birth_time;	//敌人出生时间
		double hp;	//HP
		int style;	//使用的敌人风格
		int dmslUnit;	//使用的编号

		double x, y;	//初始位置
		int minScreenLife;	//最小屏幕外生存时间

		std::shared_ptr<const Dmsl::DmslVarying> varying;
	};

	StageLevel level;

	std::unique_ptr<Background::BaseBackground> background;

	double dreamX;

	struct StageTaskData {
		int frames;
		std::shared_ptr<StageTask::BaseTask> task;
	};
	std::vector<StageTaskData> stageTasks;
};

std::shared_ptr<StageData> LoadStage(const std::string& stageName, StageLevel);	//加载关卡
void ReloadStageFast(const std::shared_ptr<StageData>& p);	//快速的重新加载已经加载的关卡