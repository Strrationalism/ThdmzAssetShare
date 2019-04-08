#include "Precompiled.h"
#include "PlayerBulletManager.h"
#include <algorithm>
#include "Collision.h"
#include "Game.h"

using namespace Collision;

void PlayerBulletManager::CheckCollision()
{
	for (auto &bullet : m_bullets)
	{
		if (!bullet->GetCheckCollision()) continue;

		double bulletX, bulletY;
		bullet->GetPos(bulletX, bulletY);
		const Circle c1{ bulletX, bulletY, bullet->GetRadius() };

		auto &enemyManager = Game::GetGame().GetEnemyManager();
		for (unsigned i = 0; i < enemyManager.Size(); ++i)
		{
			auto &enemy = enemyManager[i];
			if (!enemy.GetColliEnabled()) continue;

			const Circle c2 = enemy.GetCollision();
			if (CheckC2C(c1, c2))
				bullet->OnColli(enemy);
		}
	}
}

void PlayerBulletManager::Draw() const
{
	for (auto& b : m_bullets)
		b->Draw();
}

void PlayerBulletManager::Update()
{
	for (auto& b : m_bullets) {
		b->Update();
	}
	auto nend = std::remove_if(m_bullets.begin(), m_bullets.end(), [](auto& i) {
		return (i->Died());
	});

	m_bullets.erase(nend, m_bullets.end());

	CheckCollision();
}

void PlayerBulletManager::KillAll()
{
	int size = m_bullets.size();
	//#pragma omp parallel for
	for (int i = 0; i < size; ++i) {
		AT(m_bullets, i)->Kill();
	}
}

void PlayerBulletManager::ForceKillAll()
{
	m_bullets.clear();
}

void PlayerBulletManager::Shoot(Char::BasicCharBullet * b, double x, double y)
{
	m_bullets.push_back(std::unique_ptr<Char::BasicCharBullet>(b));
	m_bullets.back()->Create(x, y);
}