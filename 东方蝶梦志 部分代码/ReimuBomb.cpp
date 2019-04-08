#include "Precompiled.h"
#include "ReimuBomb.h"
#include "Game.h"
#include "Enemy.h"
#include "Bullet.h"
#include "CharReimu.h"
#include "Haptic.h"

int Char::Reimu::ReimuBomb::m_num = 0;
int  Char::Reimu::ReimuBomb::m_searchNewEnemy = 0;
double Char::Reimu::ReimuBomb::m_angleChanging = 0;
static double _power;
void Char::Reimu::ReimuBomb::updateState0()
{
	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	x += cos(m_angleFromPlayer + m_angleChanging) * m_radFromPlayer;
	y += sin(m_angleFromPlayer + m_angleChanging) * m_radFromPlayer;
	GameObjectWithDmslControl::SetPos(x, y);
	m_angleChanging += 0.006;
}

void Char::Reimu::ReimuBomb::updateState1()
{
	if (Game::GetGame().GetEnemyManager().Size() <= (unsigned)m_nearestEnemy || m_nearestEnemy == -1)
		m_nearestEnemy = Reimu::CharReimu::Get().GetNearestEnemy();

	if (m_nearestEnemy != -1) {
		double ex, ey, px, py;
		Game::GetGame().GetEnemyManager()[m_nearestEnemy].GetPos(ex, ey);
		Game::GetGame().GetPlayer().GetPos(px, py);
		GameObjectWithDmslControl::SetAngle(atan2(ey - m_coll.y, ex - m_coll.x));
	}
	GameObjectWithDmslControl::Update();

	double x, y;
	GameObjectWithDmslControl::GetPos(x, y);
	if (x > WIDTH / 2) x = WIDTH / 2;
	if (x < -WIDTH / 2) x = -WIDTH / 2;
	if (y > HEIGHT / 2) y = HEIGHT / 2;
	if (y < -HEIGHT / 2) y = -HEIGHT / 2;
	GameObjectWithDmslControl::SetPos(x, y);

	if (m_nowSearch != m_searchNewEnemy) {
		m_nowSearch = m_searchNewEnemy;
		m_nearestEnemy = Reimu::CharReimu::Get().GetNearestEnemy();
	}

	if (m_nearestEnemy == -1) {	//如果没有搜索到的敌人
		if (x >= WIDTH / 2) {	//并撞到屏幕边缘
			Kill();
		}
	}
}

Char::Reimu::ReimuBomb::ReimuBomb(double startAngle)
{
	m_died = false;
	++m_num;

	m_timeOffset = startAngle / 360 * 125;

	Game::GetGame().GetPlayer().GetPos(m_coll.x, m_coll.y);
	GameObjectWithDmslControl::SetPos(m_coll.x, m_coll.y);
	m_coll.radius = 150;

	m_state = STATE0;

	m_angleFromPlayer = startAngle * double(M_PI) / 180;
	m_radFromPlayer = 0;
	m_dataActor.Add(m_radFromPlayer, 200, 120, 5);

	m_nowSearch = m_searchNewEnemy;
}

Char::Reimu::ReimuBomb::~ReimuBomb()
{
	--m_num;
}

void Char::Reimu::ReimuBomb::Update()
{
	GameObjectWithDmslControl::GetPos(m_coll.x, m_coll.y);
	--m_forceKill;
	if (0 == (m_forceKill + m_timeOffset)) {
		Kill();
	}

	m_dataActor.Update();

	if (m_forceKill <= 150 + m_timeOffset) m_state = STATE1;
	if (m_forceKill == 150 + m_timeOffset) {
		m_nearestEnemy = Reimu::CharReimu::Get().GetNearestEnemy();
		GameObjectWithDmslControl::SetSpeed(0);
		GameObjectWithDmslControl::SetAngle(0);
		GameObjectWithDmslControl::SetTask({
			GameObjectWithDmslControl::Task::SPEED_CHANGE,
			0,0,
			20,0,
			20,
			1
		});
	}

	if (m_state == STATE0) updateState0();
	else if (m_state == STATE1) updateState1();

	for (auto& p : Game::GetGame().GetEnemyManager()) {
		if (((Enemy::Enemy&)(*p)).GetColliEnabled()) {
			if (Collision::CheckC2C(((Enemy::Enemy&)(*p)).GetCollision(), m_coll)) {
				if (m_state == STATE0)
					((Enemy::Enemy&)(*p)).DecHP(0.1);
				else
					Kill();
				break;
			}
		}
	}

	//杀除碰撞区域内的子弹
	for (auto& layer : Game::GetGame().GetEnemyBulletManager()) {
		for (auto& p : layer) {
			if (((EnemyBullet::Bullet*)p.get())->WhatTypeAmI() == EnemyBullet::Bullet::Type::Grid) continue;
			if (((EnemyBullet::Bullet&)(*p)).GetInvincible()) continue;
			if (((EnemyBullet::Bullet&)(*p)).CollCheckWithCircle(m_coll)) {
				p->Kill();
			}
		}
	}

	Game::GetGame().GetParticleTop().AddDot(15, [this](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
		dot.alpha = 1.0;
		dot.angle = Rand() * 360;
		dot.blend_dst = GL_ONE;
		dot.self_angle = Rand() * 360;
		dot.size = 64.0f * 2.0F;
		dot.speed = Rand() * 2.0;
		dot.r = 0.917647058823529, dot.g = 0.266666666666667, dot.b = 0.83921568627451;
	},
		[this](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
		dot.size -= 2.4;
		dot.self_angle -= 3;
		dot.speed -= 0.01;
		dot.alpha *= 0.97;
	},

		m_coll.x, m_coll.y, 2);

	const int style = 1;
	if (Game::GetGame().GetGameTime() % 2)
		/*switch (rand()%1)
		{
		case 0:*/
		Game::GetGame().GetParticleTop().AddDot(style, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
		dot.alpha = 6.0f;
		dot.angle = double(double(rand()) / RAND_MAX * 2 * M_PI);
		dot.r = 0.929411764705882, dot.g = 0.196078431372549, dot.b = 0.709803921568627;
		dot.self_angle = Rand() * 360 * 2;
		dot.size = 800.0f;
		dot.speed = double(double(rand()) / RAND_MAX * 4.0f);
		//if (rand() % 5 != 0) {
		dot.blend_dst = GL_ONE;
		//}
	}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
		dot.speed -= 0.35f;
		dot.self_angle += 1;
		dot.alpha -= 0.05f * 6;
		dot.size -= 15.0f * 2;
	}, m_coll.x, m_coll.y, 1);

	Game::GetGame().GetParticleTop().AddDot(3, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
		dot.alpha = 6.0f;
		dot.angle = double(double(rand()) / RAND_MAX * 2 * M_PI);
		dot.r = 0, dot.g = 0.196078431372549 * 2, dot.b = 0;
		dot.self_angle = Rand() * 360 * 2;
		dot.size = 1500.0f;
		dot.speed = double(double(rand()) / RAND_MAX * 4.0f);
		//if (rand() % 5 != 0) {
		dot.blend_dst = GL_ONE;
		//}
	}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
		dot.speed -= 0.35f;
		dot.self_angle += 1;
		dot.alpha -= 0.05f * 6;
		dot.size -= 15.0f * 2;
	}, m_coll.x, m_coll.y, 1);
	/*	break;
	case 1:
		Game::GetGame().GetParticleTop().AddDot(style, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
			dot.alpha = 6.0f;
			dot.angle = double(double(rand()) / RAND_MAX * 2 * M_PI);
			dot.r = 0.301960784313725 * 2, dot.g = 0.87843137254902 * 2, dot.b = 0.941176470588235 * 2;
			dot.self_angle = Rand() * M_PI * 2;
			dot.size = 800.0f;
			dot.speed = double(double(rand()) / RAND_MAX * 4.0f);

			//if (rand() % 5 != 0) {
				dot.blend_dst = GL_ONE;
			//}
		}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
			dot.speed -= 0.35f;
			dot.self_angle += 1;
			dot.alpha -= 0.05f * 6;
			dot.size -= 15.0f * 2;
		}, m_coll.x, m_coll.y, 1);
		break;
	case 2:
		Game::GetGame().GetParticleTop().AddDot(style, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
			dot.alpha = 6.0f;
			dot.angle = double(double(rand()) / RAND_MAX * 2 * M_PI);
			dot.r = 0.980392156862745/2.0, dot.g = 0.83921568627451/2.0, dot.b = 0.850980392156863/2.0;
			dot.self_angle = Rand() * M_PI * 2;
			dot.size = 800.0f;
			dot.speed = double(double(rand()) / RAND_MAX * 4.0f);

			//if (rand() % 5 != 0) {
				dot.blend_dst = GL_ONE;
			//}
		}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
			dot.speed -= 0.35f;
			dot.self_angle += 1;
			dot.alpha -= 0.05f * 6;
			dot.size -= 15.0f * 2;
		}, m_coll.x, m_coll.y, 1);
		break;
	}*/
}

void Char::Reimu::ReimuBomb::Draw() const
{
	//Collision::DrawCircle(m_coll);
}

void Char::Reimu::ReimuBomb::Kill()
{
	Basic::Haptic::Play(120);
	++m_searchNewEnemy;
	m_died = true;
	//撞击当前位置的敌人
	m_coll.radius = 230;
	for (auto& p : Game::GetGame().GetEnemyManager()) {
		if (Collision::CheckC2C(((Enemy::Enemy&)(*p)).GetCollision(), m_coll)) {
			((Enemy::Enemy&)(*p)).DecHP(_power);
		}
	}

	//杀死碰撞区域内的子弹
	for (auto& layer : Game::GetGame().GetEnemyBulletManager()) {
		for (auto& p : layer) {
			if (((EnemyBullet::Bullet&)(*p)).GetInvincible()) continue;
			if (((EnemyBullet::Bullet&)(*p)).CollCheckWithCircle(m_coll)) {
				p->Kill();
			}
		}
	}

	Basic::Sound::SeManager::Play("tan00", m_coll.x);

	//添加效果
	Game::GetGame().GetParticleTop().AddDot(3, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
		dot.alpha = 0.8f;
		dot.angle = double(double(rand()) / RAND_MAX * 2 * M_PI);
		dot.r = 1;// double(double(rand()) / RAND_MAX * 0.3f + 0.7f);
		dot.g = double(double(rand()) / RAND_MAX * 0.7f);
		dot.size = 2400.0f;
		dot.b = double(double(rand()) / RAND_MAX * 0.7f);
		dot.speed = double(double(rand()) / RAND_MAX * 128.0f);
		dot.blend_dst = GL_ONE;
	}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
		dot.speed -= 1.0f;
		dot.size -= 60.0f;
	}, m_coll.x, m_coll.y, 64);

	Game::GetGame().GetParticleTop().AddDot(0, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
		dot.alpha = 0.7f;
		dot.angle = 0;
		dot.r = 0.5f;
		dot.g = 0.5f;
		dot.size = 9600.0f;
		dot.b = 0.5f;
		dot.speed = 0;
		dot.blend_dst = GL_ONE;
	}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
		dot.alpha -= 0.01f;
	}, 0, 0);

	Game::GetGame().GetParticleTop().AddDot(15, [this](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
		dot.alpha = 1.0;
		dot.angle = Rand() * 360;
		dot.blend_dst = GL_ONE;
		dot.self_angle = Rand() * 360;
		dot.size = 64.0f * 2.0F;
		dot.speed = Rand() * 4.0;
		dot.r = 0.917647058823529, dot.g = 0.266666666666667, dot.b = 0.83921568627451;
	},
		[this](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
		dot.size -= 2.4;
		dot.self_angle -= 3;
		dot.speed -= 0.01;
		dot.alpha *= 0.95;
	},

		m_coll.x, m_coll.y, 100);
}

void Char::Reimu::ReimuBomb::SetPowerData(double single)
{
	_power = single;
}