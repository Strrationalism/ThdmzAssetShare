#include "Precompiled.h"
#include "KillAllEnemy.h"

#include "Game.h"

void StageTask::KillAllEnemy::Create(Basic::CSVReader &, const StageData&)
{
}

void StageTask::KillAllEnemy::Proc()
{
	Game::GetGame().GetEnemyManager().KillAll();
}

bool StageTask::KillAllEnemy::Finished()
{
	return true;
}