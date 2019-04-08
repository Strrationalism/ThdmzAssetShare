#include "Precompiled.h"
#include "Enemy.h"
#include "EnemyStyleManager.h"
#include "Globals.h"
#include "ItemManager.h"
#include "Game.h"
#include "DreamPointItem.h"
void Enemy::Enemy::Update()
{
	if (m_invicible) --m_invicible;
	if (m_hp <= 0 && !m_died) {
		double x, y;
		GameObjectWithDmslControl::GetPos(x, y);

		if (Game::GetGame().GetItemMgr().GetEnabled()) {
			Game::GetGame().GetItemMgr().AddItem(Item::ItemType::BluePoint, x, y, Game::GetGame().GetInputer().Rand1() * 5 + 5, m_particleWhenKilled.speed / 2);
			Game::GetGame().GetItemMgr().AddItem(Item::ItemType::Power, x, y, 2, m_particleWhenKilled.speed / 2);
		}

		for (auto& p : m_dmslUnit) {
			p.ptr->SetAttribute(m_dmslLinkTable->angle * 180 / M_PI, GameObjectWithDmslControl::GetAngle());
			p.ptr->SetAttribute(m_dmslLinkTable->speed, GameObjectWithDmslControl::GetSpeed());
			if (p.ptr->GetInterfaceExist(m_enemyDiedProgramLink)) {
				p.ptr->Call(m_enemyDiedProgramLink);
			}
		}
		Kill();
		return;
	}
	callDmsl();

	m_dmslUnit.erase(std::remove_if(m_dmslUnit.begin(), m_dmslUnit.end(), [](auto& p) {
		return !p.running;
	}), m_dmslUnit.end());

	GameObjectWithDmslControl::Update();

	double x, y;
	GameObjectWithDmslControl::GetPos(x, y);
	if (m_minLiveOutScreen == 0) {	//超出屏幕则杀死
		if (x >= WIDTH / 2 + m_radius || x <= -WIDTH / 2 - m_radius || y >= HEIGHT / 2 + m_radius || y <= -HEIGHT / 2 - m_radius)
			m_died = true;
	}
	else if (m_minLiveOutScreen > 0) --m_minLiveOutScreen;

	m_collision.x = x;
	m_collision.y = y;

	m_enemyImage.SetPos(x, y);
	m_enemyImage.Update();

	//转脸模式
	double xSpeed = GameObjectWithDmslControl::GetSpeed() * cos(GameObjectWithDmslControl::GetAngle());
	m_enemyImage.SetFlip(false, m_xFilpEnabled && xSpeed > 0.5);

	m_enemyImage.UpdateVertex();

	if (m_colliEnabled && m_stamina) {
		if (Collision::CheckC2C(Game::GetGame().GetPlayer().GetDeathCollision(), m_collision)) {
			Game::GetGame().GetPlayer().Kill();
		}
	}
}

void Enemy::Enemy::Draw() const {
	m_enemyImage.DrawToFastBuffer();
}

void Enemy::Enemy::Create(const StageData::EnemyData & i, bool createdByScript, Enemy* hpLink)
{
	m_dreamX = 1;
	m_stamina = true;
	m_hpLink = hpLink;
	m_fullHP = m_hp = i.hp / 3.0;	//削减HP
	m_invicible = 50;	//出场无敌
	if (i.style >= 0) {
		const auto& style = EnemyStyleManager::GetStyle(i.style);
		m_enemyImage.UseImage("Enemys/" + style.textureName, style.gameFrame_animationFrame, 0);

		m_xFilpEnabled = style.xFilpEnabled;

		m_particleWhenKilled.num = style.particleWhenKilled.num;
		m_particleWhenKilled.speed = style.particleWhenKilled.speed;
		m_particleWhenKilled.r = style.particleWhenKilled.r;
		m_particleWhenKilled.g = style.particleWhenKilled.g;
		m_particleWhenKilled.b = style.particleWhenKilled.b;

		int w, h;
		m_enemyImage.GetSize(w, h);
		m_radius = w > h ? h / 2 : w / 2;
		auto radius = m_radius;
		m_collision.radius = style.collRadius;
	}

	m_died = false;

	m_dmslUnit.push_back({
		true,
		std::make_unique<Dmsl::DmslUnit>(*m_dvm, i.dmslUnit,createdByScript ? Dmsl::DmslUnit::GetRuningUnit()->GetVaryingOut() : i.varying)
	});
	m_dmslUnit.back().ptr->SetUserData((GameObjectWithDmslControl*)this);

	m_minLiveOutScreen = i.minScreenLife;
	GameObjectWithDmslControl::SetPos(i.x, i.y);

	callDmsl();

	double x = i.x;
	double y = i.y;
}

//#include <iostream>
using namespace std;
void Enemy::Enemy::Kill()
{
	m_hpLink = nullptr;
	m_died = true;
	double x, y;
	GameObjectWithDmslControl::GetPos(x, y);
	auto radius = m_radius;

	Basic::Sound::SeManager::Play("enep00", x, 12);

	//Game::GetGame().GetScoreManager().AddDreamPoint(Game::GetGame().GetPlayer().Slow() ? -0.5 : 0.5);	//调试梦秤用

	auto p = m_particleWhenKilled;
	//添加特效粒子 
	Game::GetGame().GetEnemyAnimation().Push(x, y, p.r, p.g, p.b);

	if (x <= WIDTH / 2 + m_radius && x >= -WIDTH / 2 - m_radius && y <= HEIGHT / 2 + m_radius && y >= -HEIGHT / 2 - m_radius) {
		Game::GetGame().GetParticleBottom().AddDot(2, [radius, x, y, p](Basic::Video::Renderer3D::Particle3D::Dot3D& i) {
			i.size *= 8;
			i.angle = double((double(rand()) / RAND_MAX) * 2 * M_PI);
			i.speed = ((double)rand() / RAND_MAX) * p.speed * 0.5;
			i.self_angle = ((double)rand() / RAND_MAX) * 360;
			i.x += ((double)rand() / RAND_MAX) * radius - radius / 3 * 2;
			i.y += ((double)rand() / RAND_MAX) * radius - radius / 3 * 2;
			i.r = p.r;
			i.g = p.g;
			i.b = p.b;
			//i.blend_src = GL_SRC_ALPHA;
			//i.blend_dst = GL_ONE;
		},
			[](auto& i, auto& p) {
			i.size -= 3.4f;
			i.alpha -= 0.02f;
			i.speed -= 0.05f;
			i.self_angle += 2.0f;
		}, x, y, p.num);

		Game::GetGame().GetParticleBottom().AddDot(2, [radius, x, y, p](Basic::Video::Renderer3D::Particle3D::Dot3D& i) {
			i.size *= 8;
			i.angle = double((double(rand()) / RAND_MAX) * 2 * M_PI);
			i.speed = ((double)rand() / RAND_MAX) * p.speed * 0.5;
			i.self_angle = ((double)rand() / RAND_MAX) * 360;
			i.x += ((double)rand() / RAND_MAX) * radius - radius / 3 * 2;
			i.y += ((double)rand() / RAND_MAX) * radius - radius / 3 * 2;
			i.r = p.r;
			i.g = p.g;
			i.b = p.b;
			//i.blend_src = GL_SRC_ALPHA;
			i.blend_dst = GL_ONE;
		},
			[](auto& i, auto& p) {
			i.size -= 3.4f;
			i.alpha -= 0.02f;
			i.speed -= 0.05f;
			i.self_angle += 2.0f;
		}, x, y, p.num / 2);

		radius = 64;
		Game::GetGame().GetParticleTop().AddDot(4, [radius, x, y, p](Basic::Video::Renderer3D::Particle3D::Dot3D& i) {
			i.alpha = 1;
			i.size *= 3;
			i.speed = 0;
			i.x += ((double)rand() / RAND_MAX) * radius - radius / 3 * 2;
			i.y += ((double)rand() / RAND_MAX) * radius - radius / 3 * 2;
			i.r = p.r;
			i.g = p.g;
			i.b = p.b;
			i.blend_dst = GL_ONE;
		},
			[](auto& i, auto& p) {
			i.size += 26;
			i.alpha -= 0.09f;
		}, x, y, 1);
	}
}

void Enemy::Enemy::DecHP(double hp) {
	if (m_hpLink) m_hpLink->DecHP(hp);
	if (m_invicible <= 0) m_hp -= hp;
	double x, y;
	GameObjectWithDmslControl::GetPos(x, y);
	if (m_hp <= 10) {
		Basic::Sound::SeManager::Play("damage00", x, 70);
	}
	else {
		Basic::Sound::SeManager::Play("damage01", y, 50);
	}

	//梦秤处理
	if (!Game::GetGame().GetPlayer().IsInvincible())
	{
		const auto dreamPower = (0.005*hp) * Game::GetGame().GetStageData()->dreamX * m_dreamX;
		Item::DreamPointItem::AddToGame(
			m_collision.x,
			m_collision.y,
			Game::GetGame().GetPlayer().Slow() ? -dreamPower : dreamPower
		);
	}
}

void Enemy::Enemy::CallPrivateUnit(int unit, const std::shared_ptr<const Dmsl::DmslVarying>& varying)
{
	auto p = new Dmsl::DmslUnit(*m_dvm, unit, varying);
	bool running = true;
	m_stopRunning = &running;

	p->SetAttribute(m_dmslLinkTable->angle, GameObjectWithDmslControl::GetAngle() * 180 / M_PI);
	p->SetAttribute(m_dmslLinkTable->speed, GameObjectWithDmslControl::GetSpeed());
	p->SetUserData((GameObjectWithDmslControl*)this);
	p->Call();

	GameObjectWithDmslControl::SetAngle(p->GetAttribute(m_dmslLinkTable->angle) / 180 * M_PI);
	GameObjectWithDmslControl::SetSpeed(p->GetAttribute(m_dmslLinkTable->speed));

	if (running) {
		m_taskList.AddTask([p, this]() {
			m_dmslUnit.push_back({
				true,
				std::unique_ptr<Dmsl::DmslUnit>(p)
			});
		}, 2);
	}
	else {
		delete p;
	}
}
/*
#include "MemoryPool.h"
static MemoryPool<Enemy::Enemy, 20> pool;
void* Enemy::Enemy::operator new(size_t)
{
	return pool.Get();
}

void Enemy::Enemy::operator delete(void* ptr)
{
	pool.Return(ptr);
}*/

//Dmsl库
static double libEnemy_getHP(double*, void* ptr) {
	return ((Enemy::Enemy*)((GameObjectWithDmslControl*)ptr))->GetHP();
}

static void libEnemy_setHP(double* hp, void* ptr) {
	((Enemy::Enemy*)((GameObjectWithDmslControl*)ptr))->SetHP(*hp);
}

static void libEnemy_StopDmsl(double*, void* ptr) {
	((Enemy::Enemy*)((GameObjectWithDmslControl*)ptr))->StopDmsl();
}

static void libEnemy_CallPrivateUnit(double* args, void* ptr) {
	int v = args[0];
	auto varyingOut = Dmsl::DmslUnit::GetRuningUnit()->GetVaryingOut();
	((Enemy::Enemy*)ptr)->CallPrivateUnit(v, varyingOut);
}

static void libEnemy_SetStamina(double* args, void* ptr) {
	((Enemy::Enemy*)ptr)->SetStaminaEnabled(*args > 0);
}

static void libEnemy_Piano(double* arg, void* ptr) {
	std::terminate();
}

void Enemy::Enemy::callDmsl()
{
	for (auto& p : m_dmslUnit) {
		p.ptr->SetAttribute(m_dmslLinkTable->angle, GameObjectWithDmslControl::GetAngle() * 180 / M_PI);
		p.ptr->SetAttribute(m_dmslLinkTable->speed, GameObjectWithDmslControl::GetSpeed());
		m_stopRunning = &p.running;
		p.ptr->Call();
		GameObjectWithDmslControl::SetAngle(p.ptr->GetAttribute(m_dmslLinkTable->angle) / 180 * M_PI);
		GameObjectWithDmslControl::SetSpeed(p.ptr->GetAttribute(m_dmslLinkTable->speed));
	}
}

void Enemy::Enemy::LinkEnemyDmslLib(Dmsl::DmslVirtualMachine & dmsl)
{
	dmsl.LinkCFunc("GetHP", &libEnemy_getHP);
	dmsl.LinkCMet("SetHP", &libEnemy_setHP);
	dmsl.LinkCMet("StopDmsl", &libEnemy_StopDmsl);
	dmsl.LinkCMet("Call", &libEnemy_CallPrivateUnit);
	dmsl.LinkCMet("Piano", &libEnemy_Piano);
	dmsl.LinkCMet("SetStamina", &libEnemy_SetStamina);
}

Enemy::Enemy::Enemy()
{
	m_dvm = Game::GetGame().GetStageData()->enemyScript;
	m_dmslLinkTable = Game::GetGame().GetStageData()->enemyScriptLinkTable;
	m_enemyDiedProgramLink = Game::GetGame().GetStageData()->enemyDiedProgramHandle;
}

Enemy::Enemy::Enemy(const VMContext & c)
{
	m_dvm = c.dvm;
	m_dmslLinkTable = c.table;
	m_enemyDiedProgramLink = c.diedHandle;
	SetVMContext(c);
}

#include "Enemy_14_to_18.h"
#include "Enemy_23_to_25.h"
/*
Enemy::Enemy * Enemy::CreateEnemy(int style)
{
	Enemy* ptr = nullptr;
	switch (style) {
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
		ptr = new SpecialEnemy::Enemy_14_to_18;
		break;
	case 23:
	case 24:
	case 25:
		ptr = new SpecialEnemy::Enemy_23_to_25;
		break;
	default:
		ptr = new Enemy;
		break;
	}
	return ptr;
}*/

Enemy::Enemy * Enemy::CreateEnemy(int style, GameObjectWithDmslControl::VMContext c)
{
	Enemy* ptr = nullptr;

	switch (style) {
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
		ptr = new SpecialEnemy::Enemy_14_to_18(c);
		break;
	case 23:
	case 24:
	case 25:
		ptr = new SpecialEnemy::Enemy_23_to_25(c);
		break;
	default:
		ptr = new Enemy(c);
		break;
	}
	return ptr;
}