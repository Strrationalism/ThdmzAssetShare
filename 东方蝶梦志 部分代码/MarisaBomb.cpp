#include "Precompiled.h"
#include "MarisaBomb.h"
#include "Game.h"
#include "Shaders.h"
#include "Collision.h"
#include "Bullet.h"
#include "Haptic.h"
#include "Boss.h"

static double _power;
//static std::vector<Collision::Rectangle> collDraw;

void Char::Marisa::MarisaBomb::checkToKillPlayer(const Collision::Rectangle & coll)
{
	//collDraw.push_back(coll);
	if (m_width > 160 && Collision::CheckR2C(coll, Game::GetGame().GetPlayer().GetDeathCollision()))
	{
		Game::GetGame().GetPlayer().Kill();
	}
}

void Char::Marisa::MarisaBomb::buildColl(double length,double w)
{
	Collision::Rectangle r;
	r.rotate = -m_angle;
	r.height = 240 * (m_width / 480) * w;
	r.width = 1200;
	r.x = m_x + (r.width/2 + length)*cos(m_angle*M_PI/180);
	r.y = m_y + sin(m_angle*M_PI/180)*(r.width / 2 + length);

	checkToKillPlayer(r);
}

Char::Marisa::MarisaBomb::MarisaBomb(bool flip,bool usePlayerPos,double x,double y,double height,double angle):
	m_usePlayerPos(usePlayerPos),
	m_x(x),
	m_y(y),
	m_height(height),
	m_angle(angle)
{
	m_died = false;
	m_flip = flip;
	m_taskList.AddTask([this]() {
		Kill();
	}, 500);

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_width, 0, 50, 4);
	}, 449);

	if(usePlayerPos)
		m_dataActor.Add(m_width, 480, 50, 2);
	else {
		m_taskList.AddTask([this]() {
			m_dataActor.Add(m_width, m_height, 50, 1);
		}, 100);

		m_dataActor.Add(m_width, 10, 50, 2);
	}
	m_mask = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("MaisaMasterSparkMask");
	m_perlin = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("CloudWB");
	m_circle = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("MaisaMasterSparkCircle");

	if (usePlayerPos) {
		Game::GetGame().GetPlayer().SetShootEnable(false);
		Game::GetGame().GetPlayer().SetMoveSpeedMul(0.2);
	}

	//ÊÖ±úÕð¶¯
	Basic::Haptic::Play(5000);
}

Char::Marisa::MarisaBomb::~MarisaBomb()
{
	if (m_usePlayerPos) {
		Game::GetGame().GetPlayer().SetShootEnable(true);
		Game::GetGame().GetPlayer().SetMoveSpeedMul(1.0);
	}
}


void Char::Marisa::MarisaBomb::Update()
{
	m_dataActor.Update();
	double playerX, playerY;	//Hacked!

	int flipMul = m_flip ? -1 : 1;

	m_marisaBomb_r = 0.7 + 0.3 * Rand();
	m_marisaBomb_g = 0.7 + 0.3 * Rand();
	m_marisaBomb_b = 0.7 + 0.3 * Rand();

	if (m_usePlayerPos) {
		Game::GetGame().GetPlayer().GetPos(playerX, playerY);		
	}
	else {
		playerX = m_x;
		playerY = m_y;

		const auto pBoss = Boss::Boss::GetBoss();
		if (pBoss)
		{
			if (pBoss->GetHP() <= 0 || pBoss->GetHP() >= 9999)
				Kill();
		}
		else
			Kill();
	}

	if (Game::GetGame().GetGameTime() % 35 == 0 && m_width >= 240) {
		m_circles.push_back({ 0,0,0 });
	}

	for (auto& p : m_circles) {
		p.x += flipMul * 24;
		double per = flipMul*(p.x - p.startX) / (flipMul*(flipMul*WIDTH / 2 - p.startX));
		if (per >= 1) per = 1;
		p.height = Mix(0, 260, SlowMotion2(per));
	}

	m_circles.erase(std::remove_if(m_circles.begin(), m_circles.end(), [](auto& p) {
		return p.x >= WIDTH / 2 + 200;
	}), m_circles.end());

	Collision::Rectangle coll;
	coll.rotate = 0;
	coll.y = playerY;
	coll.height = 267;

	if (m_flip) {
		coll.x = ((WIDTH / 2) + playerX) / 2 - WIDTH / 2 - 240;
		coll.width = WIDTH / 2 + playerX;
	}
	else {
		coll.x = playerX + WIDTH / 2;
		coll.width = WIDTH;
	}

	

	if (m_usePlayerPos) {
		//collDraw.clear();
		//collDraw.push_back(coll);
		for (auto& p : Game::GetGame().GetEnemyManager()) {
			auto pEne = dynamic_cast<Enemy::Enemy*>(p.get());
			if (pEne->GetColliEnabled()) {
				if (Collision::CheckR2C(coll, pEne->GetCollision())) {
					pEne->DecHP(_power);

					Game::GetGame().GetParticleTop().AddDot(0, [flipMul](Basic::Video::Renderer3D::Particle3D::Dot3D& d) {
						d.alpha = 2.0;
						d.angle = Rand() * M_PI * flipMul;// +M_PI / 4;
						if (!(rand() % 3))
							d.blend_dst = GL_ONE;
						d.size = 16.0;
						d.speed = 6.0 * Rand();
						d.r = Rand() * 0.4;
						d.g = Rand() * 0.4;
						d.b = Rand() * 0.2 + 0.8;
					}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& d, auto& p) {
						d.alpha -= 0.1;
						d.size -= 0.2;
					}, pEne->GetCollision().x, pEne->GetCollision().y, 64);
				}
			}
		}


		for (auto& layer : Game::GetGame().GetEnemyBulletManager()) {
			for (auto& p : layer) {
				auto pEne = (EnemyBullet::Bullet*)p.get();
				if (pEne->WhatTypeAmI() == EnemyBullet::Bullet::Type::Grid) continue;
				double x, y;
				pEne->GetPos(x, y);
				if (m_flip) {
					if (x <= playerX && y >= playerY - 267 / 2 && y <= playerY + 267 / 2) {
						if (!pEne->GetInvincible())
							pEne->Kill();
					}
				}
				else {
					if (x >= playerX && y >= playerY - 267 / 2 && y <= playerY + 267 / 2) {
						if (!pEne->GetInvincible())
							pEne->Kill();
					}
				}
			}
		}
	}
	else {
		//collDraw.clear();
		buildColl(500, 1.25);
		buildColl(350, 1);
		buildColl(300, 0.95);
		buildColl(250, 0.85);
		buildColl(150, 0.75);
		buildColl(100, 0.5);
		buildColl(70, 0.3);
	}

	m_taskList.Update();
}

void Char::Marisa::MarisaBomb::Draw() const
{
	double playerX, playerY;


	if (m_usePlayerPos)
		Game::GetGame().GetPlayer().GetPos(playerX, playerY);
	else
		playerX = m_x, playerY = m_y;

	int mul = m_flip ? -1 : 1;
	playerX += mul * 30;

	double offset = Game::GetGame().GetGameTime();
	offset /= -50;

	
	glTranslated(playerX, playerY, 0);
	glRotated(m_usePlayerPos ? m_angle : m_angle+180, 0, 0, 1);

	glBlendFunc(GL_SRC_COLOR, GL_ONE);
	Basic::Shaders::MarisaMask(offset);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *m_perlin);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *m_mask);
	glActiveTexture(GL_TEXTURE0);
	glBegin(GL_QUADS); {
		const double texWidth = (mul * 640 - playerX) / 1280;

		glColor4d(m_marisaBomb_r, m_marisaBomb_g, m_marisaBomb_b, 1.0);
		glTexCoord2d(0.001, 0); glVertex2d(0,  - m_width / 2);
		glTexCoord2d(0.001, 1); glVertex2d(0,  + m_width / 2);
		glTexCoord2d(texWidth * mul, 1); glVertex2d(+1440 * mul,  + m_width / 2);
		glTexCoord2d(texWidth * mul, 0); glVertex2d(+1440 * mul,  - m_width / 2);
	}
	glEnd();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(0);

	//glBlendFunc(GL_SRC_COLOR, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, *m_mask);
	glBegin(GL_QUADS); {
		const double texWidth = (mul * 640 - playerX) / 1280;

		glColor4d(m_marisaBomb_b, m_marisaBomb_g, m_marisaBomb_r, 0.75);
		glTexCoord2d(0.001, 0); glVertex2d(0,  - m_width / 4);
		glTexCoord2d(0.001, 1); glVertex2d(0,  + m_width / 4);
		glTexCoord2d(texWidth * mul, 1); glVertex2d(+1440 * mul,  + m_width / 4);
		glTexCoord2d(texWidth * mul, 0); glVertex2d(+1440 * mul,  - m_width / 4);
	}
	glEnd();
	glUseProgram(0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, *m_circle);
	for (auto& p : m_circles) {
		glBegin(GL_QUADS); {
			glColor4d(m_marisaBomb_r, m_marisaBomb_b, m_marisaBomb_g, m_width / 480 * 0.5);
			glTexCoord2d(0.001, 0); glVertex2d(+p.x,  - p.height);
			glTexCoord2d(0.001, 1); glVertex2d(+p.x,  + p.height);
			glTexCoord2d(1, 1); glVertex2d(+p.x + mul * 192,  + p.height);
			glTexCoord2d(1, 0); glVertex2d(+p.x + mul * 192,  - p.height);
		}glEnd();
	}

	glLoadIdentity();

	//for(auto& c:collDraw)
		//Collision::DrawRectangle(c);
}

void Char::Marisa::MarisaBomb::Kill()
{
	m_died = true;
}

void Char::Marisa::MarisaBomb::SetPowerData(double d)
{
	_power = d;
}