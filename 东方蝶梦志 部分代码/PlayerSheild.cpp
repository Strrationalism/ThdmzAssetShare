#include "Precompiled.h"
#include "PlayerSheild.h"
#include "Game.h"
#include "Bullet.h"
#include "DreamSheildBrokenEffect.h"
#include "DreamPointItem.h"

const static auto DreamPointWait = 35;
const static auto DreamPerBullet = 0.0005;
void PlayerSheild::halfKillBullet()
{
	Collision::Circle cir = Game::GetGame().GetPlayer().GetDeathCollision();
	cir.radius = 400;

	for (auto& layer : Game::GetGame().GetEnemyBulletManager())
	{
		for (auto& bullet : layer)
		{
			if (static_cast<EnemyBullet::Bullet&>(*bullet).GetInvincible())
				continue;
			if (static_cast<EnemyBullet::Bullet&>(*bullet).CollCheckWithCircle(cir))
			{
				double x, y;
				auto& b = static_cast<EnemyBullet::Bullet&>(*bullet);
				b.GetPos(x, y);
				int mul = Game::GetGame().GetPlayer().Slow() ? -1 : 1;

				double dream = Game::GetGame().GetScoreManager().GetDream();
				if (std::abs(dream) >= 0.7) mul = 0;

				Item::DreamPointItem::AddToGame(x, y, DreamPerBullet, DreamPointWait * mul);
				b.Kill();
			}
		}
	}

}


void PlayerSheild::OnDraw() const
{
	//全屏变暗

	float alpha = 0.5f;

	if (m_fullTime - m_time < 10)
	{
		alpha = (m_fullTime - m_time) / 10.0f;
		alpha = alpha * 0.5F;
	}
	else if (m_time < 10)
	{
		alpha = m_time / 10.0f;
		alpha = alpha * 0.5f;
	}

	glDisable(GL_TEXTURE_2D);
	glColor4f(0, 0, 0, alpha);
	glBegin(GL_QUADS);
	{
		glVertex2f(-640, -360);
		glVertex2f(-640, 360);
		glVertex2f(640, 360);
		glVertex2f(640, -360);
	}
	glEnd();
	glEnable(GL_TEXTURE_2D);
}

PlayerSheild::PlayerSheild(bool isLeft):
	m_isLeft{ isLeft },
	m_fullTime{isLeft ? 3000 : 1500}
{
	m_time = m_fullTime;
}

bool PlayerSheild::OnUpdate(double x, double y)
{
	m_px = x;
	m_py = y;

	auto initer = [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {

		dot.alpha = 1;
		dot.angle = Rand() * 360;
		dot.speed = Rand() * 3.5;
		dot.r = Rand();
		dot.b = Rand();
		dot.g = Rand();
		dot.self_angle = Rand() * 360;
		dot.size = 128;
		dot.blend_dst = Rand() > 0.5 ? GL_ONE : GL_ONE_MINUS_SRC_ALPHA;
	};

	auto program = [](Basic::Video::Renderer3D::Particle3D::Dot3D& d,
		Basic::Video::Renderer3D::Particle3D& sys)
	{
		d.alpha = d.alpha * 0.95;
		d.size = d.size * 0.95;
		if (d.size < 0.25) d.size = -1;
		d.speed = d.speed * 0.99;

	};

	const bool shootEnabled =
		Game::GetGame().GetPlayer().GetShootEnabled();

	if(shootEnabled)
		m_time--;

	const double rad = 256 * GetPercent() + 20;
	const double ang = Game::GetGame().GetGameTime() / 10.0;

	//L2:左端获得全屏吸点
	if (m_isLeft)
	{
		Game::GetGame().GetItemMgr().GetAll();
	}


	Game::GetGame().GetParticleBottom().AddDot(2, initer, program, rad*cos(ang) + m_px, rad*sin(ang) + m_py,2);
	Game::GetGame().GetParticleBottom().AddDot(2, initer, program, -rad*cos(ang) + m_px, -rad*sin(ang) + m_py,2);

	if (m_time > 0 == false)
	{
		Basic::Sound::SeManager::PlayHigh("bonus", m_px);

		if (m_giveLife)
		{
			const double itemX = -Rand() * 320;
			const double itemY = Rand() * 480 - 240;

			if (Game::GetGame().GetStageData()->stageName == "StageEx")
				Game::GetGame().GetItemMgr().AddItem(Item::ItemType::HeartKagera, itemX, itemY, 1);
			Game::GetGame().GetItemMgr().AddItem(Item::ItemType::HeartKagera, itemX, itemY , 1);
		}
	}
	return m_time > 0;
}

void PlayerSheild::Bomb()
{
	if (m_time > 10)
	{
		m_giveLife = false;
		Game::GetGame().GetPlayer().Invincible(350);

		AfterEffect::DreamSheildBrokenEffect::Mode effectMode =
			AfterEffect::DreamSheildBrokenEffect::Mode::HalfScreen;

		//L1:右端时，主动按X炸盾会全屏消弹
		if (!m_isLeft)
		{
			for (auto& layer : Game::GetGame().GetEnemyBulletManager())
			{
				for (auto& b : layer)
				{
					double x, y;
					static_cast<EnemyBullet::Bullet&>(*b).GetPos(x, y);
					int mul = Game::GetGame().GetPlayer().Slow() ? -1 : 1;

					double dream = Game::GetGame().GetScoreManager().GetDream();
					if (std::abs(dream) >= 0.7) mul = 0;

					Item::DreamPointItem::AddToGame(x, y, DreamPerBullet, DreamPointWait * mul);
				}
			}
			Game::GetGame().GetEnemyBulletManager().KillAll();
			effectMode =
				AfterEffect::DreamSheildBrokenEffect::Mode::FullScreen;
		}
		else
			halfKillBullet();

		Game::GetGame().AddAfterEffect(new AfterEffect::DreamSheildBrokenEffect(m_px, m_py, effectMode));
		m_time = 10;
	}
}

void PlayerSheild::KillPlayer()
{
	if (m_time > 10)
	{
		m_giveLife = false;
		Game::GetGame().GetPlayer().Invincible(350);

		Game::GetGame().AddAfterEffect(new AfterEffect::DreamSheildBrokenEffect(m_px, m_py, AfterEffect::DreamSheildBrokenEffect::Mode::HalfScreen));
		//Basic::Sound::SeManager::PlayHigh("bonus", m_px);

		halfKillBullet();
		m_time = 10;
	}
}

bool PlayerSheild::IsItemPowerX2() const
{
	return m_isLeft;
}

bool PlayerSheild::IsShootPowerX1dot3() const
{
	return !m_isLeft;
}
