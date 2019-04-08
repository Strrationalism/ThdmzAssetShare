#include "Precompiled.h"
#include "BulletMarisa2.h"

static double c_bulletDamage = 1.35;
static const int c_imageIdx = 5;

void Char::Marisa::BulletMarisa2::Update()
{
	++m_timer;
	SetSpeed(m_nowSpeed);

	double x, y;
	GameObjectWithDmslControl::GetPos(x, y);
	if (WIDTH / 2 + 13 <= x) m_died = true;
	GameObjectWithDmslControl::Update();
	m_sprite.SetPos(x, y);
	m_sprite.SetAlpha(m_powerX1dot3 ? 2 : 1);
	m_sprite.SetZoom(m_powerX1dot3 ? 1.3 : 1);

	m_sprite.UpdateVertex();

	auto &enemyManager = Game::GetGame().GetEnemyManager();
	for (size_t i = 0; i < enemyManager.Size(); ++i)
	{
		auto &enemy = enemyManager[i];
		auto &c1 = enemy.GetCollision();
		if (!enemy.GetColliEnabled()) continue;

		const int colliRadius = 32;
		Collision::Circle c2{ x, y, colliRadius };
		if (Collision::CheckC2C(c1, c2))
		{
			enemy.DecHP(c_bulletDamage);
			//Kill();	注释掉此行以使魔机可贯穿
		}
	}

	m_powerX1dot3 =
		Game::GetGame().GetPlayer().GetSheild().has_value() ?
		Game::GetGame().GetPlayer().GetSheild()->IsShootPowerX1dot3() :
		false;
}

void Char::Marisa::BulletMarisa2::Draw() const
{
	m_sprite.DrawToFastBuffer(
		m_powerX1dot3 ?
		Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR :
		Basic::Video::Renderer2D::FASTBUFFER_BLEND_NORMAL
	);
}

void Char::Marisa::BulletMarisa2::Kill()
{
	m_died = true;
}

void Char::Marisa::BulletMarisa2::Create(double x, double y)
{
	m_died = false;
	m_sprite.UseImage("PlayerBullet", c_imageIdx);
	m_maxSpeed = GetSpeed();
	m_nowSpeed = 0.2 * m_maxSpeed;
	m_dataActor.Add(m_nowSpeed, m_maxSpeed, 60, 0);
	SetSpeed(m_nowSpeed);
	SetPos(x, y);
	SetRadius(8);
	m_sprite.SetZoom(1);
	m_sprite.SetAlpha(0.9);
}

void Char::Marisa::BulletMarisa2::OnColli(Enemy::Enemy & enemy)
{
	Kill();
	enemy.DecHP(m_powerX1dot3 ? c_bulletDamage * 1.1 : c_bulletDamage);
}

void Char::Marisa::BulletMarisa2::SetPowerData(double d)
{
	c_bulletDamage = d;
}

#include "MemoryPool.h"

static MemoryPool<Char::Marisa::BulletMarisa2, 25> pool;

void* Char::Marisa::BulletMarisa2::operator new(size_t)
{
	return pool.Get();
}

void Char::Marisa::BulletMarisa2::operator delete(void* ptr)
{
	pool.Return(ptr);
}