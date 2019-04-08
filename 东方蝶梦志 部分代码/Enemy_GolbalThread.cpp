#include "Precompiled.h"
#include "Enemy_GolbalThread.h"
#include "Game.h"

void Enemy::SpecialEnemy::Enemy_GolbalThread::Create(int dmsl, double x, double y)
{
	m_died = false;
	m_dmslUnit.push_back({
		true,
		std::make_unique<Dmsl::DmslUnit>(*m_dvm, dmsl)
	});
	m_dmslUnit.back().ptr->SetUserData((GameObjectWithDmslControl*)this);

	callDmsl();

	GameObjectWithDmslControl::SetPos(x, y);
	GameObjectWithDmslControl::GetPos(m_collision.x, m_collision.y);
}

void Enemy::SpecialEnemy::Enemy_GolbalThread::Kill()
{
	m_died = true;
}

void Enemy::SpecialEnemy::Enemy_GolbalThread::Update()
{
	callDmsl();

	m_dmslUnit.erase(std::remove_if(m_dmslUnit.begin(), m_dmslUnit.end(), [](auto& p) {
		return !p.running;
	}), m_dmslUnit.end());

	GameObjectWithDmslControl::Update();

	GameObjectWithDmslControl::GetPos(m_collision.x, m_collision.y);

	if (m_dmslUnit.empty()) Kill();
}

void Enemy::SpecialEnemy::Enemy_GolbalThread::Draw() const
{
}

void StageTask::CreateStageDmsl::Create(Basic::CSVReader & c, const StageData &)
{
	m_dmsl = c.PopInt();
	m_x = c.PopDouble();
	m_y = c.PopDouble();
}

void StageTask::CreateStageDmsl::Proc()
{
	auto p = new Enemy::SpecialEnemy::Enemy_GolbalThread(GameObjectWithDmslControl::VMContext{
		Game::GetGame().GetStageData()->enemyScript,
		Game::GetGame().GetStageData()->enemyScriptLinkTable,
		Game::GetGame().GetStageData()->enemyDiedProgramHandle
	});
	p->Create(m_dmsl, m_x, m_y);
	Game::GetGame().GetUILayerGameObjectManager().Insert(p);
	m_ok = true;
}

bool StageTask::CreateStageDmsl::Finished()
{
	return m_ok;
}

/*
#include "MemoryPool.h"
static MemoryPool<Enemy::SpecialEnemy::Enemy_GolbalThread, 10> pool;
void* Enemy::SpecialEnemy::Enemy_GolbalThread::operator new(size_t)
{
	return pool.Get();
}

void Enemy::SpecialEnemy::Enemy_GolbalThread::operator delete(void* ptr)
{
	pool.Return(ptr);
}*/