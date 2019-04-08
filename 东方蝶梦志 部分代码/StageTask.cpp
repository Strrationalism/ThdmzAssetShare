#include "Precompiled.h"
#include "StageTask.h"
#include "KillAllEnemy.h"
#include "KillAllBullet.h"
#include "GetAllItem.h"
#include "BossTask.h"
#include "StageTitle.h"
#include "ConversationTask.h"
#include "Globals.h"
#include "ClearTask.h"
#include "EnableEnemyCSV.h"
#include "Enemy_GolbalThread.h"

using namespace StageTask;

std::shared_ptr<BaseTask> StageTask::CreateTaskObject(const std::string & taskName)
{
	BaseTask* p = nullptr;
	if (taskName == "KillAllEnemy") p = new KillAllEnemy;
	else if (taskName == "KillAllBullet") p = new KillAllBullet;
	else if (taskName == "GetAllItem") p = new GetAllItem;
	else if (taskName == "ConversationTask") p = new StageTask::Conversation;
	else if (taskName == "StageTitle") p = new StageTask::StageTitle;
	else if (taskName == "CreateBoss") p = new CreateBoss;
	else if (taskName == "CreateBossWB") p = new CreateBoss(true);
	else if (taskName == "StartBoss") p = new StartBoss;
	else if (taskName == "BossPowerEffect") p = new BossPowerEffect;
	else if (taskName == "Clear") p = new ClearTask;
	else if (taskName == "EnableEnemyCSV") p = new EnableEnemyCSV;
	else if (taskName == "CreateStageDmsl") p = new CreateStageDmsl;
	else if (taskName == "BossAttachStage5DoremiBook") p = new BossAttachStage5DoremiBook;
	else Crash(L"未知关卡任务：" + Str2WStr(taskName));
	return std::shared_ptr<BaseTask>(p);
}