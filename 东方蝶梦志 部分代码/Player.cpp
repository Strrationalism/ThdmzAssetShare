#include "Precompiled.h"
#include "Player.h"
#include "Globals.h"
#include "Collision.h"
#include "Game.h"
#include "NegaCircleZoom.h"
#include "DreamWaterWave.h"
#include "ConversationLogic.h"

using namespace std;


void Player::SetShootEnable(bool b) {
	/*if (!b) {
		m_shootDisable_counter++;

		if (m_shootDisable_counter == 1) {
			m_shootEnabled = b;

			if (m_shoot != m_shootEnabled) {
				if (m_shoot) {
					m_char->ShootEnd(m_flip);
					m_options->ShootEnd(m_flip);
				}
			}

			if (m_shoot == m_shootEnabled) {
				if (m_shoot) {
					m_char->ShootBegin(m_x, m_y, m_flip);
					m_options->ShootBegin(m_flip);
				}
			}
		}
	}
	else {
		m_shootDisable_counter--;

		if (m_shootDisable_counter == 0) {
			m_shootEnabled = b;

			if (m_shoot != m_shootEnabled) {
				if (m_shoot) {
					m_char->ShootEnd(m_flip);
					m_options->ShootEnd(m_flip);
				}
			}

			if (m_shoot == m_shootEnabled) {
				if (m_shoot) {
					m_char->ShootBegin(m_x, m_y, m_flip);
					m_options->ShootBegin(m_flip);
				}
			}
		}
	}*/
	m_shootEnabled = b;
}
bool Player::GetShootEnabled() const
{
	return m_shootEnabled;
}
void Player::Player::Draw() const
{
	if (m_state != DIED && m_invincible % 8 < 4)
		m_charAnime.DrawToFastBuffer();

	//Collision::DrawCircle(m_graze);
	if (m_state != DIED) m_options->OnDraw();
}

void Player::Player::Update()
{
	if (m_state == COMMING_TO_DEATH) m_comingToDeath.Update();
	if (m_invincible) --m_invincible;

	//如果正在接受玩家控制
	if (m_state == NORMAL) {
		if (m_vec_push) {
			m_x += cos(m_vec_angle)*(m_slow ? m_slowSpeed : m_normalSpeed)*m_speedMul*m_vec_speed;
			m_y += sin(m_vec_angle)*(m_slow ? m_slowSpeed : m_normalSpeed)*m_speedMul*m_vec_speed;
		}
		//边界限定
		if (m_x <= -WIDTH / 2 + 45) m_x = -WIDTH / 2 + 45;
		else if (m_x >= WIDTH / 2 - 45) m_x = WIDTH / 2 - 45;
		if (m_y <= -HEIGHT / 2 + 40) m_y = -HEIGHT / 2 + 40;
		else if (m_y >= HEIGHT / 2 - 40) m_y = HEIGHT / 2 - 40;

		if (m_x >= WIDTH / 2 - 400) {
			Game::GetGame().GetItemMgr().GetAll();
		}

		if (m_shoot) m_shootKeyEvent();

		if (m_shoot && m_shootEnabled && m_shootBombEnabled) {
			m_char->ShootUpdate(1, m_x, m_y, m_flip);	//发射子弹
			m_options->ShootUpdate(m_flip);
		}
	}

	else if (m_state == BIRTHING) {
		m_x += 5;
		if (m_timer >= 30) {
			m_state = NORMAL;
			if (m_slow) {
				m_dataActor.Add(m_collDisplayProgress, 1, 20, 1);
				m_options->OnSlowStart();
			}
			if (m_shoot) {
				m_char->ShootBegin(m_x, m_y, m_flip);
				m_options->ShootBegin(m_flip);
			}
		}
		//m_charAnime.SetPos(x, y);
	}
	m_charAnime.SetPos(m_x, m_y);
	m_charAnime.Update();
	m_charAnime.SetZoom(0.75);
	m_dataActor.Update();
	m_charAnime.UpdateVertex();
	m_char->Update(m_x, m_y);

	Game::GetGame().GetPlayerDeathCollDisplay().GetSprite().SetZoom(m_collDisplayProgress);
	Game::GetGame().GetPlayerGrazeCollDisplay().GetSprite().SetZoom((2 - m_collDisplayProgress)*1.3f);
	Game::GetGame().GetPlayerGrazeCollDisplay().GetSprite().SetAlpha(m_collDisplayProgress * 0.5f);
	Game::GetGame().GetPlayerDeathCollDisplay().GetSprite().SetPos(m_x, m_y);
	Game::GetGame().GetPlayerGrazeCollDisplay().GetSprite().SetPos(m_x, m_y);
	Game::GetGame().GetPlayerDeathCollDisplay().GetSprite().UpdateVertex();
	Game::GetGame().GetPlayerGrazeCollDisplay().GetSprite().UpdateVertex();

	Game::GetGame().GetPlayerGrazeCollDisplay().GetSprite().SetAngle(m_collAngle);
	m_collAngle += m_collAngleSpeed;
	if (Basic::Sound::BgmPlayer::IsBeatFrame()) {
		m_collAngleSpeed = 3.0;
		Game::GetGame().GetDataActor().Add(m_collAngleSpeed, 0.5f, 20, 1);
	}

	m_death.x = m_graze.x = m_x;
	m_death.y = m_graze.y = m_y;

	if (m_state != DIED) m_options->OnUpdate(m_x, m_y);

	++m_timer;

	if (m_sheild.has_value())
	{
		if (!m_sheild.value().OnUpdate(m_x, m_y))
			m_sheild.reset();
	}
}

void Player::Player::SetChar(Char::BasicChar & c)
{
	m_charAnime.UseImage("Player", 8, c.GetSprite2DImageNum(), 10);
	m_normalSpeed = c.GetNormalSpeed();
	m_slowSpeed = c.GetSlowSpeed();
	m_char = &c;
	m_powerTop = c.GetPowerTop();
	m_power = 0;

	m_death.radius = 0.5;
	m_graze.radius = 80;
}

void Player::OnEvent(const Basic::Input::Message & e)
{
	//if (m_state == DIED) return;
	switch (e.type) {
	case Basic::Input::Message::Type::VecMotion: {
		const bool isMirror = Game::GetGame().IsMirrorMode();

		auto motion = e.vec;

		if (isMirror) motion.x = -motion.x;

		m_vec_push = motion.x || motion.y;
		m_vec_angle = atan2((double)motion.y, (double)motion.x);

		double x = motion.x / 32768.0;
		double y = motion.y / 32768.0;

		//求圆上的分量比率
		double xMul = fabs(cos(m_vec_angle));
		double yMul = fabs(sin(m_vec_angle));

		x *= xMul;
		y *= yMul;

		m_vec_speed = sqrt(x * x + y * y);

		break;
	}
	case Basic::Input::Message::Type::KeyDown:
		if (e.key == Basic::Input::Key::Slow) {
			m_slow = true;
			if (m_state == NORMAL) {
				m_dataActor.Add(m_collDisplayProgress, 1, 20, 1);
				m_options->OnSlowStart();
			}
		}
		else if (e.key == Basic::Input::Key::Shoot) {
			if (!m_shoot) {
				m_shoot = true;
				if (m_state == NORMAL && m_shootEnabled) {
					m_char->ShootBegin(m_x, m_y, m_flip);
					m_options->ShootBegin(m_flip);
				}
			}
		}
		else if (e.key == Basic::Input::Key::Bomb) {
			if (!m_bombEnabled) break;
			if (!m_shootBombEnabled) break;
			if (m_sheild.has_value())
			{
				if (m_sheild->Avliable())
				{
					m_sheild.value().Bomb();
					return;
				}
			}
			if (m_power >= 1.0f && (m_state == COMMING_TO_DEATH || m_state == NORMAL) && !m_char->Bombing()) {
				if (m_state == COMMING_TO_DEATH) {
					m_state = NORMAL;
					m_comingToDeath.Clear();
				}
				Game::GetGame().GetItemMgr().GetAll();
				m_char->Bomb(m_flip);
				SetPower(m_power - 1);
				m_options->SetPower((int)m_power);
			}
		}
		/*else if (e.key == Basic::Input::Key::Swap && !m_char->Bombing()) {
			m_flip = !m_flip;
			m_charAnime.SetFlip(false, m_flip);
		}*/
		break;
	case Basic::Input::Message::Type::KeyUp:
		if (e.key == Basic::Input::Key::Slow) {
			m_slow = false;
			if (m_state == NORMAL) {
				m_dataActor.Add(m_collDisplayProgress, 0, 20, 1);
				m_options->OnSlowStopped();
			}
		}
		else if (e.key == Basic::Input::Key::Shoot) {
			if (m_shoot) {
				m_shoot = false;
				if (m_state == NORMAL && m_shootEnabled) {
					m_char->ShootEnd(m_flip);
					m_options->ShootEnd(m_flip);
				}
			}
		}
		break;
	};
}

void Player::Player::ForceKill()
{
	m_state = DIED;
	m_x = -9999;
	m_y = 0;
	m_shootKeyEvent = []() {};
	m_options.reset(nullptr);

	m_shootEnabled = m_bombEnabled = true;
}
#include "Haptic.h"
void Player::Kill()
{
	if (m_sheild.has_value())
	{
		if (m_sheild->Avliable())
		{
			m_sheild.value().KillPlayer();
			//m_sheild.reset();
			return;
		}
	}

	double x = m_x, y = m_y;
	if (m_invincible || m_state == DIED || m_state == COMMING_TO_DEATH) return;
	m_state = COMMING_TO_DEATH;

	Basic::Sound::SeManager::Play("pldead00", x,40);
	Basic::Haptic::Play(500);

	//double cx = m_death.x, cy = m_death.y;
	const double cx = x, cy = y;

	int commingToDeath = 0;
	if (m_power >= 1.0)
		commingToDeath = 9;

	m_comingToDeath.AddTask([cx, cy, x, y, this]() {		//决死
		if (this->m_slow) {
			this->m_dataActor.Add(m_collDisplayProgress, 0, 20, 1);
			this->m_options->OnSlowStopped();
		}

		if (this->m_shoot) {
			this->m_char->ShootEnd(m_flip);
			this->m_options->ShootEnd(m_flip);
		}

		Game::GetGame().GetTaskList().AddTask([cx,cy]() {
			Game::GetGame().GetEnemyBulletManager().KillAllKeepNotKillLayerWithCenterPosition(cx, cy);
		}, 5);

		auto power = m_power > 3 ? m_power : 3;
		Game::GetGame().GetItemMgr().AddItem(Item::ItemType::Power, cx, cy, 7, 15);

		Game::GetGame().GetTaskList().AddTask([]() {
			Game::GetGame().GetEnemyManager().KillAll();
		}, 90);
		Game::GetGame().GetPlayer().ForceKill();

		Game::GetGame().AddAfterEffect(new AfterEffect::NegaCircleZoom(cx, cy));
		auto pDreamWater = new AfterEffect::DreamWaterWave(cx, cy);
		pDreamWater->SetBlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		pDreamWater->SetColor(1, 1, 1, 1, 1, 1, 1, 1);

		Game::GetGame().AddAfterEffect(pDreamWater);

		Game::GetGame().GetScoreManager().KillALife();

		//死亡粒子特效
		double radius = 8;

		Game::GetGame().GetParticleTop().AddDot(2, [radius](Basic::Video::Renderer3D::Particle3D::Dot3D& i) {
			i.size *= 8;
			i.angle = double((double(rand()) / RAND_MAX) * 2 * M_PI);
			i.speed = ((double)rand() / RAND_MAX) * 24 * 0.5;
			i.self_angle = ((double)rand() / RAND_MAX) * 360;
			i.x += ((double)rand() / RAND_MAX) * radius - radius / 3 * 2;
			i.y += ((double)rand() / RAND_MAX) * radius - radius / 3 * 2;
			i.r = Rand();
			i.g = Rand();
			i.b = Rand();
			//i.blend_src = GL_SRC_ALPHA;
			i.blend_dst = GL_ONE;
		},
			[](auto& i, auto& p) {
			i.size -= 3.4f;
			i.alpha -= 0.02f;
			i.speed -= 0.05f;
			i.self_angle += 2.0f;
		}, x, y, 96);

		Game::GetGame().GetParticleTop().AddDot(2, [radius](Basic::Video::Renderer3D::Particle3D::Dot3D& i) {
			i.size *= 8;
			i.angle = double((double(rand()) / RAND_MAX) * 2 * M_PI);
			i.speed = ((double)rand() / RAND_MAX) * 32 * 0.5;
			i.self_angle = ((double)rand() / RAND_MAX) * 360;
			i.x += ((double)rand() / RAND_MAX) * radius - radius / 3 * 2;
			i.y += ((double)rand() / RAND_MAX) * radius - radius / 3 * 2;
			i.r = Rand();
			i.g = Rand();
			i.b = Rand();
			//i.blend_src = GL_SRC_ALPHA;
			//i.blend_dst = GL_ONE;
		},
			[](auto& i, auto& p) {
			i.size -= 3.4f;
			i.alpha -= 0.02f;
			i.speed -= 0.05f;
			i.self_angle += 2.0f;
		}, x, y, 48);

		radius = 64;
		Game::GetGame().GetParticleTop().AddDot(4, [radius](Basic::Video::Renderer3D::Particle3D::Dot3D& i) {
			i.alpha = 1;
			i.size *= 3;
			i.speed = 0;
			i.x += ((double)rand() / RAND_MAX) * radius - radius / 3 * 2;
			i.y += ((double)rand() / RAND_MAX) * radius - radius / 3 * 2;
			i.r = Rand();
			i.g = Rand();
			i.b = Rand();
			i.blend_dst = GL_ONE;
		},
			[](auto& i, auto& p) {
			i.size += 26;
			i.alpha -= 0.09f;
		}, x, y, 1);
	}, commingToDeath);
}

void Player::Player::Birth()
{
	if (m_power < 3)
		SetPower(3);

	//m_slow = false;

	m_sheild.reset();
	m_flip = false;
	m_shootBombEnabled = true;
	m_charAnime.SetFlip(false, m_flip);
	//m_shootDisable_counter = 0;
	m_collDisplayProgress = 0;
	m_state = BIRTHING;
	m_timer = 0;
	m_x = -WIDTH / 2 - 80;
	m_y = 0;
	Invincible(500);
	m_options = m_char->CreateOption();
	m_options->SetPower((int)m_power);
}

void Player::PowerUp(double add) {
	if (m_power < m_powerTop) {
		if (m_power + add >= m_powerTop) {
			add = m_powerTop - m_power;
		}
		if ((int)m_power != int(m_power + add)) {
			if (m_options) m_options->SetPower(int(m_power + add));
			Basic::Sound::SeManager::Play("powerup", m_x, 14);
		}
		m_power += add;
		m_char->PowerUp(m_power, add);
	}

	Game::GetGame().GetUI().GetGrazeDisplay().SetPower(m_power);
}

std::optional<PlayerSheild>& Player::GetSheild()
{
	return m_sheild;
}

void Player::SetPower(double power) {
	m_power = power;
	Game::GetGame().GetUI().GetGrazeDisplay().SetPower(m_power);
	if (m_options) m_options->SetPower(int(m_power));
	if (m_char) m_char->PowerUp(0, m_power);
}

void Player::OnGraze() {
	if (m_state != DIED) {
		Basic::Sound::SeManager::Play("graze", m_x, 24);
		Game::GetGame().GetScoreManager().GrazeAdd(1);

		Game::GetGame().GetParticleTop().AddDot(0,
			[](Basic::Video::Renderer3D::Particle3D::Dot3D& d) {
			d.alpha = 2;

			d.r = d.g = d.b = 0.25;
			d.size = 60;
			d.angle = Rand() * M_PI * 2;
			d.speed = Rand() * 2.5 + 2.5;
		}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& d, auto&) {
			d.size -= 2;
			d.speed *= 0.995;
		}, m_x, m_y, 1
			);
	}
}

void Player::SetPos(double x, double y)
{
	m_x = x;
	m_y = y;
}
