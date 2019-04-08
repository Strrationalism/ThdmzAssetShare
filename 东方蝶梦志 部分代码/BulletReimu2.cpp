#include "Precompiled.h"
/*#include "BulletReimu2.h"
#include "Game.h"
#include "CharReimu.h"

void Char::Reimu::BulletReimu2::findNearestEnemy()
{
	double x, y;
	GameObjectWithDmslControl::GetPos(x, y);
	auto& em = Game::GetGame().GetEnemyManager();
	m_nearestEnemy  = ((CharReimu*)Game::GetGame().GetPlayer().GetChar())->GetNearestEnemy();
	if (m_nearestEnemy != -1 && m_nearestEnemy < em.Size()) {
		auto c1 = em[m_nearestEnemy].GetCollision();
		//m_enemyAngle = atan2((c1.y - y), (c1.x - x));
	}
}

static double inline bezier2(double x1,double x2,double x3, double t) {
	return (1 - t) * (1 - t) * x1 + 2 * t*(1 - t)*x2 + t *t * x3;
}

void Char::Reimu::BulletReimu2::Update()
{
	double x, y;
	m_dataActor.Update();
	GameObjectWithDmslControl::GetPos(x, y);
	if (x >= WIDTH / 2 + 13 || x <= -WIDTH /2 - 13 || y >= HEIGHT/2 + 6 || y <= -HEIGHT/2 -6) m_died = true;
	//GameObjectWithDmslControl::Update();
	m_sprite.SetPos(x, y);
	m_sprite.UpdateVertex();

	//if (m_timer == 0) {
		//findNearestEnemy();
	//}

	if (m_nearestEnemy != -1) {
		//终点
		double ex, ey;

		if (Game::GetGame().GetEnemyManager().Size() > (unsigned)m_nearestEnemy) {
			Game::GetGame().GetEnemyManager()[m_nearestEnemy].GetPos(ex, ey);

			//控制点
			double cx = m_startX + 140;

			//当前点
			double nx = bezier2(m_startX, cx, ex, m_berzierNow);
			double ny = bezier2(m_startY, m_startY, ey, m_berzierNow);

			GameObjectWithDmslControl::SetPos(nx, ny);
			m_sprite.SetPos(nx, ny);
			m_angle = atan2(ny - y, nx - x);
		}
		else {
			m_nearestEnemy = -1;
			//m_enemyAngle = 0;
			ex = 0;
			ey = 0;
		}
	}
	else
	{
		GameObjectWithDmslControl::SetSpeed(27);
		GameObjectWithDmslControl::Update();
	}
	//++m_timer;
	m_sprite.SetAngle(m_angle/double(M_PI)*180);
	GameObjectWithDmslControl::SetAngle(m_angle);
}

void Char::Reimu::BulletReimu2::Draw() const
{
	m_sprite.DrawToFastBuffer();
}

void Char::Reimu::BulletReimu2::Kill()
{
	m_died = true;
}

void Char::Reimu::BulletReimu2::Create(double x, double y)
{
	m_sprite.UseImage("PlayerBullet", 9);
	m_died = false;
	GameObjectWithDmslControl::SetSpeed(27);
	GameObjectWithDmslControl::SetAngle(0);
	//m_angle = 0;
	GameObjectWithDmslControl::SetPos(x, y);
	SetRadius(8);

	m_startX = x;
	m_startY = y;

	findNearestEnemy();
	if (m_nearestEnemy != -1 && m_nearestEnemy < Game::GetGame().GetEnemyManager().Size()) {
		auto& c1 = Game::GetGame().GetEnemyManager()[m_nearestEnemy].GetCollision();
		double length = ((c1.x - x)*(c1.x - x) + (c1.y - y)*(c1.y - y));
		m_dataActor.Add(m_berzierNow, 1.0, sqrt(length/1000), 1);
	}
}

void Char::Reimu::BulletReimu2::OnColli(Enemy::Enemy &enemy)
{
	Kill();
	double x, y;
	enemy.GetPos(x, y);
	Game::GetGame().GetParticleTop().AddDot(3,
		[](Basic::Video::Renderer3D::Particle3D::Dot3D& d) {
		d.size *= 16;
		d.angle = M_PI;
		d.alpha = 0.6f;
		d.speed = 2.5;
		d.x -= double(rand()) / RAND_MAX * 40;
		//d.y += double(rand()) / RAND_MAX * 10 - 5;
		d.y += 30;
		d.r = 0XFA / 256.0f;
		d.g = 0X5E / 256.0f;
		d.b = 0X0A / 256.0f;

		d.blend_dst = GL_ONE;
	},
		[](Basic::Video::Renderer3D::Particle3D::Dot3D& d, Basic::Video::Renderer3D::Particle3D& i) {
		d.speed += 0.05f * double(rand()) / RAND_MAX;
		d.size -= 0.04;
		d.alpha -= 0.03;
		d.g += 0.02;
	}, x, y, 1);
	enemy.DecHP(0.4);
}
*/