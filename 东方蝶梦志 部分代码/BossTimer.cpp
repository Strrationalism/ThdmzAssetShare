#include "Precompiled.h"
#include "BossTimer.h"
#include "SeManager.h"
#include "Globals.h"
#include "Game.h"
Boss::BossTimer::BossTimer() :m_numberDisplay("GameUI/Number1")
{
	m_died = false;
	m_numberDisplay.SetDisplayFormat(2);
	m_numberDisplay.SetPos(WIDTH / 2 - 2 * 28 - 16, -HEIGHT / 2 + 16);
}

void Boss::BossTimer::Update()
{
	m_dataActor.Update();
	m_numberDisplay.SetAlpha(m_alpha);
	m_numberDisplay.SetZoom(m_zoom);

	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	bool halfAlpha = y < -290;
	if (halfAlpha != m_halfAlpha) {
		m_halfAlpha = halfAlpha;
		if (m_halfAlpha)
			m_dataActor.Add(m_alpha, 0.4, 20, 1);
		else
			m_dataActor.Add(m_alpha, 1, 20, 1);
	}
}

void Boss::BossTimer::Draw() const
{
	m_numberDisplay.Draw();
}

void Boss::BossTimer::Hide()
{
	m_dataActor.Add(m_alpha, 0, 40, 1);
}

void Boss::BossTimer::Show()
{
	if (m_halfAlpha)
		m_dataActor.Add(m_alpha, 0.4, 20, 1);
	else
		m_dataActor.Add(m_alpha, 1, 20, 1);
}

void Boss::BossTimer::SetTime(int time)
{
	int sec = time / LPS;
	if (sec != m_sec) {
		if (sec < 10) {
			m_numberDisplay.SetColorFliter(1, 0, 0);
			if (sec <= 3) {
				Basic::Sound::SeManager::Play("timeout2", 0);
				m_zoom = 1.5;
				m_dataActor.Add(m_zoom, 1, 10, 1);
			}
			else {
				Basic::Sound::SeManager::Play("timeout", 0);
				m_zoom = 1.3;
				m_dataActor.Add(m_zoom, 1, 10, 1);
			}
		}
		else {
			m_numberDisplay.SetColorFliter(1, 1, 1);
		}
		m_numberDisplay.SetNumber(time / LPS);
		m_sec = sec;
	}
}

void Boss::BossTimer::Kill()
{
	Hide();
	Game::GetGame().GetTaskList().AddTask([this]() {
		m_died = true;
	}, 40);
}