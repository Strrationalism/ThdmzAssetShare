#include "Precompiled.h"
#include "CircleBullet.h"
#include "BulletStyleManager.h"
#include "Game.h"
#include "Collision.h"
#include "Globals.h"
#include "Shaders.h"
#include "MemoryPool.h"

static MemoryPool<EnemyBullet::CircleBullet, 1000> pool;
void*  EnemyBullet::CircleBullet::operator new(size_t)
{
	return pool.Get();
}

void EnemyBullet::CircleBullet::operator delete(void* ptr)
{
	pool.Return(ptr);
}

void EnemyBullet::CircleBullet::addATail(double angle) const
{
	if (m_alpha > 0) {
		auto p = new CircleBulletTail(m_style, m_subStyle, m_size, m_collision.x, m_collision.y, m_r, m_g, m_b, angle * 180 / double(M_PI) + 90, m_alpha / 4, m_hdrEnabled);
		Game::GetGame().GetEnemyBulletManager().Insert(p);
	}
}

void EnemyBullet::CircleBullet::callDmsl()
{
	if (m_dmslUsing) {
		m_dmslUnit->SetAttribute(m_dmslLinkTable.angle, GameObjectWithDmslControl::GetAngle() * 180 / M_PI);
		m_dmslUnit->SetAttribute(m_dmslLinkTable.speed, GameObjectWithDmslControl::GetSpeed());
		m_dmslUnit->Call();
		GameObjectWithDmslControl::SetAngle(m_dmslUnit->GetAttribute(m_dmslLinkTable.angle) / 180 * M_PI);
		GameObjectWithDmslControl::SetSpeed(m_dmslUnit->GetAttribute(m_dmslLinkTable.speed));
		m_bulletImage.SetAngle(
			(
				m_imageAngleMode == UseAngle ?
				m_dmslUnit->GetAttribute(m_dmslLinkTable.angle) * M_PI / 180:
				m_imageAngle * M_PI / 180
				
				)
			* 180 / double(M_PI) + 90
		);
		if (!m_dmslUsing)
			m_dmslUnit.reset();
	}
}

EnemyBullet::CircleBullet::~CircleBullet()
{
}

void EnemyBullet::CircleBullet::StopDmsl()
{
	m_dmslUsing = false;
}

void EnemyBullet::CircleBullet::Update()
{
	if (m_animated) m_bulletImage.Update();
	callDmsl();
	GameObjectWithDmslControl::Update();

	double x, y;
	GameObjectWithDmslControl::GetPos(x, y);
	if (m_minLiveOutScreen <= 0) {	//超出屏幕则杀死
		if (x >= WIDTH / 2 + m_radius*m_size || x <= -WIDTH / 2 - m_radius*m_size || y >= HEIGHT / 2 + m_radius*m_size || y <= -HEIGHT / 2 - m_radius*m_size) {
			m_died = true;
		}
	}
	else --m_minLiveOutScreen;

	m_bulletImage.SetAlpha(m_alpha);
	m_bulletImage.SetZoom(m_size);
	m_bulletImage.SetPos(x, y);
	m_bulletImage.UpdateVertex();
	m_collision.x = x;
	m_collision.y = y;
	m_collision.radius = m_collRadius*m_size;

	//擦弹检查
	if (m_goDie == -1) {
		if ((m_alpha > 0.5 || m_forcedColli) && m_size > 0.1) {
			if (Collision::CheckC2C(Game::GetGame().GetPlayer().GetGrazeCollision(), m_collision)) {
				if (Collision::CheckC2C(Game::GetGame().GetPlayer().GetDeathCollision(), m_collision)) {
					if (Game::GetGame().GetPlayer().IsInvincible() && !m_invincible)
						Kill();
					else
						Game::GetGame().GetPlayer().Kill();
				}
				else if (m_nograze) {
					Game::GetGame().GetPlayer().OnGraze();
					m_nograze = false;
				}
			}
		}
	}
	else if (m_goDie > 0) {
		double progress = double(30 - m_goDie) / m_goDie;
		m_bulletImage.SetZoom(m_size*(progress)+m_size);
		m_bulletImage.SetAlpha(1 - progress);
		--m_goDie;
	}
	else {
		m_died = true;
	}

	if (m_imageAngleMode == AutoRotate)
		m_imageAngle += 4;

	if (m_tailEnabled) {
		if (Game::GetGame().GetGameTime() % 4)
			addATail(m_imageAngleMode == UseAngle ? GameObjectWithDmslControl::GetAngle() : m_imageAngle);
	}

	m_bulletImage.SetColorFliter(m_r, m_g, m_b);
}

void EnemyBullet::CircleBullet::Draw() const
{
	if (m_alpha > 0) {
		m_bulletImage.DrawToFastBuffer(m_hdrEnabled ? Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR : Basic::Video::Renderer2D::FASTBUFFER_BLEND_NORMAL);
	}
}

void EnemyBullet::CircleBullet::Kill()
{
	if (m_goDie != -1) return;
	m_goDie = 30;
	m_tailEnabled = false;

	if (m_dmslUsing)
	{
		if (m_dmslUnit->GetInterfaceExist(m_dmslLinkTable.interfaceDied.value()))
		{
			m_dmslUnit->Call(m_dmslLinkTable.interfaceDied.value());
		}
	}
}

void EnemyBullet::CircleBullet::Create(double x, double y, int style, int subStyle, int dmslUnit, int minLiveOutScreen, bool invalid, const std::shared_ptr<const Dmsl::DmslVarying>& varyingIn = nullptr)
{
	GameObjectWithDmslControl::SetPos(x, y);
	m_nograze = true;

	m_style = style;
	m_subStyle = subStyle;

	m_imageAngleMode = UseAngle;

	//设置m_bulletImage
	const auto& styleStruct = EnemyBullet::BulletStyleManager::GetMgr().GetStyle(style);
	m_animated = styleStruct.animated;
	if (m_animated) {
		//加载动画
		m_bulletImage.UseImage("Bullets/" + styleStruct.textureName, 4, 0, -1);
	}
	else {
		//无动画的子弹
		m_bulletImage.UseImage("Bullets/" + styleStruct.textureName, subStyle);
	}

	int w, h;
	m_bulletImage.GetSize(w, h);
	m_radius = w > h ? h : w;

	//创建DmslUnit
	m_dmslUnit.emplace(*Bullet::m_bulletScript, dmslUnit, varyingIn);
	m_dmslUnit->SetUserData((GameObjectWithDmslControl*)this);
	m_dmslUsing = true;

	//状态初始化
	//m_invailed = invalid;	//TODO:如果出现问题则改为m_invicible
	m_died = false;

	//创建动画
	m_size = 3 * styleStruct.initSize;
	m_alpha = 0;
	m_dataActor.Add(m_size, styleStruct.initSize, 20, 1);
	m_dataActor.Add(m_alpha, 1.0, 20, 1);

	m_collRadius = styleStruct.collArg1;
	m_collision.radius = styleStruct.collArg1*m_size;
	m_collision.x = x;
	m_collision.y = y;

	callDmsl();

	m_minLiveOutScreen = minLiveOutScreen;

	//用于对象复用的初始化
	/*m_goDie = -1;
	m_r = m_g = m_b = 1;
	m_hdrEnabled = false;
	m_tailEnabled = false;
	m_imageAngleEnabled = false;
	m_imageAngle = 0;
	m_invincible = false;*/
}

void EnemyBullet::CircleBullet::ChangeStyle(int style, int subStyle)
{
	m_style = style;
	m_subStyle = subStyle;

	//设置m_bulletImage
	const auto& styleStruct = EnemyBullet::BulletStyleManager::GetMgr().GetStyle(style);
	m_animated = styleStruct.animated;
	if (m_animated) {
		//加载动画
		m_bulletImage.UseImage("Bullets/" + styleStruct.textureName, 2, 0, -1);
	}
	else {
		//无动画的子弹
		m_bulletImage.UseImage("Bullets/" + styleStruct.textureName, subStyle);
	}

	int w, h;
	m_bulletImage.GetSize(w, h);
	m_radius = w > h ? h : w;

	//创建动画
	m_size = 3 * styleStruct.initSize;
	m_alpha = 0;
	m_dataActor.Add(m_size, styleStruct.initSize, 20, 1);
	m_dataActor.Add(m_alpha, 1.0, 20, 1);

	m_collRadius = styleStruct.collArg1;
	m_collision.radius = styleStruct.collArg1*m_size;
}

void EnemyBullet::CircleBullet::EnableHDR(bool b)
{
	m_hdrEnabled = b;
}

void EnemyBullet::CircleBullet::EnableTail(bool b)
{
	m_tailEnabled = b;
	m_drawOnTop = b;
}

bool EnemyBullet::CircleBullet::CollCheckWithCircle(const Collision::Circle & c) const
{
	return Collision::CheckC2C(m_collision, c);
}

EnemyBullet::Bullet::Type EnemyBullet::CircleBullet::WhatTypeAmI() const
{
	return Bullet::Type::Circle;
}

void EnemyBullet::CircleBulletTail::StopDmsl()
{
}

void EnemyBullet::CircleBulletTail::Update()
{
	m_alpha *= 0.92;
	m_image.SetAlpha(m_alpha);

	if (m_alpha <= 0.1) m_died = true;
}

void EnemyBullet::CircleBulletTail::Draw() const
{
	m_image.DrawToFastBuffer(m_hdrEnabled ? Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR : Basic::Video::Renderer2D::FASTBUFFER_BLEND_NORMAL);
}

void EnemyBullet::CircleBulletTail::Kill()
{
	//m_died = true;
}

void EnemyBullet::CircleBulletTail::EnableHDR(bool b)
{
	m_hdrEnabled = b;
}

void EnemyBullet::CircleBulletTail::EnableTail(bool)
{
}

bool EnemyBullet::CircleBulletTail::CollCheckWithCircle(const Collision::Circle &) const
{
	return false;
}

EnemyBullet::Bullet::Type EnemyBullet::CircleBulletTail::WhatTypeAmI() const
{
	return Bullet::Type::CircleTail;
}

EnemyBullet::CircleBulletTail::CircleBulletTail(int style, int subStyle, double size, double x, double y, double r, double g, double b, double angle, double alpha, bool hdr)
{
	m_died = false;
	//设置m_bulletImage
	const auto& styleStruct = EnemyBullet::BulletStyleManager::GetMgr().GetStyle(style);
	m_image.UseImage("Bullets/" + styleStruct.textureName, subStyle);

	m_image.SetAngle(angle);

	m_image.SetPos(x, y);
	m_image.SetZoom(size);
	m_image.SetAlpha(m_alpha = alpha);

	m_image.SetColorFliter(r, g, b);

	m_image.UpdateVertex();

	m_hdrEnabled = hdr;
}

static MemoryPool<EnemyBullet::CircleBulletTail, 1000> pool2;

void * EnemyBullet::CircleBulletTail::operator new(size_t)
{
	return pool2.Get();
}

void EnemyBullet::CircleBulletTail::operator delete(void * ptr)
{
	pool2.Return(ptr);
}