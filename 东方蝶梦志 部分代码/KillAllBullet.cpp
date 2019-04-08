#include "Precompiled.h"
#include "KillAllBullet.h"
#include "Game.h"

void StageTask::KillAllBullet::Create(Basic::CSVReader &, const StageData&)
{
}

void StageTask::KillAllBullet::Proc()
{
	Game::GetGame().GetEnemyBulletManager().KillAll();
}

bool StageTask::KillAllBullet::Finished()
{
	return true;
}