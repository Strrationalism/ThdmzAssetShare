#include "Precompiled.h"
#include "Boss.h"
#include "Game.h"
#include "BossSpellCardStandEffect.h"
#include "BossSpellCardAttackEffect.h"
#include "ImageCacheManager.h"
//#include "BossAttackTimeShow.h"
#include "DreamWaterWave.h"
#include "NegaCircleZoom.h"
#include "BossSpellBonusShow.h"
#include "BossSpellScorer.h"
#include "SpellCardDatabase.h"

#define BOSS_DEFAULT_X (WIDTH/4)
#define BOSS_DEFAULT_Y 0

Boss::Boss* nowBoss = nullptr;
Boss::Boss * Boss::Boss::GetBoss()
{
	return nowBoss;
}

void Boss::Boss::popSkill()
{


	Game::GetGame().GetTaskList().AddTask([]() {
		Game::GetGame().GetEnemyBulletManager().KillAll();
		Game::GetGame().GetEnemyManager().KillAll();
	}, 25);
	m_shooter.angle = 0;
	m_shooter.layer = 0;
	m_shooter.minLiveOutScreen = 0;
	m_shooter.radius = 0;
	m_shooter.range = 360;
	m_shooter.ways = 1;
	m_shooter.vecangle = 0;
	m_shooter.spy = false;
	if (m_skills.empty()) return;
	if (m_skills.front()->timeLimit != -1) {
		m_fullTimeLimit = m_skillTimer = m_skills.front()->timeLimit;
	}
	else {
		while (1) {
			if (m_skillTimer <= 0)
				m_skills.pop();
			else
				break;
			if (m_skills.empty()) {
				m_canBeKill = true;
				Kill();
				return;
			}
			m_fullTimeLimit = m_skillTimer = m_skills.front()->timeLimit;
		}
	}

	m_spellCardScore = m_skills.front()->score;
	m_dontKillHPBecauseLastBomb = Game::GetGame().GetPlayer().Bombing();
	m_delayTime = 50;
	m_colliEnabled = false;
	m_fullHP = m_hp = m_skills.front()->hp;

	m_dmslUnit.push_back({ true,std::unique_ptr<Dmsl::DmslUnit>() });
	m_dmslUnit.back().ptr.swap(m_skills.front()->dmslUnit);

	m_isSpellCard = m_skills.front()->isSpellCard;
	m_isTimeSkill = m_skills.front()->timeSpellCard;

	m_bossHpDisplay->SetVisible(!m_isTimeSkill);
	m_isTimeSkill ? m_bossMahoujin->Hide() : m_bossMahoujin->Show();
	if(m_isTimeSkill) Game::GetGame().SetBossPosY(-9999);

	m_hpDisplayLowerLimit = m_skills.front()->hpDisplayLowLimit;
	if (m_skills.front()->useNewHPLine) {
		m_hpDisplayHighLimit = 1;

		m_bossHpDisplay->SetHP(0);
		if (!m_hpPoints.empty()) {
			for (auto p : m_hpPoints.front())
				m_bossHpDisplay->SetHPPoint(p);

			m_hpPoints.pop();
		}
		m_bossHpDisplay->FadeToFull();
	}
	else
		m_hpDisplayHighLimit = m_lastLowerHP;

	m_lastLowerHP = m_hpDisplayLowerLimit;

	if (!m_isTimeSkill) {
		m_colliEnabled = true;
		m_enemyImage.SetAlpha(1.0f);
	}
	else
		m_enemyImage.SetAlpha(0.0f);
	m_bossMahoujin->EffectSinSize(m_isSpellCard);

	if (m_isSpellCard) {
		double x, y;
		GetPos(x, y);

		m_spellTimer = 0;
		m_cardBonus = true;
		m_spellCardDisplay->Reset(m_skills.front()->spellCardNameDisplay);
		Basic::Sound::SeManager::Play("ch00", x);
		Game::GetGame().GetMiscManagerBottom().Insert(new BossSpellCardStandEffect(m_spellLH));
		Game::GetGame().GetMiscManagerBottom().Insert(new BossSpellCardAttackEffect);
		m_bg.Show();
		m_spellCardCountDisplay->KillASpell();

		m_spellScorer->SpellCardStart(m_skillTimer,m_isTimeSkill, m_skills.front()->spellHash);
	}

	m_skills.pop();
}

Boss::Boss::Boss(std::shared_ptr<Dmsl::DmslVirtualMachine> dvm) :
	Enemy(VMContext{ dvm, std::make_shared<Dmsl::GameObjectLibraryLinkTable>(), 0 })
{
	m_hp = 9999999;
	m_dvm = std::move(dvm);
	m_colliEnabled = false;

	m_died = false;
	GameObjectWithDmslControl::SetPos(0, 0);
	GameObjectWithDmslControl::SetSpeed(0);

	m_bossTimer = new BossTimer;
	m_spellCardDisplay = new SpellCardDisplay;
	m_bossHpDisplay = new BossHPDisplay;
	m_bossMahoujin = new BossMahoujin;
	m_spellCardCountDisplay = new BossSpellCardCountDisplay;
	m_spellScorer = new BossSpellScorer;
	m_spellLH = std::make_shared<Basic::Video::Renderer2D::Sprite2D>();

	m_hpPoints.push(std::vector<double>());
}

Boss::Boss::~Boss()
{
	if (m_needDeleteObjects) {
		delete m_bossTimer;
		delete m_spellCardDisplay;
		delete m_bossMahoujin;
		delete m_spellCardCountDisplay;
		delete m_spellScorer;
		delete m_bossHpDisplay;
	}

	nowBoss = nullptr;
}

void Boss::Boss::SetStartBossTask(StageTask::StartBoss * s)
{
	m_startBossTask = s;
}

void Boss::Boss::AddSkill(Skill * s)
{
	m_skills.push(std::unique_ptr<Skill>(s));

	(m_hpPoints.back()).push_back(s->hpDisplayLowLimit);
	if (s->hpDisplayLowLimit == 0) {
		m_hpPoints.push(std::vector<double>());
	}
	if (s->isSpellCard) ++m_fullSpellCount;
}

void Boss::Boss::AddAnimation(const AnimationData & s)
{
	m_animations.push_back(s);
}

void Boss::Boss::FirstFrameRun()
{
	m_dreamX = 0.5;
	SetVMContext({
		Game::GetGame().GetStageData()->enemyScript,
		Game::GetGame().GetStageData()->enemyScriptLinkTable,
		Game::GetGame().GetStageData()->enemyDiedProgramHandle
	});
	UseCharAnimation(0);
	m_hp = 99999999;
	m_collision.radius = 45;
	m_bg.Active();
	//Game::GetGame().GetEnemyManager().EnableCreateByCSV(false);

	if (m_inAnimation != -1) {
		UseCharAnimation(m_inAnimation);
		GameObjectWithDmslControl::SetPos(WIDTH / 2 + 100, 0);
		GameObjectWithDmslControl::SetTask({
			GameObjectWithDmslControl::Task::MOVE,
			WIDTH / 2 + 100, 0,
			BOSS_DEFAULT_X,BOSS_DEFAULT_Y,
			50,1
		});
	}
	else {
		GameObjectWithDmslControl::SetPos(BOSS_DEFAULT_X, BOSS_DEFAULT_Y);
	}
	nowBoss = this;
}

void Boss::Boss::Update()
{
	double x, y;
	GameObjectWithDmslControl::GetPos(x, y);

	

	if(!m_isTimeSkill)
		Game::GetGame().SetBossPosY(y);

	//翻脸模式
	if (m_canFlip && !m_canBeKill) {
		double px, py;
		Game::GetGame().GetPlayer().GetPos(px, py);
		m_enemyImage.SetFlip(false, px >= x);
	}

	if (m_isTimeSkill)
		m_hp = 99999999;

	if (m_delayTime) {
		--m_delayTime;
		m_hp = m_fullHP;
	}

	if (m_dontKillHPBecauseLastBomb) {
		m_dontKillHPBecauseLastBomb = Game::GetGame().GetPlayer().Bombing();
		m_hp = m_fullHP;
		m_cardBonus = true;
	}

	if (m_isSpellCard && !m_delayTime)
		++m_spellTimer;

	if (m_started) {
		--m_skillTimer;
		int displayTimer = m_skillTimer;
		if (displayTimer < 0) displayTimer = 0;
		else if (displayTimer >= 9900) displayTimer = 9900;
		m_bossTimer->SetTime(displayTimer);
	}

	if (m_skillTimer == 0 && m_hp > 0) {
		m_colliEnabled = false;
		m_hp = 0;
		if (!m_isTimeSkill)
		{
			m_cardBonus = false;
			m_spellScorer->ClearScore();
		}
		m_skillTimer = -1;
	}

	if (m_effectPowerUp) {
		--m_effectPowerUp;

		if (m_effectPowerUp >= 40) {
			Game::GetGame().GetParticleBottom().AddDot(3, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
				dot.alpha = 0.1;
				dot.angle = 0;
				dot.speed = 0;

				dot.r = Rand() * 0.2;
				dot.g = Rand() * 0.2;
				dot.b = Rand() * 0.2;

				dot.userdata[0] = dot.x;	//圆心x位置
				dot.userdata[1] = dot.y;	//圆心y位置
				dot.userdata[2] = Rand() * 500;	//圆半径
				dot.userdata[3] = Rand() * M_PI * 2;	//与圆心角度
				dot.self_angle = 0;	//状态

				dot.size = 1000 * Rand();

				dot.blend_dst = GL_ONE;
			},
				[](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
				if (dot.self_angle == 1)  dot.alpha -= 0.015;
				else dot.alpha += 0.05;
				if (dot.alpha >= 1.0f) dot.self_angle = 1;
				if (dot.userdata[2] > 15) dot.userdata[2] -= 15;
				else dot.userdata[2] = 0;
				dot.userdata[3] += 0.05;

				dot.x = dot.userdata[2] * cos(dot.userdata[3]) + dot.userdata[0];
				dot.y = dot.userdata[2] * sin(dot.userdata[3]) + dot.userdata[1];

				dot.size -= 15;
			}, x, y, 15);

			Game::GetGame().GetParticleTop().AddDot(2, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
				dot.alpha = 0.1;
				dot.angle = 0;
				dot.speed = 0;

				dot.r = Rand() * 0.9;
				dot.g = Rand() * 0.9;
				dot.b = Rand() * 0.9;

				dot.userdata[0] = dot.x;	//圆心x位置
				dot.userdata[1] = dot.y;	//圆心y位置
				dot.userdata[2] = Rand() * 300;	//圆半径
				dot.userdata[3] = Rand() * M_PI * 2;	//与圆心角度
				dot.self_angle = 360 * Rand();

				dot.size = 500 * Rand();

				dot.blend_dst = GL_ONE;
			},
				[](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
				dot.alpha += 0.05;
				dot.self_angle += 5;
				if (dot.userdata[2] > 7.5) dot.userdata[2] -= 7.5;
				else dot.userdata[2] = 0;
				//dot.userdata[3] += 0.05;

				dot.x = dot.userdata[2] * cos(dot.userdata[3]) + dot.userdata[0];
				dot.y = dot.userdata[2] * sin(dot.userdata[3]) + dot.userdata[1];

				dot.size -= 25;
			}, x, y, 7);
		}
		else if (m_effectPowerUp == 1) {
			Basic::Sound::SeManager::Play("tan00", x);
			Game::GetGame().GetParticleTop().AddDot(2, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
				dot.size = 500 * Rand();

				dot.r = Rand() * 0.9;
				dot.g = Rand() * 0.9;
				dot.b = Rand() * 0.9;

				dot.speed = 17.5 * Rand();
				dot.angle = 2 * M_PI * Rand();
				dot.self_angle = 360 * Rand();

				dot.blend_dst = GL_ONE;
			}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
				dot.alpha -= 0.025;
				dot.self_angle -= 5;
			}, x, y, 40);

			Game::GetGame().GetParticleTop().AddDot(3, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
				dot.size = 500 * Rand();
				dot.alpha = 0.5 * Rand();

				dot.r = Rand() * 0.2;
				dot.g = Rand() * 0.2;
				dot.b = Rand() * 0.2;

				dot.speed = 35 * Rand();
				dot.angle = 2 * M_PI * Rand();

				dot.blend_dst = GL_ONE;
			}, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
				dot.alpha -= 0.025;
			}, x, y, 100);
		}
	}

	if (!m_isTimeSkill)
	{
		Game::GetGame().GetParticleBottom().AddDot(3,
			[this](Basic::Video::Renderer3D::Particle3D::Dot3D& d) {
			d.angle = M_PI;
			d.speed = Rand() * 2.5;
			d.size = 500;
			d.alpha = 0.3f;
			d.r = this->m_fogR;
			d.g = this->m_fogG;
			d.b = this->m_fogB;
			d.blend_dst = GL_ONE;

			d.x += (Rand() * 2 - 1) * 30;
			d.y += 70;
		},
			[](Basic::Video::Renderer3D::Particle3D::Dot3D& d, auto&) {
			d.alpha -= 0.005f;
		}, x, y, 1
			);

		Game::GetGame().GetParticleTop().AddDot(3,
			[this](Basic::Video::Renderer3D::Particle3D::Dot3D& d) {
			d.angle = M_PI;
			d.speed = Rand() * 2.5;
			d.size = 500;
			d.alpha = 0.4f;
			d.r = this->m_fogR;
			d.g = this->m_fogG;
			d.b = this->m_fogB;

			d.x += (Rand() * 2 - 1) * 30;
			d.y += 70;
		},
			[](Basic::Video::Renderer3D::Particle3D::Dot3D& d, auto&) {
			d.alpha -= 0.005f;
		}, x, y, 1
			);
	}

	//if (m_isSpellCard) {
		//如果检测到玩家死了或炸了，那么取消收卡资格
		if (m_cardBonus) {
			if (Game::GetGame().GetPlayer().Died() || Game::GetGame().GetPlayer().Bombing())
			{
				m_spellScorer->ClearScore();
				m_cardBonus = false;
			}
			
		}
	//}

	auto displayHP = Mix(m_hpDisplayLowerLimit, m_hpDisplayHighLimit, m_hp / m_fullHP);
	if (displayHP >= 0 && displayHP <= 100)
		m_bossHpDisplay->SetHP(displayHP);

	m_bossHpDisplay->SetPos(x, y);
	m_bossMahoujin->SetPos(x, y);

	if (m_hp <= 0 && !m_died && m_started) {
		//Game::GetGame().GetPlayer().Invincible(3);
		m_colliEnabled = false;
		m_taskList.Clear();
		m_effectPowerUp = -1;

		Game::GetGame().GetTaskList().AddTask([]() {
			Game::GetGame().GetEnemyManager().KillAll();
		}, 1);
		Game::GetGame().GetTaskList().AddTask([]() {
			Game::GetGame().GetEnemyBulletManager().KillAll();
		}, 2);
		//Game::GetGame().GetEnemyBulletManager().KillAll();

		if (m_lastLowerHP <= 0)
			m_bossHpDisplay->SetVisible(false);

		m_hp = 9999999;

		SpellCardOver();

		m_dmslUnit.clear();

		m_spellCardDisplay->Hide();

		if (m_isSpellCard) {
			Basic::Sound::SeManager::Play("tan01", x);
			m_bg.Hide();

			Game::GetGame().GetUILayerGameObjectManager().Insert(new BossSpellBonusShow(m_cardBonus));

			unsigned baseScore = m_spellCardScore * ScoreManager::GetLevelScoreMul(Game::GetGame().GetLevel());
			if (m_cardBonus) {
				Game::GetGame().GetItemMgr().AddItem(Item::ItemType::HeartKagera, x, y, 1, 15);
				Game::GetGame().GetItemMgr().AddItem(Item::ItemType::Power, x, y, 30, 15);
				Game::GetGame().GetItemMgr().AddItem(Item::ItemType::BigPower, x, y, 1, 19);
				Game::GetGame().GetItemMgr().AddItem(Item::ItemType::BluePoint, x, y, 20, 12);
			}
			else {

				Game::GetGame().GetItemMgr().AddItem(Item::ItemType::BluePoint, x, y, 5, 5);
				Game::GetGame().GetItemMgr().AddItem(Item::ItemType::Power, x, y, 35, 6);
			}
		}

		if (!m_skills.empty()) {
			m_taskList.AddTask([this] {
				popSkill();
			}, m_isSpellCard ? 120 : 10);
		}
		else {
			m_canBeKill = true;
			m_hp = 9999999;
			m_colliEnabled = false;
			Kill();
		}
		return;
	}
	if (m_delayTime == 0) {
		while (!m_readyToInsert.empty()) {
			m_dmslUnit.push_back({ true,std::move(m_readyToInsert.front()) });
			m_readyToInsert.pop();
		}

		for (auto& p : m_dmslUnit) {
			p.ptr->SetAttribute(m_dmslLinkTable->angle, GameObjectWithDmslControl::GetAngle() * 180 / M_PI);
			p.ptr->SetAttribute(m_dmslLinkTable->speed, GameObjectWithDmslControl::GetSpeed());
			m_stopRunning = &p.running;
			p.ptr->Call();
			GameObjectWithDmslControl::SetAngle(p.ptr->GetAttribute(m_dmslLinkTable->angle) / M_PI * 180);
			GameObjectWithDmslControl::SetSpeed(p.ptr->GetAttribute(m_dmslLinkTable->speed));
		}
		m_dmslUnit.erase(std::remove_if(m_dmslUnit.begin(), m_dmslUnit.end(), [](auto& p) {
			return !p.running;
		}), m_dmslUnit.end());
	}
	GameObjectWithDmslControl::Update();

	m_collision.x = x;
	m_collision.y = y;

	m_enemyImage.SetPos(x, y + 5 * std::sin(Game::GetGame().GetGameTime() / 30.0));

	m_enemyImage.Update();
	m_enemyImage.SetZoom(0.75f);
	m_enemyImage.UpdateVertex();

	if (Collision::CheckC2C(Game::GetGame().GetPlayer().GetDeathCollision(), m_collision)) {
		if (m_colliEnabled)
			Game::GetGame().GetPlayer().Kill();
	}
}

void Boss::Boss::Draw() const
{
	Enemy::Draw();
}

void Boss::Boss::UseCharAnimation(int n)
{
	m_enemyImage.UseImage(AT(m_animations, n).textureName, AT(m_animations, n).screenFrame_frame, AT(m_animations, n).begin, AT(m_animations, n).num);
}

void Boss::Boss::Start()
{
	m_started = true;
	popSkill();

	Game::GetGame().GetUILayerGameObjectManager().Insert(m_bossTimer);
	Game::GetGame().GetUILayerGameObjectManager().Insert(m_spellCardDisplay);
	Game::GetGame().GetMiscManagerTop().Insert(m_bossHpDisplay);
	Game::GetGame().GetUILayerGameObjectManager().Insert(m_spellScorer);
	Game::GetGame().GetMiscManagerBottom().Insert(m_bossMahoujin);
	Game::GetGame().GetUILayerGameObjectManager().Insert(m_spellCardCountDisplay);
	m_spellCardCountDisplay->InitStart(m_fullSpellCount);

	m_bossTimer->Show();

	m_needDeleteObjects = false;
}
#include "Haptic.h"
void Boss::Boss::Kill()
{
	double x, y;
	GameObjectWithDmslControl::GetPos(x, y);

	/*
			auto f = []() {Game::GetGame().GetEnemyBulletManager().ForceKillAll(); };
		f();
		for (int frame = 1; frame < 20; ++frame)
			Game::GetGame().GetTaskList().AddTask(f, frame);
			*/

	if (m_canBeKill) {


		m_canBeKill = false;

		m_bossHpDisplay->SetVisible(false);

		Game::GetGame().GetWarningLineSystem().ClearWithFadeoutAnimation();

		Game::GetGame().AddAfterEffect(new AfterEffect::NegaCircleZoom(m_collision.x, m_collision.y));
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
		auto pDreamWater = new AfterEffect::DreamWaterWave(x, y);
		pDreamWater->SetBlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		pDreamWater->SetColor(1, 1, 1, 1, 1, 1, 1, 1);
		Game::GetGame().AddAfterEffect(pDreamWater);

		//Game::GetGame().GetEnemyManager().EnableCreateByCSV(true);
		Basic::Sound::SeManager::Play("enep01", x);
		Basic::Haptic::Play(500);


		//连续清除20帧
		auto f = []() {Game::GetGame().GetEnemyBulletManager().ForceKillAll(); };
		f();

		for (int frame = 1; frame < 80; ++frame)
			Game::GetGame().GetTaskList().AddTask(f, frame);
		

		if (m_outAnimation != -1) {
			m_hp = 9999999;
			UseCharAnimation(m_outAnimation);
			m_enemyImage.SetFlip(false, true);
			m_taskList.AddTask([this]() {
				m_died = true;
				Game::GetGame().SetBossPosY(-9999);
				m_bossTimer->Kill();
				m_spellCardDisplay->Kill();
				m_bossHpDisplay->Kill();
				m_bossMahoujin->Kill();
				m_spellCardCountDisplay->Kill();
				m_spellScorer->Kill();

				m_startBossTask->Stop();
			}, 100);

			double px, py;
			GameObjectWithDmslControl::GetPos(px, py);
			GameObjectWithDmslControl::SetTask({
				GameObjectWithDmslControl::Task::MOVE,
				px,py,
				WIDTH / 2 + 100,0,
				100,1
			});
		}
		else {
			m_died = true;
			Game::GetGame().SetBossPosY(-9999);
			m_bossTimer->Kill();
			m_spellCardDisplay->Kill();
			m_bossHpDisplay->Kill();
			m_bossMahoujin->Kill();
			m_spellCardCountDisplay->Kill();
			m_spellScorer->Kill();

			m_startBossTask->Stop();
		}
	}
}

void Boss::Boss::SpellCardOver()
{
	m_spellScorer->SpellCardEnd();
}

void Boss::Boss::CallPrivateUnit(int unit, const std::shared_ptr<const Dmsl::DmslVarying>& varying)
{
	auto p = std::make_unique<Dmsl::DmslUnit>(*m_dvm, unit, varying);
	p->SetUserData(this);
	p->Call();
	m_readyToInsert.push(std::move(p));
}