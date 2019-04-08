#include "Precompiled.h"
#include "BulletReimu.h"
#include "Globals.h"
#include "SeManager.h"
#include "EnemyManager.h"
#include "Game.h"
#include <vector>
#include "Sprite2D.h"

Char::Reimu::BulletReimu::BulletReimu(bool marisaMode)
{
	m_marisaMode = marisaMode;
}

void Char::Reimu::BulletReimu::Update()
{
	double x, y;
	GameObjectWithDmslControl::GetPos(x, y);
	if (x >= WIDTH / 2 + 13) m_died = true;
	GameObjectWithDmslControl::Update();
	m_sprite.SetPos(x, y);
	m_sprite.SetAngle(GetAngle() * 180 / M_PI);
	m_sprite.SetAlpha(m_powerX1dot3 ? 2 : 1);
	m_sprite.SetZoom(m_powerX1dot3 ? 1.3 : 1);

	m_sprite.UpdateVertex();

	m_powerX1dot3 =
		Game::GetGame().GetPlayer().GetSheild().has_value() ?
		Game::GetGame().GetPlayer().GetSheild()->IsShootPowerX1dot3() :
		false;

}

void Char::Reimu::BulletReimu::Draw() const
{
	m_sprite.DrawToFastBuffer(
		m_powerX1dot3 ? 
		Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR : 
		Basic::Video::Renderer2D::FASTBUFFER_BLEND_NORMAL
	);
}

void Char::Reimu::BulletReimu::Kill()
{
	m_died = true;
}

void Char::Reimu::BulletReimu::Create(double x, double y)
{
	m_sprite.UseImage("PlayerBullet", m_imageNum);
	m_sprite.SetAlpha(m_imageAlpha);
	m_died = false;
	//GameObjectWithDmslControl::SetAngle(0);
	GameObjectWithDmslControl::SetPos(x + 54, y);
	SetRadius(8);
}

void Char::Reimu::BulletReimu::SetImageNum(int num)
{
	//m_sprite.UseImage("PlayerBullet", num);
	m_imageNum = num;
}

void Char::Reimu::BulletReimu::SetImageAlpha(double alpha)
{
	m_imageAlpha = alpha;
	//m_sprite.SetAlpha(alpha);
}

void Char::Reimu::BulletReimu::OnColli(Enemy::Enemy & enemy)
{
	Kill();

	bool marisa = m_marisaMode;
	double x, y;
	enemy.GetPos(x, y);
	Game::GetGame().GetParticleTop().AddDot(3,
		[marisa](Basic::Video::Renderer3D::Particle3D::Dot3D& d) {
		d.size *= 16;
		d.angle = M_PI;
		d.alpha = 0.6f;
		d.speed = 2.5;
		d.x -= double(Rand()) * 40;
		//d.y += double(rand()) / RAND_MAX * 10 - 5;
		d.y += 30;
		d.r = marisa ? 0.352941176470588 : (0XFA / 256.0f);
		d.g = marisa ? 0.87843137254902 : (0X5E / 256.0f);
		d.b = marisa ? 0.972549019607843 : (0X0A / 256.0f);

		d.blend_dst = GL_ONE;
	},
		[](Basic::Video::Renderer3D::Particle3D::Dot3D& d, Basic::Video::Renderer3D::Particle3D& i) {
		d.speed += 0.05f * double(rand()) / RAND_MAX;
		d.size -= 0.04;
		d.alpha -= 0.03;
		d.g += 0.02;
	}, x, y, 1);

	enemy.DecHP(m_powerX1dot3 ? m_power * 1.1 : m_power);
}

void Char::Reimu::BulletReimu::SetAngle(double a)
{
	GameObjectWithDmslControl::SetAngle(a);
	m_sprite.SetAngle(a);
}

#include "MemoryPool.h"

static MemoryPool<Char::Reimu::BulletReimu, 25> pool;
void Char::Reimu::BulletReimu::operator delete(void* ptr)
{
	pool.Return(ptr);
}

void* Char::Reimu::BulletReimu::operator new(size_t)
{
	return pool.Get();
}