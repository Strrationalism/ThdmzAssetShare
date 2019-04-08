#include "Precompiled.h"
#include "EnableEnemyCSV.h"
#include "Game.h"
void StageTask::EnableEnemyCSV::Create(Basic::CSVReader & c, const StageData&)
{
	m_enable = c.PopInt() > 0 ? true : false;
}

void StageTask::EnableEnemyCSV::Proc()
{
	Game::GetGame().GetEnemyManager().EnableCreateByCSV(m_enable);
	m_finished = true;
}

bool StageTask::EnableEnemyCSV::Finished()
{
	return m_finished;
}