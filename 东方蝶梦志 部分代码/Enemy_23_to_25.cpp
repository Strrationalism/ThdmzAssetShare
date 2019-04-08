#include "Precompiled.h"
#include "Enemy_23_to_25.h"
#include "EnemyStyleManager.h"
#include "Game.h"
using namespace Enemy::SpecialEnemy;

void Enemy_23_to_25::Update()
{
	Enemy::Enemy::Update();

	double x, y;
	GetPos(x, y);

	m_backLight.SetPos(x, y);

	double wave = sin(Game::GetGame().GetGameTime() / 10.0);
	m_backLight.SetZoom((1.1 + 0.1 * wave) + (1 - m_beginEffect));
	m_enemyImage.SetZoom(1.0 + (1 - m_beginEffect));
	m_enemyImage.SetAlpha(m_beginEffect);
	m_backLight.SetAlpha((0.8 + 0.2 * wave)*m_beginEffect);

	double imageAngle = Game::GetGame().GetGameTime() * 2;
	m_enemyImage.SetAngle(imageAngle);
	m_backLight.SetAngle(imageAngle);

	m_backLight.UpdateVertex();
}

void Enemy_23_to_25::Draw() const
{
	m_backLight.DrawToFastBuffer(Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR);
	Enemy::Enemy::Draw();
}

static const double colorFliter[3][3] = {
	{1,1,1},
	{1,0,0},
	{0,0,1}
};

void Enemy_23_to_25::Create(const StageData::EnemyData & e, bool createdByScript, Enemy* hpLink)
{
	Enemy::Enemy::Create(e, createdByScript, hpLink);

	const auto& style = EnemyStyleManager::GetStyle(e.style);
	m_backLight.UseImage("Enemys/" + style.textureName, 0);
	m_backLight.SetColorFliter(
		colorFliter[e.style - 23][0],
		colorFliter[e.style - 23][1],
		colorFliter[e.style - 23][2]
	);

	m_dataActor.Add(m_beginEffect, 1, 25, 1);
}
/*
#include "MemoryPool.h"
static MemoryPool<Enemy::SpecialEnemy::Enemy_23_to_25, 20> pool;
void* Enemy::SpecialEnemy::Enemy_23_to_25::operator new(size_t)
{
	return pool.Get();
}

void Enemy::SpecialEnemy::Enemy_23_to_25::operator delete(void* ptr)
{
	pool.Return(ptr);
}
*/