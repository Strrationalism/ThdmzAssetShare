#include "Precompiled.h"
#include "Grid.h"

inline void EnemyBullet::Grid::callDmsl()
{
	m_dmsl->SetAttribute(m_dmslLinkTable.angle, GameObjectWithDmslControl::GetAngle() * 180 / M_PI);
	m_dmsl->SetAttribute(m_dmslLinkTable.speed, GameObjectWithDmslControl::GetSpeed());
	m_dmsl->Call();
	GameObjectWithDmslControl::SetAngle(m_dmsl->GetAttribute(m_dmslLinkTable.angle) / 180 * M_PI);
	GameObjectWithDmslControl::SetSpeed(m_dmsl->GetAttribute(m_dmslLinkTable.speed));
}

void EnemyBullet::Grid::Create(int dmslUnit, int x, int y, const std::shared_ptr<const Dmsl::DmslVarying>& varyingIn)
{
	m_dmsl.emplace(*m_bulletScript, dmslUnit, varyingIn);
	m_dmsl->SetUserData(this);
	GameObjectWithDmslControl::SetPos(x, y);
	callDmsl();
	m_died = false;
	m_visible = false;
}

void EnemyBullet::Grid::Draw() const
{
}

void EnemyBullet::Grid::Update()
{
	callDmsl();
	GameObjectWithDmslControl::Update();
}

void EnemyBullet::Grid::Kill()
{
	m_died = true;
}

void EnemyBullet::Grid::StopDmsl()
{
	m_died = true;
}

EnemyBullet::Bullet::Type EnemyBullet::Grid::WhatTypeAmI() const
{
	return Type::Grid;
}

bool EnemyBullet::Grid::CollCheckWithCircle(const Collision::Circle &) const
{
	return false;
}

void EnemyBullet::Grid::EnableHDR(bool)
{
}

void EnemyBullet::Grid::EnableTail(bool)
{
}

#include "MemoryPool.h"
static MemoryPool<EnemyBullet::Grid, 500> pool2;

void * EnemyBullet::Grid::operator new(size_t)
{
	return pool2.Get();
}

void EnemyBullet::Grid::operator delete(void * ptr)
{
	pool2.Return(ptr);
}