#include "Precompiled.h"
#include "MarisaOption.h"
#include <cmath>
#include "Log.h"
#include "BGMPlayer.h"
#include "BulletReimu2.h"
#include "BulletReimu.h"
#include "BulletMarisa2.h"
#include "SnowRVReader.h"
#include "Game.h"

enum class EmitterMode {
	GreenWith2Ways,
	BlueWithShake,
	Blue,
	Off
};

struct OptionSetting {
	double slow_x, slow_y;
	double x, y;
	double power, angle;
	bool have;
	EmitterMode emi;
};



static const OptionSetting optionState[5][4] = {
	{	//Power < 1ʱ
		{ 0,0,0,0,0,0,false,EmitterMode::Off },
		{ 0,0,0,0,0,0,false,EmitterMode::Off },
		{ 0,0,0,0,0,0,false,EmitterMode::Off },
		{ 0,0,0,0,0,0,false,EmitterMode::Off }
	},
	{	//Power >= 1 && Power < 2 ʱ
		{-60,0,60,0,1,0,true,EmitterMode::GreenWith2Ways },
		{ 0,0,0,0,0,0,false,EmitterMode::Off },
		{ 0,0,0,0,0,0,false,EmitterMode::Off },
		{ 0,0,0,0,0,0,false,EmitterMode::Off }
	},
	{	//Power >= 2 && Power < 3 ʱ
		{ -40,-50,60,10,1,5,true,EmitterMode::BlueWithShake },
		{ -40,50,60,-10,1,-5,true,EmitterMode::BlueWithShake },
		{ 0,0,0,0,0,0,false,EmitterMode::Off },
		{ 0,0,0,0,0,0,false,EmitterMode::Off }
	},
	{	//Power >= 3 && Power < 4 ʱ
		{ -40,-50,50,20,1,10,true,EmitterMode::BlueWithShake },
		{ -40,50,50,-20,1,-10,true,EmitterMode::BlueWithShake },
		{ 70,0,60,0,1,0,true,EmitterMode::GreenWith2Ways },
		{ 0,0,0,0,0,0,false,EmitterMode::Off }
	},
	{	//Full Power
		{ 50,50,50,30,1,15,true,EmitterMode::GreenWith2Ways },
		{ 50,-50,50,-30,1,-15,true,EmitterMode::GreenWith2Ways },
		{ -50,-50,60,11,1,-5,true,EmitterMode::BlueWithShake },
		{ -50,50,60,-11,1,5,true,EmitterMode::BlueWithShake }
	}
};

static double _marisaNonSlowPowerBlue, _marisaNonSlowPowerGreen;
static double _marisaNonSlowPowerAngle;

Char::Marisa::MarisaOption::MarisaOption()
{
	for (int i = 0; i < 4; ++i) {
		m_sp[i].UseImage("PlayerOptions", 1);
		m_sp[i].SetZoom(1.5f);
		m_pos[i][0] = 0;
		m_pos[i][1] = 0;
	}
}
void Char::Marisa::MarisaOption::OnUpdate(double x, double y)
{
	m_timer++;
	if (m_timer >= 999) m_timer = 0;
	if (Basic::Sound::BgmPlayer::IsBeatFrame()) {
		m_angleSpeed = 4.5f;
		m_dataActor.Add(m_angleSpeed, 1.5f, 20, 1);
	}

	m_dataActor.Update();

	m_angle += m_angleSpeed;

	for (int i = 0; i < 4; ++i) {
		m_sp.at(i).SetPos(m_pos[i][0] + x, m_pos[i][1] + y);
		m_sp.at(i).SetAngle(m_angle);
		m_sp.at(i).UpdateVertex();
	}

	m_px = x;
	m_py = y;
}

void Char::Marisa::MarisaOption::OnDraw() const
{
	for (int i = 0; i < 4; ++i) {
		if (optionState[m_nowPower][i].have) {
			m_sp.at(i).DrawToFastBuffer();
		}
	}
}

void Char::Marisa::MarisaOption::OnSlowStart()
{
	m_slowMode = true;
	for (int i = 0; i < 4; ++i) {
		if (optionState[m_nowPower][i].have) {
			m_dataActor.Add(m_pos[i][0], optionState[m_nowPower][i].slow_x, 20, 1);
			m_dataActor.Add(m_pos[i][1], optionState[m_nowPower][i].slow_y, 20, 1);
		}
	}
}

void Char::Marisa::MarisaOption::OnSlowStopped()
{
	m_slowMode = false;
	for (int i = 0; i < 4; ++i) {
		if (optionState[m_nowPower][i].have) {
			m_dataActor.Add(m_pos[i][0], optionState[m_nowPower][i].x, 20, 1);
			m_dataActor.Add(m_pos[i][1], optionState[m_nowPower][i].y, 20, 1);
		}
	}
}

void Char::Marisa::MarisaOption::ShootBegin(bool)
{
}

static const double optionDefaultAngle[5][4] = {
	{},
	{0},
	{0.15,-0.15},
	{0.18,-0.18,0},
	{0.2,-0.2,-0.1,0.1},
};

void Char::Marisa::MarisaOption::ShootUpdate(bool flip)
{
	if (!(m_timer % 9)) {
		const double offsets[2] = { -_marisaNonSlowPowerAngle,_marisaNonSlowPowerAngle };
		for (int shootTime = 0; shootTime < 2; ++shootTime) {
			double offset = offsets[shootTime];
			for (int i = 0; i < 4; ++i) {
				if (optionState[m_nowPower][i].have) {
					const int mul = flip ? -1 : 1;
					const int offsetX = flip ? -80 : 0;
					if (m_slowMode) {
						auto& partten = optionState[m_nowPower][i];
						switch (partten.emi)
						{
						case EmitterMode::GreenWith2Ways:
						{
							auto p = new Reimu::BulletReimu(true);
							p->SetSpeed(mul*28.5);
							p->GetSprite().SetFlip(false, flip);
							p->SetAngle(0.1 + 0.15 * sin(Game::GetGame().GetGameTime() / 50.0));
							p->SetPower(_marisaNonSlowPowerGreen);
							p->SetImageNum(3);
							Game::GetGame().GetPlayerBulletManager().Shoot(p, partten.x + m_px, partten.y + m_py);

							p = new Reimu::BulletReimu(true);
							p->SetSpeed(mul*28.5);
							p->GetSprite().SetFlip(false, flip);
							p->SetAngle(-0.1 - 0.15 * sin(Game::GetGame().GetGameTime() / 50.0));
							p->SetPower(_marisaNonSlowPowerGreen);
							p->SetImageNum(3);
							Game::GetGame().GetPlayerBulletManager().Shoot(p, partten.x + m_px, partten.y + m_py);
							break;
						}
							break;
						case EmitterMode::BlueWithShake:
						{
							auto p = new Reimu::BulletReimu(true);
							p->SetSpeed(mul*28.5);
							p->GetSprite().SetFlip(false, flip);
							p->SetAngle(((partten.y > 0) ? -1 : 1) * 0.3 * sin(Game::GetGame().GetGameTime() / 50.0));
							p->SetPower(_marisaNonSlowPowerBlue);
							p->SetImageNum(2);
							Game::GetGame().GetPlayerBulletManager().Shoot(p, m_pos[i][0] + m_px + offsetX, m_pos[i][1] + m_py);
							break;
						}
							break;
						default:
							Crash(L"Bad Marisa shoot partten.");
							break;
						}
					}
					else {
						auto p = new Marisa::BulletMarisa2;
						p->SetSpeed(mul*28.5);
						p->GetSprite().SetFlip(false, flip);
						//p->SetPower(_marisaOptionSubPowerSlow);
						//p->SetImageNum(2);
						Game::GetGame().GetPlayerBulletManager().Shoot(p, mul*m_pos[i][0] + m_px + offsetX, m_pos[i][1] + m_py);
					}
				}
			}
		}
	}
}

void Char::Marisa::MarisaOption::ShootEnd(bool)
{
}

void Char::Marisa::MarisaOption::SetPower(int power)
{
	for (int i = m_nowPower; i < 4; ++i) {
		m_pos[i][0] = m_pos[i][1] = 0;
	}
	m_nowPower = std::clamp(power,0,4);
	if (m_slowMode) {
		OnSlowStart();
	}
	else {
		OnSlowStopped();
	}
}




void Char::Marisa::MarisaOption::SetPowerData(const Basic::SnowRVReader & rvReader)
{
	Marisa::BulletMarisa2::SetPowerData(rvReader.GetDouble("MARISA_SLOW_SINGLE_POWER"));
	_marisaNonSlowPowerBlue = rvReader.GetDouble("MARISA_FAST_POWER_BLUE");
	_marisaNonSlowPowerGreen = rvReader.GetDouble("MARISA_FAST_POWER_GREEN");
}
