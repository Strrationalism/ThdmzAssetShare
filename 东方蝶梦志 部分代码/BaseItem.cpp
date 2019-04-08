#include "Precompiled.h"
#include "BaseItem.h"
#include "Game.h"

void Item::BaseItem::SetItemImage(int num)
{
	m_itemImage.UseImage("Items", num);
	m_itemImage.SetAlpha(0.7);

	const bool flip = Game::GetGame().GetMirrorMode();
	m_itemImage.SetFlip(false, flip);
}

void Item::BaseItem::Create(double x, double y, double ySpd, double xSpd)
{
	m_timer = 0;
	m_ySpd = ySpd*0.35;
	m_xSpd = xSpd*0.75;

	m_circle.x = x;
	m_circle.y = y;
	this->OnCreate();
	m_itemImage.SetPos(x, y);

	m_getLimiter = 20;

	m_negaX = Game::GetGame().GetInputer().Rand2() >= 0;
}

#include "Log.h"
void Item::BaseItem::Update()
{
	m_timer++;
	double normalAngle = 1 - Clamp(0, 100, m_timer) / 100;
	normalAngle = 360 * 10 * SlowMotion3(normalAngle);

	if (m_getLimiter) --m_getLimiter;
	if (m_getting && m_getLimiter == 0) {
		//正在收取

		if (Game::GetGame().GetPlayer().Died()) {
			m_getting = false;
			m_itemImage.SetAngle(normalAngle);
		}
		else {
			double px, py;
			Game::GetGame().GetPlayer().GetPos(px, py);
			double angle = atan2(py - m_circle.y, px - m_circle.x);
			m_circle.x += cos(angle) * m_getSpeed;
			m_circle.y += sin(angle) * m_getSpeed;

			double xlength = (py - m_circle.y)*(py - m_circle.y) + (px - m_circle.x)*(px - m_circle.x);

			m_itemImage.SetAngle(180 * Mix(0, angle, 1 - (xlength / m_gettingLength)) / M_PI);

			Collision::Circle player;
			Game::GetGame().GetPlayer().GetPos(player.x, player.y);
			player.radius = 5;
			if (Game::GetGame().GetPlayer().Slow()) {
				player.radius *= 8.0;
			}
			if (Collision::CheckC2C(m_circle, player)) {
				m_died = true;
				OnGot();
			}
		}
	}
	else {
		//没有收取
		m_itemImage.SetAngle(normalAngle);
		m_circle.x -= m_ySpd * 0.625f;
		m_ySpd += 0.009f;

		m_circle.y -= m_negaX ? m_xSpd : -m_xSpd;
		if (m_xSpd - 0.05 > 0.95) m_xSpd -= 0.05;

		if (!Game::GetGame().GetPlayer().Died()) {
			Collision::Circle player;
			Game::GetGame().GetPlayer().GetPos(player.x, player.y);
			player.radius = 64;
			if (Game::GetGame().GetPlayer().Slow()) {
				player.radius *= 2.0;
			}

			if (m_circle.y >= HEIGHT / 2 - m_circle.radius || m_circle.y <= -HEIGHT / 2 + m_circle.radius)
				m_negaX = !m_negaX;

			if (m_circle.x > WIDTH / 2 - m_circle.radius) {
				m_circle.x = WIDTH / 2 - m_circle.radius;
				m_ySpd = -m_ySpd;
			}

			if (m_circle.x < -WIDTH / 2 - m_circle.radius * 2)
				m_died = true;

			else if (Collision::CheckC2C(m_circle, player)) {
				Get(false);
				m_getLimiter = 0;
			}
		}
	}
	m_itemImage.SetPos(m_circle.x, m_circle.y);
	//m_itemCornerImage.SetPos(m_circle.x, -350);

	m_itemImage.UpdateVertex();
	//m_itemCornerImage.UpdateVertex();
}

void Item::BaseItem::Get(bool itemGetLine) {
	m_getting = true;
	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	m_gettingLength = (y - m_circle.y)*(y - m_circle.y) + (x - m_circle.x)*(x - m_circle.x);
	m_getSpeed = itemGetLine ? 13 : 7;
}