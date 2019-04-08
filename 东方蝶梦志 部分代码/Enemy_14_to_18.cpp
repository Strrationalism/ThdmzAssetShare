#include "Precompiled.h"
#include "Enemy_14_to_18.h"
#include "Game.h"
#include "EnemyStyleManager.h"

Enemy::SpecialEnemy::Enemy_14_to_18::Enemy_14_to_18()
{
}

Enemy::SpecialEnemy::Enemy_14_to_18::~Enemy_14_to_18()
{
}

const static double colorFilter[] = {
	1,1,0,	//14
	0.5,0.5,0.5,	//15
	1,0,0,	//16
	0,1,0,	//17
	0,0,1	//18
};

void Enemy::SpecialEnemy::Enemy_14_to_18::Create(const StageData::EnemyData & i, bool createdByScript, Enemy* hpLink)
{
	Enemy::Create(i, createdByScript, hpLink);
	const auto& style = EnemyStyleManager::GetStyle(i.style);

	m_background.UseImage("Enemys/" + style.textureName, style.gameFrame_animationFrame, 0);
	m_background.SetColorFliter(colorFilter[(i.style - 14) * 3], colorFilter[(i.style - 14) * 3 + 1], colorFilter[(i.style - 14) * 3 + 2]);

	m_particleNumber = 2 * (i.style - 14) + 5;

	m_startEffect = 0;
	m_dataActor.Add(m_startEffect, 1.0, 15, 1);
}

void Enemy::SpecialEnemy::Enemy_14_to_18::Update()
{
	Enemy::Update();
	m_background.SetPos(m_collision.x, m_collision.y);
	m_background.Update();
	m_background.SetZoom(1.3 * m_startEffect);
	m_background.UpdateVertex();
	m_background.SetAlpha(m_startEffect *(0.6 + 0.1 * sin(Game::GetGame().GetGameTime() / 10.0)));

	m_enemyImage.SetZoom(3 - 2 * m_startEffect);
	//m_enemyImage.SetAlpha(m_startEffect);
	m_enemyImage.UpdateVertex();

	if (m_startEffect >= 0.5 && Game::GetGame().GetGameTime() % 2 == 0) {
		Game::GetGame().GetParticleTop().AddDot(m_particleNumber, [](Basic::Video::Renderer3D::Particle3D::Dot3D& d) {
			d.speed = 2.0;
			d.size = 256.0;
			d.angle = -M_PI;
			d.x += 8 * (2 * Rand() - 1);
			d.blend_dst = GL_ONE;
			d.userdata[0] = (rand() % 2) ? -0.01 : 0.01;
		}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& d, auto&) {
			d.speed *= 0.99999;
			d.size -= 0.1;
			d.alpha -= 0.1;
			d.self_angle += d.userdata[0];
		}, m_collision.x, m_collision.y - 5
			);

		Game::GetGame().GetParticleTop().AddDot(m_particleNumber + 1, [](Basic::Video::Renderer3D::Particle3D::Dot3D& d) {
			d.speed = 2.0;
			d.size = 256.0;
			d.angle = -M_PI;
			d.userdata[0] = (rand() % 2) ? -0.01 : 0.01;
			d.x += 8 * (2 * Rand() - 1);
			d.blend_dst = GL_ONE;
		}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& d, auto&) {
			d.speed *= 0.99999;
			d.size -= 0.1;
			d.alpha -= 0.035;
			d.self_angle += d.userdata[0];
		}, m_collision.x, m_collision.y - 5
			);
	}
}

void Enemy::SpecialEnemy::Enemy_14_to_18::Draw() const
{
	m_background.DrawToFastBuffer(Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR);
	Enemy::Draw();
}

/*
#include "MemoryPool.h"
static MemoryPool<Enemy::SpecialEnemy::Enemy_14_to_18, 20> pool;
void* Enemy::SpecialEnemy::Enemy_14_to_18::operator new(size_t)
{
	return pool.Get();
}

void Enemy::SpecialEnemy::Enemy_14_to_18::operator delete(void* ptr)
{
	pool.Return(ptr);
}*/