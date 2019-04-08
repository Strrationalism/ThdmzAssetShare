#include "Precompiled.h"
#include "GetAllItem.h"
#include "Game.h"

void StageTask::GetAllItem::Create(Basic::CSVReader &, const StageData&)
{
}

void StageTask::GetAllItem::Proc()
{
	Game::GetGame().GetItemMgr().GetAll();
	m_finished = true;
}

bool StageTask::GetAllItem::Finished()
{
	return m_finished;
}