#include "Precompiled.h"
#include "CharReimu.h"
#include "Game.h"
#include "SeManager.h"
#include "BulletReimu.h"
#include "ImageCacheManager.h"
#include "ReimuBomb.h"
#include "Haptic.h"
#include "ReimuOption.h"

static double _reimuMainPower;

Char::Reimu::CharReimu::~CharReimu()
{
}

Char::Reimu::CharReimu & Char::Reimu::CharReimu::Get()
{
	static CharReimu chr;
	return chr;
}

void Char::Reimu::CharReimu::Reset()
{
	m_bombing = false;
}

int Char::Reimu::CharReimu::GetSprite2DImageNum()
{
	return 0;
}

void Char::Reimu::CharReimu::InitWithSnowRV(const Basic::SnowRVReader & data)
{
	_reimuMainPower = data.GetDouble("REIMU_MAIN");

	Char::Reimu::ReimuOption::SetPowerData(
		data.GetDouble("REIMU_OPTION"),
		data.GetDouble("REIMU_SLOWOPTION")
	);
	Char::Reimu::ReimuBomb::SetPowerData(data.GetDouble("REIMU_BOMB"));
	Char::Reimu::ReimuOption::SetNonSlowPowerAngle(data.GetDouble("REIMU_OPTION_ANGLE"));
}

double Char::Reimu::CharReimu::GetPowerTop()
{
	return 5.0;
}

void Char::Reimu::CharReimu::PowerUp(double now, double add)
{
}

void Char::Reimu::CharReimu::ShootBegin(double x, double y, bool)
{
	m_timer = 0;
}

void Char::Reimu::CharReimu::ShootUpdate(double power, double x, double y, bool flip)
{
	m_timer++;
	if (!(m_timer % 6)) {
		const int mul = flip ? -1 : 1;
		const int offsetX = flip ? -100 : 0;
		auto p = new BulletReimu;
		p->SetSpeed(25.5 * mul);
		p->GetSprite().SetFlip(false, flip);
		p->SetImageNum(0);
		p->SetPower(_reimuMainPower);
		Game::GetGame().GetPlayerBulletManager().Shoot(p, x + offsetX, y + 10);
		p = new BulletReimu;
		p->SetSpeed(25.5 * mul);
		p->GetSprite().SetFlip(false, flip);
		p->SetImageNum(0);
		p->SetPower(_reimuMainPower);
		Game::GetGame().GetPlayerBulletManager().Shoot(p, x + offsetX, y - 10);

		double px, py;
		Game::GetGame().GetPlayer().GetPos(px, py);
		Basic::Sound::SeManager::Play("plst00", px, 12);
	}
}

void Char::Reimu::CharReimu::ShootEnd(bool)
{
}

void Char::Reimu::CharReimu::Update(double x, double y)
{
	auto& em = Game::GetGame().GetEnemyManager();
	auto size = (int)em.Size();
	uint32_t nearestLengthP = UINT32_MAX;
	m_nearestEnemy = -1;
	for (int i = 0; i < size; ++i) {
		if (!em[i].GetColliEnabled()) continue;
		auto c1 = em[i].GetCollision();
		//²â¾à
		uint32_t length = uint32_t((c1.x - x)*(c1.x - x) + (c1.y - y)*(c1.y - y));
		if (length < nearestLengthP) {
			m_nearestEnemy = (unsigned)i;
			nearestLengthP = length;
		}
	}

	double ex = 0, ey = 0;
	if (m_nearestEnemy != -1) em[m_nearestEnemy].GetPos(ex, ey);
	//m_nearestEnemyAngle = atan2(ey - y, ex - x);
}

std::unique_ptr<Char::BasicOption> Char::Reimu::CharReimu::CreateOption()
{
	return std::make_unique<Reimu::ReimuOption>();
}

#include "KeepingBarrelBlur.h"
#include "FlashObject.h"
void Char::Reimu::CharReimu::Bomb(bool)
{
	//Basic::Haptic::PlayEffect("ReimuBombBottom");

	auto pFlash = new Utils::FlashObject(160, 2, 100, 0, 320, 0.35);
	pFlash->SetDrawOnTop(true);
	pFlash->SetColor(0.5, 0.35, 0.35);
	Game::GetGame().GetMiscManagerBottom().Insert(pFlash);
	Game::GetGame().AddAfterEffect(new AfterEffect::KeepingBarrelBlurWithPlayer(140, 320, 100, 0.05, 2, 0));

	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);

	Basic::Sound::SeManager::Play("nep00", x);
	Game::GetGame().GetPlayer().Invincible(800);

	for (int i = 0; i < 6; ++i) {
		//Game::GetGame().GetTaskList().AddTask([i]() {
		auto p = new ReimuBomb(i*60.0f);
		Game::GetGame().GetUILayerGameObjectManager().Insert(p);
		//}, 1 * i);
	}

	m_bombing = true;
	Game::GetGame().GetTaskList().AddTask([this]() {
		this->m_bombing = false;
	}, 400);

	//ÊÖ±úÕð¶¯
	Basic::Haptic::Play(2000);
}

bool Char::Reimu::CharReimu::Bombing()
{
	return ReimuBomb::Bombing() || m_bombing;
}

const char* Char::Reimu::CharReimu::GetName()
{
	return "Reimu";
}