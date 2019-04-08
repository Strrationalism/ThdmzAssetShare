#include "Precompiled.h"
#include "BossTask.h"
#include "Boss.h"
#include "Game.h"
#include <sstream>
#include "DmslLoader.h"
#include "UnicodeSite.h"
#include "DMSLUtils.h"
#include "StageData.h"
#include "CtxFont.h"
#include "BossWithWBEffectSpellCard.h"
#include "MarisaBomb.h"
#include "BossWithWBEffectSpellCard.h"
#include "SpellCardDatabase.h"
StageTask::CreateBoss* StageTask::CreateBoss::m_lastBossTask = nullptr;

Basic::Video::Renderer2D::CtxFont scFont("Font2.fnt", "Font2");

static void libBOSS_UseCharacterAnimation(double* arg, void* bossObj) {
	((Boss::Boss*)bossObj)->UseCharAnimation((int)*arg);
}

static void libBOSS_EffectPowerUp(double*, void* boss) {
	((Boss::Boss*)boss)->EffectPowerUp();
}

static void libBOSS_SetBackground(double* a, void*) {
	auto p = Background::BaseBackground::CreateBackground(int(*a));
	p->Init();
	p->Reset();
	p->SkipToBossMode();
	Game::GetGame().SetBackground(std::move(p));
}

static void libBOSS_MarisaBomb(double* args, void* ptr) {
	double x, y;
	((Boss::Boss*)ptr)->GetPos(x, y);
	((Boss::Boss*)ptr)->DisableFlip();
	Basic::Sound::SeManager::PlayHigh("nep00", x / 16);
	Game::GetGame().GetMiscManagerTop().Insert(
		new Char::Marisa::MarisaBomb(true, false, x, y,args[1],args[0]));
}

static void libBOSS_WBEffect(double* args, void*)
{
	static_cast<Boss::BossWithWBEffectSpellCard*>(Boss::Boss::GetBoss())->SetWB(*args);
}

static std::string applyChar(std::string str)
{
	while (1)
	{
		auto p = str.find('&');
		if (p == std::string::npos) break;
		auto before = str.substr(0, p);
		auto after = str.substr(p + 1, str.length() - p - 1);

		str = before + Game::GetGame().GetPlayer().GetChar()->GetName() + after;
	}
	return str;
}

StageTask::CreateBoss::CreateBoss(bool withWBEffect):
	m_withWBEffect{ withWBEffect }
{
}


void StageTask::CreateBoss::Create(Basic::CSVReader & stageData, const StageData& stage)
{
	m_lastBossTask = this;

	

	Dmsl::DmslLoader loader;
	loader.Include("Math");
	loader.Include("Se");
	loader.Include("GameObject");
	loader.Include("EnemyLib");
	loader.Include("BossLib");
	loader.IncludeMain(applyChar(stageData.PopString()));	//C列，Dmsl程序
	std::shared_ptr<Dmsl::DmslVirtualMachine> pDvm = loader.CompileShared();
	auto pBoss = m_withWBEffect ? new Boss::BossWithWBEffectSpellCard(pDvm) : new Boss::Boss(pDvm);
	Dmsl::LoadMathLibrary(*pDvm);
	Dmsl::LoadGameObjectLibrary(*pDvm, *(pBoss->GetDmslLinkTable()));
	Dmsl::LoadSePlayer(*pDvm);
	pDvm->LinkCMet("UseCharacterAnimation", &libBOSS_UseCharacterAnimation);
	pDvm->LinkCMet("EffectPowerUp", &libBOSS_EffectPowerUp);
	pDvm->LinkCMet("MarisaBomb", &libBOSS_MarisaBomb);
	pDvm->LinkCMet("WBEffect", &libBOSS_WBEffect);
	pDvm->LinkCMet("SetBackground", &libBOSS_SetBackground);

	pDvm->SetDMOVFSSelect((int)stage.level);
	Enemy::Enemy::LinkEnemyDmslLib(*pDvm);
	pDvm->Ready();

	m_bossObj = pBoss;

	Basic::CSVReader skill;
	skill.Load(applyChar(stageData.PopString()));	//D列，招数表
	while (!skill.IsLastLine()) {
		auto pSkill = new Boss::Boss::Skill;
		pSkill->hpDisplayLowLimit = skill.PopDouble() / 100;
		pSkill->hp = skill.PopDouble();

		pSkill->timeLimit = skill.PopInt();
		if (skill.PopInt())
			pSkill->timeSpellCard = true;
		else
			pSkill->timeSpellCard = false;
		pSkill->dmslUnit = std::make_unique<Dmsl::DmslUnit>(*pDvm, (unsigned)skill.PopInt());
		pSkill->dmslUnit->SetUserData(pBoss);

		auto spellCardStr = skill.PopString();

		pSkill->isSpellCard = skill.PopInt() == 1;
		if (pSkill->isSpellCard) {
			std::vector<std::string> scNames;
			Dmsl::Utils::Split(spellCardStr, scNames, ':');
			if (scNames.size() != 4) Crash(L"符卡名不足4个。");
			const int spellCard = atoi(scNames[(int)stage.level].c_str());
			const int spellHash = SpellCardDatabase::GetHash(spellCard, stage.level, 
				Game::GetGame().GetPlayer().GetChar()->GetName() == std::string("Reimu") ? SpellCardDatabase::Charater::Reimu : SpellCardDatabase::Charater::Marisa);

			pSkill->spellHash = spellHash;

			pSkill->spellCardNameDisplay = std::make_shared<Basic::Video::Renderer2D::Sprite2DWStreamLayout>(
				SDL_Rect{-640,0,1280,48},
				Basic::Video::Renderer2D::Sprite2DWStreamLayout::LayoutMode::CenterTop
				);
			pSkill->spellCardNameDisplay->SetSpriteSizeOffset(15, 0);
			scFont.SetSprite2DWStreamToString(Basic::UnicodeText(spellCard), *pSkill->spellCardNameDisplay);
			for (auto& p : *pSkill->spellCardNameDisplay)
			{
				p->SetZoom(1);
			}
		}

		pSkill->useNewHPLine = (skill.PopInt() == 1);

		pSkill->score = skill.PopInt();

		pBoss->AddSkill(pSkill);
		skill.NextLine();
	}

	auto lhName = applyChar(stageData.PopString());	//E列，立绘纹理名
	if (!lhName.empty()) {
		pBoss->GetSpellCardStandImage().UseImage(lhName, stageData.PopInt());	//F列，立绘编号
	}
	else
		stageData.PopString();

	auto bgImages = applyChar(stageData.PopString());	//G列，背景图表
	if (!bgImages.empty()) {
		//加载背景
		pBoss->GetBgCollector().LoadBossBG(bgImages);
	}

	//加载点阵动画列表
	Basic::CSVReader animations;
	animations.Load(applyChar(stageData.PopString()));	//H列，点阵动画数据列表
	Boss::Boss::AnimationData ani;
	while (!animations.IsLastLine()) {
		ani.textureName = animations.PopString();
		ani.screenFrame_frame = animations.PopInt();
		ani.begin = animations.PopInt();
		ani.num = animations.PopInt();
		pBoss->AddAnimation(ani);

		animations.NextLine();
	}

	//烟雾RGB
	double r, g, b;
	r = stageData.PopDouble();
	g = stageData.PopDouble();
	b = stageData.PopDouble();
	pBoss->SetFogColor(r, g, b);

	//入场出场动画
	int in, out;
	in = stageData.PopInt();
	out = stageData.PopInt();
	pBoss->SetInOutAnimation(in, out);
}

void StageTask::CreateBoss::Proc()
{
	if (!m_finishd) {
		((Boss::Boss*)m_bossObj)->FirstFrameRun();
		Game::GetGame().GetEnemyManager().InsertEnemy((Enemy::Enemy*)m_bossObj);
		m_finishd = true;
		//Game::GetGame().GetUI().GetDreamSystem().Enable(false);
	}
}

bool StageTask::CreateBoss::Finished()
{
	return m_finishd;
}

StageTask::CreateBoss::~CreateBoss()
{
	if (!m_finishd) {
		delete m_bossObj;
	}
}

void StageTask::StartBoss::Create(Basic::CSVReader & c, const StageData&)
{
	m_bossObj = CreateBoss::GetLastBossTask()->GetBossObj();
}

void StageTask::StartBoss::Proc()
{
	if (m_first) {
		Game::GetGame().GetItemMgr().GetAll();
		m_first = false;
		((Boss::Boss*)m_bossObj)->SetStartBossTask(this);
		((Boss::Boss*)m_bossObj)->Start();
		Game::GetGame().GetItemMgr().SetEnabled(false);
	}
}

bool StageTask::StartBoss::Finished()
{
	return m_finished;
}

void StageTask::StartBoss::Stop() {
	m_finished = true;
	//Game::GetGame().GetUI().GetDreamSystem().Enable(true);
	Game::GetGame().GetItemMgr().SetEnabled(true);
}

void StageTask::BossPowerEffect::Create(Basic::CSVReader & c, const StageData&)
{
	m_x = c.PopDouble();
	m_y = c.PopDouble();
}

void StageTask::BossPowerEffect::Proc()
{
	m_frames++;
	if (m_frames >= 20) m_finished = true;

	Game::GetGame().GetParticleTop().AddDot(3, [](Basic::Video::Renderer3D::Particle3D::Dot3D& dot) {
		dot.alpha = 0.001;
		dot.angle = 0;
		dot.speed = 0;

		dot.r = Rand() * 0.2;
		dot.g = Rand() * 0.2;
		dot.b = Rand() * 0.2;

		dot.userdata[0] = dot.x;	//圆心x位置
		dot.userdata[1] = dot.y;	//圆心y位置
		dot.userdata[2] = Rand() * 250;	//圆半径
		dot.userdata[3] = Rand() * M_PI * 2;	//与圆心角度
		dot.self_angle = 0;	//状态

		dot.size = 1000 * Rand();

		dot.blend_dst = GL_ONE;
	},
		[](Basic::Video::Renderer3D::Particle3D::Dot3D& dot, Basic::Video::Renderer3D::Particle3D&) {
		if (dot.self_angle == 1)  dot.alpha -= 0.03;
		else dot.alpha += 0.025;
		if (dot.alpha >= 1.0f) dot.self_angle = 1;
		if (dot.userdata[2] > 15) dot.userdata[2] -= 15;
		else dot.userdata[2] = 0;
		dot.userdata[3] += 0.05;

		dot.x = dot.userdata[2] * cos(dot.userdata[3]) + dot.userdata[0];
		dot.y = dot.userdata[2] * sin(dot.userdata[3]) + dot.userdata[1];

		dot.size -= 15;
	}, m_x, m_y, 75 / 2);
}

bool StageTask::BossPowerEffect::Finished()
{
	return m_finished;
}

void StageTask::BossAttachStage5DoremiBook::Create(Basic::CSVReader &, const StageData &)
{
}

void StageTask::BossAttachStage5DoremiBook::Proc()
{
	dynamic_cast<Boss::BossWithWBEffectSpellCard*>(Boss::Boss::GetBoss())->AttachStage5DoremiBook();
	m_finished = true;
}

bool StageTask::BossAttachStage5DoremiBook::Finished()
{
	return m_finished;
}
