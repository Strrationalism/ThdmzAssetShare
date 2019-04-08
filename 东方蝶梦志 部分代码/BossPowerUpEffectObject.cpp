#include "Precompiled.h"
#include "BossPowerUpEffectObject.h"
#include "Game.h"

void Boss::BossPowerUpEffectObject::Update()
{
	--m_effectPowerUp;

	if (m_followPlayer)
		Game::GetGame().GetPlayer().GetPos(m_x, m_y);

	if (m_effectPowerUp <= 0)
		Kill();

	if (m_effectPowerUp == 1) {
		Basic::Sound::SeManager::Play("tan00", m_x);
		Game::GetGame().GetParticleTop().AddDot(2, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
			dot.size = 500 * Rand();

			dot.r = Rand() * 0.9;
			dot.g = Rand() * 0.9;
			dot.b = Rand() * 0.9;

			dot.speed = 17.5 * Rand();
			dot.angle = 2 * M_PI * Rand();
			dot.self_angle = 360 * Rand();

			dot.blend_dst = GL_ONE;
		}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
			dot.alpha -= 0.025;
			dot.self_angle -= 5;
		}, m_x, m_y, 40);

		Game::GetGame().GetParticleTop().AddDot(3, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
			dot.size = 500 * Rand();
			dot.alpha = 0.5 * Rand();

			dot.r = Rand() * 0.2;
			dot.g = Rand() * 0.2;
			dot.b = Rand() * 0.2;

			dot.speed = 35 * Rand();
			dot.angle = 2 * M_PI * Rand();

			dot.blend_dst = GL_ONE;
		}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
			dot.alpha -= 0.025;
		}, m_x, m_y, 100);
	}
	else if (m_effectPowerUp >= 40) {
		Game::GetGame().GetParticleBottom().AddDot(3, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
			dot.alpha = 0.1;
			dot.angle = 0;
			dot.speed = 0;

			dot.r = Rand() * 0.2;
			dot.g = Rand() * 0.2;
			dot.b = Rand() * 0.2;

			dot.userdata[0] = dot.x;	//圆心x位置
			dot.userdata[1] = dot.y;	//圆心y位置
			dot.userdata[2] = Rand() * 500;	//圆半径
			dot.userdata[3] = Rand() * M_PI * 2;	//与圆心角度
			dot.self_angle = 0;	//状态

			dot.size = 1000 * Rand();

			dot.blend_dst = GL_ONE;
		},
			[](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
			if (dot.self_angle == 1)  dot.alpha -= 0.015;
			else dot.alpha += 0.05;
			if (dot.alpha >= 1.0f) dot.self_angle = 1;
			if (dot.userdata[2] > 15) dot.userdata[2] -= 15;
			else dot.userdata[2] = 0;
			dot.userdata[3] += 0.05;

			dot.x = dot.userdata[2] * cos(dot.userdata[3]) + dot.userdata[0];
			dot.y = dot.userdata[2] * sin(dot.userdata[3]) + dot.userdata[1];

			dot.size -= 15;
		}, m_x, m_y, 15);

		Game::GetGame().GetParticleTop().AddDot(2, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
			dot.alpha = 0.1;
			dot.angle = 0;
			dot.speed = 0;

			dot.r = Rand() * 0.9;
			dot.g = Rand() * 0.9;
			dot.b = Rand() * 0.9;

			dot.userdata[0] = dot.x;	//圆心x位置
			dot.userdata[1] = dot.y;	//圆心y位置
			dot.userdata[2] = Rand() * 300;	//圆半径
			dot.userdata[3] = Rand() * M_PI * 2;	//与圆心角度
			dot.self_angle = 360 * Rand();

			dot.size = 500 * Rand();

			dot.blend_dst = GL_ONE;
		},
			[](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
			dot.alpha += 0.05;
			dot.self_angle += 5;
			if (dot.userdata[2] > 7.5) dot.userdata[2] -= 7.5;
			else dot.userdata[2] = 0;
			//dot.userdata[3] += 0.05;

			dot.x = dot.userdata[2] * cos(dot.userdata[3]) + dot.userdata[0];
			dot.y = dot.userdata[2] * sin(dot.userdata[3]) + dot.userdata[1];

			dot.size -= 25;
		}, m_x, m_y, 7);
	}
	
}

Boss::BossPowerUpEffectObject::BossPowerUpEffectObject()
{
	m_followPlayer = true;
	m_died = false;
}

Boss::BossPowerUpEffectObject::BossPowerUpEffectObject(double x, double y):
	m_x(x),
	m_y(y)
{
	m_died = false;
	m_followPlayer = false;
	Basic::Sound::SeManager::Play("ch00", x);
}

void Boss::BossPowerUpEffectObject::Draw() const
{
}

void Boss::BossPowerUpEffectObject::Kill()
{
	m_died = true;
}