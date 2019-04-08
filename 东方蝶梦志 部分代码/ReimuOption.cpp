#include "Precompiled.h"
#include "ReimuOption.h"
#include "Log.h"
#include "BGMPlayer.h"
#include "BulletReimu.h"
#include "BulletReimu2.h"
#include "Game.h"
struct OptionSetting {
	double slow_x, slow_y;
	double x, y;

	double power, angle;
	bool have;
};

static const OptionSetting optionState[5][4] = {
	{	//Power < 1ʱ
		{ 0,0,0,0,0,0,false },
		{ 0,0,0,0,0,0,false },
		{ 0,0,0,0,0,0,false },
		{ 0,0,0,0,0,0,false }
	},
	{	//Power >= 1 && Power < 2 ʱ
		{-60,0,60,0,1,0,true},
		{ 0,0,0,0,0,0,false },
		{ 0,0,0,0,0,0,false },
		{ 0,0,0,0,0,0,false }
	},
	{	//Power >= 2 && Power < 3 ʱ
		{ -50,20,60,10,1,5,true },
		{ -50,-20,60,-10,1,-5,true },
		{ 0,0,0,0,0,0,false },
		{ 0,0,0,0,0,0,false }
	},
	{	//Power >= 3 && Power < 4 ʱ
		{ -10,55,50,20,1,10,true },
		{ -10,-55,50,-20,1,-10,true },
		{ -60,0,60,0,1,0,true },
		{ 0,0,0,0,0,false }
	},
	{	//Full Power
		{ -20,40,50,30,1,15,true },
		{ -20,-40,50,-30,1,-15,true },
		{ -60,-20,60,11,1,-5,true },
		{ -60,20,60,-11,1,5,true }
	}
};

static double _reimuOptionSubPower, _reimuOptionSubPowerSlow;
static double _reimuNonSlowPowerAngle;

Char::Reimu::ReimuOption::ReimuOption()
{
	for (int i = 0; i < 4; ++i) {
		m_sp[i].UseImage("PlayerOptions", 0);
		m_sp[i].SetZoom(1.5f);
		m_pos[i][0] = 0;
		m_pos[i][1] = 0;
	}
}
void Char::Reimu::ReimuOption::OnUpdate(double x, double y)
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

void Char::Reimu::ReimuOption::OnDraw() const
{
	for (int i = 0; i < 4; ++i) {
		if (optionState[m_nowPower][i].have) {
			m_sp.at(i).DrawToFastBuffer();
		}
	}
}

void Char::Reimu::ReimuOption::OnSlowStart()
{
	m_slowMode = true;
	for (int i = 0; i < 4; ++i) {
		if (optionState[m_nowPower][i].have) {
			m_dataActor.Add(m_pos[i][0], optionState[m_nowPower][i].slow_x, 20, 1);
			m_dataActor.Add(m_pos[i][1], optionState[m_nowPower][i].slow_y, 20, 1);
		}
	}
}

void Char::Reimu::ReimuOption::OnSlowStopped()
{
	m_slowMode = false;
	for (int i = 0; i < 4; ++i) {
		if (optionState[m_nowPower][i].have) {
			m_dataActor.Add(m_pos[i][0], optionState[m_nowPower][i].x, 20, 1);
			m_dataActor.Add(m_pos[i][1], optionState[m_nowPower][i].y, 20, 1);
		}
	}
}

void Char::Reimu::ReimuOption::ShootBegin(bool)
{
}

static const double optionDefaultAngle[5][4] = {
	{},
	{0},
	{0.15,-0.15},
	{0.18,-0.18,0},
	{0.2,-0.2,-0.1,0.1},
};

void Char::Reimu::ReimuOption::ShootUpdate(bool flip)
{
	if (!(m_timer % 9)) {
		const double offsets[2] = { -_reimuNonSlowPowerAngle,_reimuNonSlowPowerAngle };
		for (int shootTime = 0; shootTime < 2; ++shootTime) {
			double offset = offsets[shootTime];
			for (int i = 0; i < 4; ++i) {
				if (optionState[m_nowPower][i].have) {
					const int mul = flip ? -1 : 1;
					const int offsetX = flip ? -80 : 0;
					if (!m_slowMode) {
						auto p = new BulletReimu;
						p->SetSpeed(mul*28.5);
						p->GetSprite().SetFlip(false, flip);
						p->SetAngle((mul * optionState[m_nowPower][i].angle + offset) * M_PI / 480);
						p->SetPower(_reimuOptionSubPower);
						p->SetImageNum(4);
						Game::GetGame().GetPlayerBulletManager().Shoot(p, m_pos[i][0] + m_px + offsetX, m_pos[i][1] + m_py);
					}
					else {
						auto p = new BulletReimu;
						p->SetSpeed(mul*28.5);
						p->GetSprite().SetFlip(false, flip);
						p->SetPower(_reimuOptionSubPowerSlow);
						p->SetImageNum(1);
						Game::GetGame().GetPlayerBulletManager().Shoot(p, mul*m_pos[i][0] + m_px + offsetX, m_pos[i][1] + m_py);
					}
				}
			}
		}
	}
}

void Char::Reimu::ReimuOption::ShootEnd(bool)
{
}

void Char::Reimu::ReimuOption::SetPower(int power)
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

void Char::Reimu::ReimuOption::SetPowerData(double fast, double slow)
{
	_reimuOptionSubPower = fast;
	_reimuOptionSubPowerSlow = slow;
}

void Char::Reimu::ReimuOption::SetNonSlowPowerAngle(double angle)
{
	_reimuNonSlowPowerAngle = angle;
}