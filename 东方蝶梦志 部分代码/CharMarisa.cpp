#include "Precompiled.h"
#include "CharMarisa.h"
#include "Globals.h"
#include "Particle3D.h"
#include "Game.h"
#include "FlashObject.h"
#include "MarisaBomb.h"
#include "BulletMarisa2.h"
#include "KeepingBarrelBlur.h"
#include "ScreenVibrator.h"
#include "MarisaOption.h"
#include "SnowRVReader.h"

using namespace Char::Marisa;
using namespace Basic::Video::Renderer3D;

static const int c_mainGunInterval = 6;		// 主炮射击间隔

static double _mainPower;

Char::Marisa::CharMarisa::~CharMarisa()
{
}

CharMarisa & Char::Marisa::CharMarisa::Get()
{
	static CharMarisa chr;
	return chr;
}

void Char::Marisa::CharMarisa::Reset()
{
	m_bombing = false;
	m_shootTimer = 0;
}

int Char::Marisa::CharMarisa::GetSprite2DImageNum()
{
	return 10;
}

void Char::Marisa::CharMarisa::InitWithSnowRV(const Basic::SnowRVReader & d)
{
	_mainPower = d.GetDouble("MARISA_MAIN");

	Marisa::MarisaOption::SetPowerData(d);
	Marisa::BulletMarisa2::SetPowerData(d.GetDouble("MARISA_SLOWOPTION"));
	Marisa::MarisaBomb::SetPowerData(d.GetDouble("MARISA_BOMB"));
}

double Char::Marisa::CharMarisa::GetPowerTop()
{
	return 5.0;
}

void Char::Marisa::CharMarisa::PowerUp(double now, double add)
{
}

void Char::Marisa::CharMarisa::ShootBegin(double x, double y, bool)
{
	m_shootTimer = 0;
}

#include "BulletReimu.h"
using namespace Char::Reimu;
void Char::Marisa::CharMarisa::ShootUpdate(double power, double x, double y, bool flip)
{
	++m_shootTimer;

	auto &game = Game::GetGame();
	auto &player = game.GetPlayer();
	double px, py;
	player.GetPos(px, py);

	int mul = flip ? -1 : 1;

	if (flip) x -= 100;

	// 发射主炮
	if (m_shootTimer % c_mainGunInterval == 0)
	{
		auto p = new BulletReimu(true);
		p->SetSpeed(25.5);
		p->SetImageNum(6);
		p->SetImageAlpha(1.0);
		p->SetPower(_mainPower);
		if (flip) p->SetAngle(M_PI);
		game.GetPlayerBulletManager().Shoot(p, x, y + 10);
		p = new BulletReimu(true);
		p->SetSpeed(25.5);
		p->SetImageNum(6);
		p->SetImageAlpha(1.0);
		p->SetPower(_mainPower);
		if (flip) p->SetAngle(M_PI);
		game.GetPlayerBulletManager().Shoot(p, x, y - 10);
		Basic::Sound::SeManager::Play("plst00", px, 12);
	}
}

void Char::Marisa::CharMarisa::ShootEnd(bool)
{
}

void Char::Marisa::CharMarisa::Update(double x, double y)
{
}

#include "MarisaOption.h"
std::unique_ptr<Char::BasicOption> Char::Marisa::CharMarisa::CreateOption()
{
	return std::make_unique<Marisa::MarisaOption>();
}

static const int bombTime = 500;	//B时长

void Char::Marisa::CharMarisa::Bomb(bool flip)
{
	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	Basic::Sound::SeManager::Play("nep00", x);

	// 背景变暗
	auto pFlash = new Utils::FlashObject(60, 2, 100, 0, 400, 0.7);
	pFlash->SetDrawOnTop(true);
	pFlash->SetColor(0.0005, 0.0005, 0.05);
	Game::GetGame().GetMiscManagerBottom().Insert(pFlash);

	Game::GetGame().AddAfterEffect(new AfterEffect::KeepingBarrelBlurWithPlayer(140, 320, 100, -0.1, 2, 0));
	Game::GetGame().AddAfterEffect(new AfterEffect::ScreenVibrator(1, 2, 20, 0, 100, 500));

	m_bombing = true;
	Game::GetGame().GetTaskList().AddTask([this]()
	{
		m_bombing = false;
	}
		, bombTime
		);
	Game::GetGame().GetPlayer().Invincible(bombTime + 300);

	Game::GetGame().GetMiscManagerTop().Insert(new MarisaBomb(flip));
}