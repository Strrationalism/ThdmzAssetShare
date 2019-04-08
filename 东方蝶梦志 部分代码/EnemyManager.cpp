#include "Precompiled.h"
#include "EnemyManager.h"
#include "Globals.h"
#include "Game.h"
#include "BGMPlayer.h"
#include "DmslLib.h"
Enemy::EnemyManager::EnemyManager()
{
	m_enemys.Prep(25);
}
void Enemy::EnemyManager::Reset()
{
	m_canCreateByCSV = true;
	m_nextEnemy = 0;
}
void Enemy::EnemyManager::ForceKillAll()
{
	m_enemys.ForceKillAll();
}
void Enemy::EnemyManager::KillAll()
{
	m_enemys.KillAll();
}
void Enemy::EnemyManager::SetStageEnemyData(const std::shared_ptr<StageData>& e)
{
	m_stageData = e;

	switch (m_stageData->level) {
	case StageLevel::E:
		//m_stageData->enemyScript->SetUniform("level", 0);
		m_stageData->enemyScript->SetDMOVFSSelect(0);
		break;
	case StageLevel::N:
		//m_stageData->enemyScript->SetUniform("level", 1);
		m_stageData->enemyScript->SetDMOVFSSelect(1);
		break;
	case StageLevel::H:
		//m_stageData->enemyScript->SetUniform("level", 2);
		m_stageData->enemyScript->SetDMOVFSSelect(2);
		break;
	case StageLevel::L:
		//m_stageData->enemyScript->SetUniform("level", 3);
		m_stageData->enemyScript->SetDMOVFSSelect(3);
		break;
	}
}

void Enemy::EnemyManager::Update()
{
	//¸üÐÂµÐÈË
	m_enemys.Update();
}

void Enemy::EnemyManager::Draw() const
{
	m_enemys.Draw();
}