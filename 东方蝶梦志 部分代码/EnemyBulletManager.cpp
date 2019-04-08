#include "Precompiled.h"
#include "EnemyBulletManager.h"
#include "Game.h"
#include "Bullet.h"

void EnemyBullet::EnemyBulletManager::KillAll()
{
	for (auto& p : m_gom)
		p.KillAll();
}

void EnemyBullet::EnemyBulletManager::KillAllKeepNotKillLayer() {
	for (unsigned i = 0; i < m_gom.size(); ++i) {
		if (m_playerDiedNoKillLayer.count(i) == 0) {
			m_gom.at(i).KillAll();
		}
	}
}

void EnemyBullet::EnemyBulletManager::KillInCircle(Collision::Circle cir)
{
	for (auto& layer : m_gom) {
		for (auto& bullet : layer)
		{
			if (((EnemyBullet::Bullet&)*bullet).CollCheckWithCircle(cir))
				bullet->Kill();
		}

	}
}

void EnemyBullet::EnemyBulletManager::KillKeepNotKillLayerInCircle(Collision::Circle cir)
{
	for (unsigned i = 0; i < m_gom.size(); ++i) {
		if (m_playerDiedNoKillLayer.count(i) == 0) {
			for (auto& bullet : m_gom.at(i))
			{
				if (((EnemyBullet::Bullet&)*bullet).CollCheckWithCircle(cir))
					bullet->Kill();
				
			}
		}
	}
}

void EnemyBullet::EnemyBulletManager::KillAllKeepNotKillLayerWithCenterPosition(double x, double y)
{
	for (int i = 1; i < 100; ++i)
	{

		Game::GetGame().GetTaskList().AddTask([i,x,y]() {
			Collision::Circle c;
			c.radius = i * 20;
			c.x = x;
			c.y = y;

			Game::GetGame().GetEnemyBulletManager().KillKeepNotKillLayerInCircle(c);
		},i);
	}
}

void EnemyBullet::EnemyBulletManager::KillAllWithCenterPosition(double x, double y)
{
	for (int i = 1; i < 100; ++i)
	{

		Game::GetGame().GetTaskList().AddTask([i, x, y]() {
			Collision::Circle c;
			c.radius = i * 20;
			c.x = x;
			c.y = y;

			Game::GetGame().GetEnemyBulletManager().KillInCircle(c);
		}, i);
	}
}

void EnemyBullet::EnemyBulletManager::ForceKillAll()
{
	for (auto& p : m_gom)
		p.ForceKillAll();
}
