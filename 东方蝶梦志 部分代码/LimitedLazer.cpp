#include "Precompiled.h"
#include "LimitedLazer.h"
#include "Collision.h"

void EnemyBullet::LimitedLazer::callDmsl()
{
	if (m_dmslKilled) return;
	m_dmslUnit->SetAttribute(m_dmslLinkTable.angle, GameObjectWithDmslControl::GetAngle() * 180 / M_PI);
	m_dmslUnit->SetAttribute(m_dmslLinkTable.speed, GetSpeed());
	m_dmslUnit->Call();
}

void EnemyBullet::LimitedLazer::updateData()
{
	double x, y;
	GetPos(x, y);

	double angle = (GameObjectWithDmslControl::GetAngle()) * 180 / M_PI;
	m_sp.SetAngle(angle + 90);

	switch (m_birthState)
	{
	case BIRTH: {
		double speed = GetSpeed();
		m_nowSpLength = (speed + m_nowSpLength);
		if (m_nowSpLength >= m_length) {
			m_nowSpLength = m_length;
			m_birthState = LIVING;
			m_dataActor.Add(m_spShooterAlpha, 0, 20, 1);
		}

		m_sp.SetSize(m_width, m_nowSpLength);
		m_sp.SetCenter(0, m_nowSpLength / 2 * m_rotateCenter);
		m_sp.SetPos(x, y);
	}
				break;
	case LIVING:
		GameObjectWithDmslControl::Update();
		m_nowSpLength = m_length;
		m_sp.SetPos(x, y);

		m_sp.SetSize(m_width, m_nowSpLength);
		m_sp.SetCenter(0, m_nowSpLength / 2 * m_rotateCenter);
		break;
	case GODIE:
		m_sp.SetSize(m_width, m_nowSpLength);
		m_sp.SetCenter(0, m_nowSpLength / 2 * m_rotateCenter);
		break;
	}

	callDmsl();
	GameObjectWithDmslControl::SetAngle(m_dmslUnit->GetAttribute(m_dmslLinkTable.angle)*M_PI / 180);
	GameObjectWithDmslControl::SetSpeed(m_dmslUnit->GetAttribute(m_dmslLinkTable.speed));

	m_sp.UpdateVertex();
}

void EnemyBullet::LimitedLazer::Create(double x, double y, int style, bool invalid, int dmslUnit, const std::shared_ptr<const Dmsl::DmslVarying>& varyingIn, int live)
{
	m_died = false;

	m_showShooter = true;

	m_hdrEnabled = true;

	m_sp.UseImage("Bullets/LimitedLazers", style);
	m_spShooter.UseImage("Bullets/LimitedLazersShooter", style);
	m_spShooter.SetPos(x, y);
	m_spShooter.SetAlpha(0);
	m_dataActor.Add(m_spShooterAlpha, 1.0, 15, 1);
	m_spShooter.UpdateVertex();
	SetPos(x, y);
	//m_invailed = invalid;	//TODO:如果出现问题则改为m_invicible

	m_width = 21;

	m_dmslUnit.emplace(*Bullet::m_bulletScript, dmslUnit, varyingIn);
	m_dmslUnit->SetUserData(this);

	m_nowSpLength = 0;
	m_length = 482;

	m_rotateCenter = 1;

	m_birthState = BIRTH;

	m_grazed = false;

	m_taskList.AddTask([this]() {
		Kill();
	}, live);

	Update();
}

#include "Game.h"
void EnemyBullet::LimitedLazer::Update()
{
	updateData();
	m_dataActor.Update();
	m_taskList.Update();
	m_spShooter.SetAlpha(m_spShooterAlpha);

	//计算碰撞体
	/*double coll[4];

	const auto& vcs = m_sp.GetDrawVerts();
	coll[0] = (vcs[0] + vcs[2]) / 2;
	coll[1] = (vcs[1] + vcs[3]) / 2;
	coll[2] = (vcs[4] + vcs[6]) / 2;
	coll[3] = (vcs[5] + vcs[7]) / 2;*/

	//碰撞检测
	if (m_width >= 10) {
		auto death = Game::GetGame().GetPlayer().GetDeathCollision();
		death.radius /= 4.0;
		if (Collision::VR2C(m_sp.GetDrawVerts(), death)) {
			if (Game::GetGame().GetPlayer().IsInvincible()) {
				if(!m_invincible)
					Kill();
			}
			else {
				Game::GetGame().GetPlayer().Kill();
			}
		}

		else if (!m_grazed) {
			if (VR2C(m_sp.GetDrawVerts(), Game::GetGame().GetPlayer().GetGrazeCollision())) {
				m_grazed = true;
				Game::GetGame().GetPlayer().OnGraze();
			}
		}
	}
}

void EnemyBullet::LimitedLazer::Draw() const
{
	m_sp.DrawToFastBuffer(m_hdrEnabled ? Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR : Basic::Video::Renderer2D::FASTBUFFER_BLEND_NORMAL);
	if (m_spShooterAlpha > 0 && m_showShooter) m_spShooter.DrawToFastBuffer(m_hdrEnabled ? Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR : Basic::Video::Renderer2D::FASTBUFFER_BLEND_NORMAL);
	/*m_sp.OldDraw();
	double coll[4];
	glDisable(GL_TEXTURE_2D);
	glColor4f(0, 0, 0, 1);
	glBegin(GL_LINES);
	const auto& vcs = m_sp.GetDrawVerts();
	coll[0] = (vcs[0] + vcs[2]) / 2;
	coll[1] = (vcs[1] + vcs[3]) / 2;
	coll[2] = (vcs[4] + vcs[6]) / 2;
	coll[3] = (vcs[5] + vcs[7]) / 2;
	glVertex2dv(coll);
	glVertex2dv(coll + 2);
	glEnd();
	glEnable(GL_TEXTURE_2D);*/
}

void EnemyBullet::LimitedLazer::Kill()
{
	StopDmsl();
	m_birthState = GODIE;

	m_nowSpLength = -m_nowSpLength;
	m_sp.SetSize(m_width, m_nowSpLength);
	m_sp.SetCenter(0, m_nowSpLength / 2);

	m_dataActor.Add(m_nowSpLength, 0, 20, 1);
	m_taskList.AddTask([this]() {
		m_died = true;
	}, 20);

	double x, y;
	GetPos(x, y);

	double coll[2];
	const auto& vcs = m_sp.GetDrawVerts();
	coll[0] = (vcs[0] + vcs[2]) / 2;
	coll[1] = (vcs[1] + vcs[3]) / 2;

	m_sp.SetPos(coll[0], coll[1]);
	m_sp.UpdateVertex();
}

void EnemyBullet::LimitedLazer::SetRotateCenter(double center)
{
	m_rotateCenter = center;
}

void EnemyBullet::LimitedLazer::SetLength(double length)
{
	m_length = length;
}

void EnemyBullet::LimitedLazer::ChangeLength(double end, int time, int func)
{
	m_dataActor.Add(m_length, end, time, func);
}

void EnemyBullet::LimitedLazer::SetWidth(double width)
{
	m_width = width;
}

void EnemyBullet::LimitedLazer::ChangeWidth(double end, int time, int func)
{
	m_dataActor.Add(m_width, end, time, func);
}

void EnemyBullet::LimitedLazer::StopDmsl()
{
	m_dmslKilled = true;
}

EnemyBullet::Bullet::Type EnemyBullet::LimitedLazer::WhatTypeAmI() const
{
	return Type::LimitedLazer;
}

bool EnemyBullet::LimitedLazer::CollCheckWithCircle(const Collision::Circle & cir) const
{
	return Collision::VR2C(m_sp.GetDrawVerts(), cir);
}

void EnemyBullet::LimitedLazer::EnableHDR(bool b)
{
	m_hdrEnabled = b;
}

void EnemyBullet::LimitedLazer::EnableTail(bool)
{
	//无效化
}

#include "MemoryPool.h"
static MemoryPool<EnemyBullet::LimitedLazer, 200> pool2;

void * EnemyBullet::LimitedLazer::operator new(size_t)
{
	return pool2.Get();
}

void EnemyBullet::LimitedLazer::operator delete(void * ptr)
{
	pool2.Return(ptr);
}