#include "Precompiled.h"
#include "DmslLib.h"
#include <cmath>
#include "Game.h"
#include "GameObjectManager.h"
#include "Enemy.h"
#include "CircleBullet.h"
#include "BulletStyleManager.h"
#include "Haptic.h"
#include "Grid.h"
#include "ReisenEye.h"

static double lib_rand01(double* args, void*) {
	return Game::GetGame().GetInputer().Rand1();
}
static double lib_rand11(double* args, void*) {
	return Game::GetGame().GetInputer().Rand2();
}
static double lib_randAngle(double* args, void*) {
	return double(Game::GetGame().GetInputer().Rand1() * 360);
}

static double lib_cos(double* args, void*) {
	return cos(args[0] * M_PI / 180);
}

static double lib_sin(double* args, void*) {
	return sin(args[0] * M_PI / 180);
}

static double lib_tan(double* args, void*) {
	return tan(args[0] * M_PI / 180);
}

static double lib_sqrt(double* args, void*) {
	return sqrt(args[0]);
}

static double lib_angle(double* args, void*) {
	return atan2(args[1] - args[3], args[0] - args[2]) * 180 / M_PI;
}

static double lib_mix(double* args, void*) {
	return (args[1] - args[0]) * args[2] + args[0];
}

static double lib_abs(double *args, void*) {
	return *args > 0 ? *args : -*args;
}

static double lib_a2r(double* arg, void*) {
	return *arg * M_PI / 180;
}

static double lib_r2a(double* arg, void*) {
	return *arg * 180 / M_PI;
}

static double lib_MixWithFunc(double* args, void*) {
	switch ((int)args[0]) {
	case 0:
		return Mix(args[1], args[2], args[3]);
	case 1:
		return Mix(args[1], args[2], SlowMotion(args[3]));
	case 2:
		return Mix(args[1], args[2], SlowMotion2(args[3]));
	case 3:
		return Mix(args[1], args[2], SlowMotion3(args[3]));
	case 4:
		return Mix(args[1], args[2], SlowMotion4(args[3]));
	case 5:
		return Mix(args[1], args[2], SlowMotion5(args[3]));
	};
	return -1;
}

#include "Log.h"
static void lib_print(double* args, void* v) {
	Log("Dmsl Debug:%f,%x", *args, (unsigned)v);
}

void Dmsl::LoadMathLibrary(DmslVirtualMachine & dvm)
{
	dvm.LinkCFunc("Rand01", &lib_rand01);
	dvm.LinkCFunc("Rand11", &lib_rand11);
	dvm.LinkCFunc("RandAngle", &lib_randAngle);
	dvm.LinkCFunc("Sin", &lib_sin);
	dvm.LinkCFunc("Cos", &lib_cos);
	dvm.LinkCFunc("Tan", &lib_tan);
	dvm.LinkCFunc("Sqrt", &lib_sqrt);
	dvm.LinkCFunc("Angle", &lib_angle);
	dvm.LinkCFunc("Abs", &lib_abs);
	dvm.LinkCFunc("Mix", &lib_mix);
	dvm.LinkCFunc("MixWithFunc", &lib_MixWithFunc);
	dvm.LinkCFunc("R2A", &lib_r2a);
	dvm.LinkCFunc("A2R", &lib_a2r);
	dvm.LinkCMet("Print", &lib_print);
}

static double libGO_GetGameTime(double*, void*) {
	return Game::GetGame().GetGameTime();
}

static void libGO_KillSelf(double* args, void* ptr) {
	((GameObjectWithDmslControl*)ptr)->Kill();
}

static double libGO_GetX(double* args, void* ptr) {
	double x, y;
	((GameObjectWithDmslControl*)ptr)->GetPos(x, y);
	return x;
}

static double libGO_GetY(double* args, void* ptr) {
	double x, y;
	((GameObjectWithDmslControl*)ptr)->GetPos(x, y);
	return y;
}

static double libGO_GetLife(double* args, void* ptr) {
	double l = (double)((GameObjectWithDmslControl*)ptr)->GetLife();
	return l + 1.0;
}

static double libGO_GetPlayerX(double* args, void*) {
	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	return x;
}

static double libGO_GetPlayerY(double* args, void*) {
	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	return y;
}

static double libGO_GetSpyAngle(double* args, void* ptr) {
	double x, y, x2, y2;
	Game::GetGame().GetPlayer().GetPos(x, y);
	((GameObjectWithDmslControl*)ptr)->GetPos(x2, y2);
	return atan2(y - y2, x - x2) * 180 / M_PI;
}

static double libGO_GetAngleFromSelf(double* args, void* ptr) {
	double x, y;
	((GameObjectWithDmslControl*)ptr)->GetPos(x, y);
	return atan2(args[1] - y, args[0] - x) * 180 / M_PI;// +double(M_PI);
}

static void libGO_MoveTo(double* args, void* ptr) {
	GameObjectWithDmslControl::Task task;
	task.type = GameObjectWithDmslControl::Task::MOVE;
	((GameObjectWithDmslControl*)ptr)->GetPos(task.old_x, task.old_y);
	task.frameTimes = (uint32_t)args[1];
	task.trg_x = args[2];
	task.trg_y = args[3];
	task.slowMotion = args[0];
	((GameObjectWithDmslControl*)ptr)->SetTask(task);
}

static void libGO_AMoveTo(double* args, void* ptr) {
	GameObjectWithDmslControl::Task task;
	task.type = GameObjectWithDmslControl::Task::MOVE;
	((GameObjectWithDmslControl*)ptr)->GetPos(task.old_x, task.old_y);
	task.frameTimes = (uint32_t)args[1];
	task.trg_x = task.old_x + args[2];
	task.trg_y = task.old_y + args[3];
	task.slowMotion = args[0];
	((GameObjectWithDmslControl*)ptr)->SetTask(task);
}

static double libGO_BeatFrame(double*, void*) {
	const int frameBeat = Basic::Sound::BgmPlayer::GetBgmData().frame_beat;
	const int frameType = Basic::Sound::BgmPlayer::GetBgmData().beatType;

	const int mod = Game::GetGame().GetGameTime() % (frameBeat * frameType);

	if (mod == 0)
		return 0;
	else if (mod == frameBeat)
		return 1;
	else if (mod == 2 * frameBeat)
		return 2;
	else if (mod == 3 * frameBeat)
		return 3;
	else
		return -1;
}

static void libGO_AngleChange(double* args, void* ptr) {
	GameObjectWithDmslControl::Task task;
	task.type = GameObjectWithDmslControl::Task::ANGLE_CHANGE;
	task.old_x = ((GameObjectWithDmslControl*)ptr)->GetAngle();
	task.frameTimes = (uint32_t)args[1];
	task.trg_x = args[2] * M_PI / 180;
	task.slowMotion = args[0];
	((GameObjectWithDmslControl*)ptr)->SetTask(task);
}

#include "Boss.h"
static void libGO_CreateEnemy(double* a, void* obj) {
	StageData::EnemyData ed;
	ed.birth_time = 0;

	ed.minScreenLife = (int)a[5];
	ed.y = a[4];
	ed.x = a[3];
	ed.dmslUnit = (int)a[2];
	ed.hp = a[1];
	ed.style = (int)a[0];

	auto newEnemy = Enemy::CreateEnemy(ed.style, static_cast<GameObjectWithDmslControl*>(obj)->GetVMContext());

	Boss::Boss* boss = nullptr;
	if (Game::GetGame().HaveBoss()) {
		boss = Boss::Boss::GetBoss();
	}

	newEnemy->Create(ed, true, boss);
	Game::GetGame().GetTaskList().AddTask([newEnemy]() {
		Game::GetGame().GetEnemyManager().InsertEnemy(newEnemy);
	}, 0);
}

static void libGO_CreateEnemy2(double* a, void* obj) {
	if (Game::GetGame().GetEnemyManager().GetEnemyCSVEnabled())
		libGO_CreateEnemy(a, obj);
}

static void libGO_CreateCircleBullet(double* a, void* ptr) {
	int dmslProgram = (int)a[2];
	int subStyle = (int)a[1];
	int style = (int)a[0];
	double x, y;
	((GameObjectWithDmslControl*)ptr)->GetPos(x, y);
	auto varyingIn = Dmsl::DmslUnit::GetRuningUnit()->GetVaryingOut();

	auto p = new EnemyBullet::CircleBullet;
	p->SetVMContext(((GameObjectWithDmslControl*)ptr)->GetVMContext());
	p->Create(x, y, style, subStyle, dmslProgram, 0, false, varyingIn);
	Game::GetGame().GetTaskList().AddTask([p]() {
		Game::GetGame().GetEnemyBulletManager().Insert(p);
	}, 1);
}

static void libGO_CreateCircleBullet2(double* a, void* u) {
	int style = (int)a[2];
	double x, y;
	((GameObjectWithDmslControl*)u)->GetPos(x, y);
	x += a[0];
	y += a[1];
	/*Collision::Circle test;
	((GameObjectWithDmslControl*)u)->GetPos(test.x, test.y);
	test.x += a[0];
	test.y += a[1];*/

	//进行碰撞检测，并决定是否发射
	//test.radius = EnemyBullet::BulletStyleManager::GetMgr().GetStyle(style).collArg1 * 2;
	//if (!Collision::CheckC2C(test, Game::GetGame().GetPlayer().GetDeathCollision())) {
		int layer = int(a[5]);
		int dmslProgram = (int)a[4];
		int subStyle = (int)a[3];
		auto varyingIn = Dmsl::DmslUnit::GetRuningUnit()->GetVaryingOut();
		auto p = new EnemyBullet::CircleBullet;
		p->SetVMContext(((GameObjectWithDmslControl*)u)->GetVMContext());
		p->Create(x, y, style, subStyle, dmslProgram, 0, false, varyingIn);

		Game::GetGame().GetTaskList().AddTask([p, layer]() {
			Game::GetGame().GetEnemyBulletManager().Insert(p, layer);
		}, 0);
	//}
}

static void libGO_CreateCircleBullet3(double* a, void* u) {

	double x, y;
	((GameObjectWithDmslControl*)u)->GetPos(x, y);
	x += a[1];
	y += a[2];
	int style = (int)a[3];
	/*Collision::Circle test;
	((GameObjectWithDmslControl*)u)->GetPos(test.x, test.y);
	test.x += a[1];
	test.y += a[2];

	//进行碰撞检测，并决定是否发射
	test.radius = EnemyBullet::BulletStyleManager::GetMgr().GetStyle(style).collArg1 * 2;
	if (!Collision::CheckC2C(test, Game::GetGame().GetPlayer().GetDeathCollision())) {*/
		int dmslProgram = (int)a[5];
		int minOut = (int)a[0];
		int subStyle = (int)a[4];
		int layer = int(a[6]);
		auto varyingIn = Dmsl::DmslUnit::GetRuningUnit()->GetVaryingOut();

		auto p = new EnemyBullet::CircleBullet;
		p->SetVMContext(((GameObjectWithDmslControl*)u)->GetVMContext());
		p->Create(x, y, style, subStyle, dmslProgram, minOut, false, varyingIn);
		Game::GetGame().GetTaskList().AddTask([p, layer]() {
			Game::GetGame().GetEnemyBulletManager().Insert(p, layer);
		}, 0);
	//}
}

static void libGO_CreateCircleBullet4(double* a, void* u) {
	int style = (int)a[3];
	/*Collision::Circle test;
	test.x = a[1];
	test.y = a[2];
	test.radius = EnemyBullet::BulletStyleManager::GetMgr().GetStyle(style).collArg1 * 2;
	if (!Collision::CheckC2C(test, Game::GetGame().GetPlayer().GetDeathCollision())) {*/
		int layer = int(a[6]);
		int dmslProgram = (int)a[5];
		int subStyle = (int)a[4];
		int minOut = (int)a[0];
		auto varyingIn = Dmsl::DmslUnit::GetRuningUnit()->GetVaryingOut();

		auto p = new EnemyBullet::CircleBullet;
		p->SetVMContext(((GameObjectWithDmslControl*)u)->GetVMContext());
		p->Create(a[1], a[2], style, subStyle, dmslProgram, minOut, false, varyingIn);
		Game::GetGame().GetTaskList().AddTask([p, layer]() {
			Game::GetGame().GetEnemyBulletManager().Insert(p, layer);
		}, 0);
	//}
}

static void libGO_SpeedChange(double* args, void* ptr) {
	GameObjectWithDmslControl::Task task;
	task.type = GameObjectWithDmslControl::Task::SPEED_CHANGE;
	task.old_x = ((GameObjectWithDmslControl*)ptr)->GetSpeed();
	task.frameTimes = (uint32_t)args[1];
	task.trg_x = args[2];
	task.slowMotion = args[0];
	((GameObjectWithDmslControl*)ptr)->SetTask(task);
}

static void libGO_Haptic(double* args, void*) {
	Basic::Haptic::Play(*args);
}

static void libGO_Grid(double* args, void* obj) {
	double x, y;
	static_cast<GameObjectWithDmslControl*>(obj)->GetPos(x, y);
	auto p = new EnemyBullet::Grid;
	p->SetVMContext(static_cast<GameObjectWithDmslControl*>(obj)->GetVMContext());
	p->Create(*args, x, y, Dmsl::DmslUnit::GetRuningUnit()->GetVaryingOut());
	Game::GetGame().GetTaskList().AddTask([p]() {
		Game::GetGame().GetEnemyBulletManager().Insert(p);
	}, 0);
}

static void libGO_Grid2(double* args, void* obj) {
	double x, y;
	static_cast<GameObjectWithDmslControl*>(obj)->GetPos(x, y);
	auto p = new EnemyBullet::Grid;
	p->SetVMContext(static_cast<GameObjectWithDmslControl*>(obj)->GetVMContext());
	p->Create(args[2], x + args[0], y + args[1], Dmsl::DmslUnit::GetRuningUnit()->GetVaryingOut());
	Game::GetGame().GetTaskList().AddTask([p]() {
		Game::GetGame().GetEnemyBulletManager().Insert(p);
	}, 0);
}

static void libGO_Grid3(double* args, void* obj) {
	auto p = new EnemyBullet::Grid;
	p->Create(args[2], args[0], args[1], Dmsl::DmslUnit::GetRuningUnit()->GetVaryingOut());
	p->SetVMContext(static_cast<GameObjectWithDmslControl*>(obj)->GetVMContext());
	Game::GetGame().GetTaskList().AddTask([p]() {
		Game::GetGame().GetEnemyBulletManager().Insert(p);
	}, 0);
}

static void libGO_SetPos(double* args, void* obj) {
	static_cast<GameObjectWithDmslControl*>(obj)->SetPos(args[0], args[1]);
}

static void libGO_CSShoot(double* args, void* obj) {
	double style = args[0];
	double subStyle = args[1];
	double dmsl = args[2];
	double x, y;
	static_cast<GameObjectWithDmslControl*>(obj)->GetPos(x, y);
	static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().Execute(
		[=](double angle, double offset_x, double offset_y) {
		Dmsl::DmslUnit::GetRuningUnit()->SetVaryingOut(0, angle);
		double nargs[] = { static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().minLiveOutScreen,offset_x,offset_y,style,subStyle,dmsl,static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().layer };
		libGO_CreateCircleBullet3(nargs, obj);
	}
	, x, y);
}

static void libGO_CSCreateEnemy(double* args, void* obj) {
	double a1 = args[0];
	double a2 = args[1];
	double a3 = args[2];
	double a4 = args[3];
	double x, y;
	static_cast<GameObjectWithDmslControl*>(obj)->GetPos(x, y);
	static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().Execute(
		[=](double angle, double offset_x, double offset_y) {
		Dmsl::DmslUnit::GetRuningUnit()->SetVaryingOut(0, angle);
		double x, y;
		static_cast<GameObjectWithDmslControl*>(obj)->GetPos(x, y);
		x += offset_x;
		y += offset_y;
		double nargs[] = { a1,a2,a3,x,y,a4 };
		libGO_CreateEnemy(nargs, obj);
	}
	, x, y);
}

static void libGO_CSCreateEnemy2(double* args, void* obj) {
	if (Game::GetGame().GetEnemyManager().GetEnemyCSVEnabled()) {
		libGO_CSCreateEnemy(args, obj);
	}
}

static void libGO_CSGrid(double* args, void* obj) {
	double dmsl = args[0];
	double x, y;
	static_cast<GameObjectWithDmslControl*>(obj)->GetPos(x, y);
	static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().Execute(
		[=](double angle, double offset_x, double offset_y) {
		Dmsl::DmslUnit::GetRuningUnit()->SetVaryingOut(0, angle);
		double nargs[] = { offset_x,offset_y,dmsl };
		libGO_Grid2(nargs, obj);
	}
	, x, y);
}

static void libGO_CSSetWays(double* args, void* obj) {
	static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().ways = *args;
}

static void libGO_CSSetAngleSpy(double* args, void* obj) {
	static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().spy = *args > 0;
}

static void libGO_CSSetAngle(double* args, void* obj) {
	static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().angle = *args;
}

static void libGO_CSSetCircleAngle(double* args, void* obj) {
	static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().vecangle = *args;
}

static void libGO_CSSetRange(double* args, void* obj) {
	static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().range = *args;
}

static void libGO_CSSetRadius(double* args, void* obj) {
	static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().radius = *args;
}

static void libGO_CSSetLayer(double* args, void* obj) {
	static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().layer = *args;
}

static void libGO_CSSetMinLiveOutScreen(double* args, void* obj) {
	static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().minLiveOutScreen = *args;
}

static void libGO_WarningLine(double* arg, void*) {
	Game::GetGame().GetWarningLineSystem().CreateLineFromPoint(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6]);
}

static void libGO_WarningLineCrossScreen(double* arg, void*) {
	Game::GetGame().GetWarningLineSystem().CreateLineCrossScreen(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6]);
}

static void libGO_AddItemBigPower(double* args, void* obj) {
	double x, y;
	static_cast<GameObjectWithDmslControl*>(obj)->GetPos(x, y);
	if (Game::GetGame().GetItemMgr().GetEnabled())
		Game::GetGame().GetItemMgr().AddItem(Item::ItemType::BigPower, x, y, *args);
}

#include "Bullet.h"
static void libGO_KillBulletWithCircle(double* args, void* obj) {
	Collision::Circle cir = { args[0],args[1],args[2] };

	for (auto& bullet : *(Game::GetGame().GetEnemyBulletManager().begin() + int(args[3]))) {
		if (((EnemyBullet::Bullet&)(*bullet)).CollCheckWithCircle(cir))
			bullet->Kill();
	}
}

static void libGO_ReisenEyes(double*, void*) {
	Game::GetGame().GetMiscManagerBottom().Insert(new ReisenEye);
}

#include "LimitedLazer.h"
static void libGO_LLazer(double* args, void* obj) {
	double x, y;
	static_cast<GameObjectWithDmslControl*>(obj)->GetPos(x, y);

	auto newLazer = new EnemyBullet::LimitedLazer;
	newLazer->Create(x, y, args[0], false, args[1], Dmsl::DmslUnit::GetRuningUnit()->GetVaryingOut(), args[2]);

	Game::GetGame().GetEnemyBulletManager().Insert(newLazer);
}

static void libGO_LLazer2(double* args, void* obj) {
	double x, y;
	static_cast<GameObjectWithDmslControl*>(obj)->GetPos(x, y);

	x += args[0];
	y += args[1];

	auto newLazer = new EnemyBullet::LimitedLazer;
	newLazer->Create(x, y, args[2], false, args[3], Dmsl::DmslUnit::GetRuningUnit()->GetVaryingOut(), args[4]);

	Game::GetGame().GetEnemyBulletManager().Insert(newLazer, args[5]);
}

static void libGO_LLazer3(double* args, void* obj) {
	auto newLazer = new EnemyBullet::LimitedLazer;
	newLazer->Create(args[0], args[1], args[2], false, args[3], Dmsl::DmslUnit::GetRuningUnit()->GetVaryingOut(), args[4]);

	Game::GetGame().GetEnemyBulletManager().Insert(newLazer, args[5]);
}

static void libGO_CSLLazer(double* args, void* obj) {
	double style = args[0];
	double dmsl = args[1];
	int live = args[2];
	double x, y;
	static_cast<GameObjectWithDmslControl*>(obj)->GetPos(x, y);
	static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().Execute(
		[=](double angle, double offset_x, double offset_y) {
		Dmsl::DmslUnit::GetRuningUnit()->SetVaryingOut(0, angle);
		double nargs[] = { offset_x,offset_y,style,dmsl,live,static_cast<GameObjectWithDmslControl*>(obj)->GetShooter().layer };
		libGO_LLazer2(nargs, obj);
	}
	, x, y);
}

#include "EnemyWarning.h"
static void libGO_EnemyWarn(double* args, void*) {
	Game::GetGame().GetMiscManagerBottom().Insert(new Enemy::EnemyWarning(*args));
}

#include "BossPowerUpEffectObject.h"
static void libGO_PowerUpEffect(double* args, void*)
{
	Game::GetGame().GetMiscManagerBottom().Insert(
		new Boss::BossPowerUpEffectObject(args[0], args[1])
	);
}

static void libGO_PowerUpEffectFollowPlayer(double* args, void*)
{
	Game::GetGame().GetMiscManagerBottom().Insert(
		new Boss::BossPowerUpEffectObject()
	);
}


static void libGO_SetSpeedMul(double* args,void* obj)
{
	static_cast<GameObjectWithDmslControl*>(obj)->SetSpeedMul(args[0], args[1]);
}

void Dmsl::LoadGameObjectLibrary(DmslVirtualMachine & s, GameObjectLibraryLinkTable& tab)
{
	//tab.level = s.GetLoc("level");
	tab.speed = s.GetLoc("speed");
	tab.angle = s.GetLoc("angle");
	tab.interfaceDied = s.GetModuleHandle("died");

	s.LinkCMet("EnemyWarn", &libGO_EnemyWarn);

	s.LinkCMet("AddItemBigPower", &libGO_AddItemBigPower);

	s.LinkCMet("PowerUpEffect", &libGO_PowerUpEffect);
	s.LinkCMet("PowerUpEffectFollowPlayer", &libGO_PowerUpEffectFollowPlayer);

	s.LinkCMet("ReisenEye", &libGO_ReisenEyes);
	s.LinkCMet("WarningLine", &libGO_WarningLine);
	s.LinkCMet("WarningLineCrossScreen", &libGO_WarningLineCrossScreen);

	s.LinkCMet("LLazer", &libGO_LLazer);
	s.LinkCMet("LLazer2", &libGO_LLazer2);
	s.LinkCMet("LLazer3", &libGO_LLazer3);
	s.LinkCMet("CSLLazer", &libGO_CSLLazer);

	s.LinkCMet("KillBulletWithCircle", &libGO_KillBulletWithCircle);
	s.LinkCMet("CSSetMinLiveOutScreen", &libGO_CSSetMinLiveOutScreen);
	s.LinkCMet("CSShoot", &libGO_CSShoot);
	s.LinkCMet("CSGrid", &libGO_CSGrid);
	s.LinkCMet("CSCreateEnemy", &libGO_CSCreateEnemy);
	s.LinkCMet("CSCreateEnemy2", &libGO_CSCreateEnemy2);
	s.LinkCMet("CSSetWays", &libGO_CSSetWays);
	s.LinkCMet("CSSetAngle", &libGO_CSSetAngle);
	s.LinkCMet("CSSetAngleSpy", &libGO_CSSetAngleSpy);
	s.LinkCMet("CSSetShooterAngle", &libGO_CSSetCircleAngle);
	s.LinkCMet("CSSetRange", &libGO_CSSetRange);
	s.LinkCMet("CSSetRadius", &libGO_CSSetRadius);
	s.LinkCMet("CSSetLayer", &libGO_CSSetLayer);

	s.LinkCMet("KillSelf", &libGO_KillSelf);
	//s.LinkCMet("Sleep", &libGO_Sleep);

	s.LinkCFunc("GetGameTime", &libGO_GetGameTime);
	s.LinkCFunc("GetX", &libGO_GetX);
	s.LinkCFunc("GetY", &libGO_GetY);
	s.LinkCFunc("GetPlayerX", &libGO_GetPlayerX);
	s.LinkCFunc("GetPlayerY", &libGO_GetPlayerY);
	s.LinkCFunc("GetLife", &libGO_GetLife);
	s.LinkCFunc("GetSpyAngle", &libGO_GetSpyAngle);
	s.LinkCFunc("GetAngleFromSelf", &libGO_GetAngleFromSelf);
	s.LinkCFunc("BeatFrame", &libGO_BeatFrame);
	s.LinkCMet("MoveTo", &libGO_MoveTo);
	s.LinkCMet("AMoveTo", &libGO_AMoveTo);
	//s.LinkCMet("MoveToA", &libGO_MoveToA);
	//s.LinkCMet("AMoveToA", &libGO_AMoveToA);
	s.LinkCMet("AngleChange", libGO_AngleChange);
	s.LinkCMet("SpeedChange", libGO_SpeedChange);
	//s.LinkCMet("AngleChangeA", libGO_AngleChange);
	//s.LinkCMet("SpeedChangeA", libGO_SpeedChange);
	s.LinkCMet("CreateEnemy", libGO_CreateEnemy);
	s.LinkCMet("CreateEnemy2", libGO_CreateEnemy2);
	s.LinkCMet("Shoot", libGO_CreateCircleBullet);
	s.LinkCMet("Shoot2", libGO_CreateCircleBullet2);
	s.LinkCMet("Shoot3", libGO_CreateCircleBullet3);
	s.LinkCMet("Shoot4", &libGO_CreateCircleBullet4);

	s.LinkCMet("Grid", &libGO_Grid);
	s.LinkCMet("Grid2", &libGO_Grid2);
	s.LinkCMet("Grid3", &libGO_Grid3);

	s.LinkCMet("SetPos", &libGO_SetPos);

	s.LinkCMet("SetSpeedMul", &libGO_SetSpeedMul);

	//s.LinkCMet("TaskBegin", &libGO_TaskBegin);
	//s.LinkCMet("TaskEnd", &libGO_TaskEnd);
	s.LinkCMet("Haptic", &libGO_Haptic);
}

const int SE_ALERT = 0;
const int SE_CAT00 = 1;
const int SE_CAT01 = 2;
const int SE_CH00 = 3;
const int SE_CH01 = 4;
const int SE_GUN00 = 5;
const int SE_KIRA00 = 6;
const int SE_KIRA01 = 7;
const int SE_KIRA02 = 8;
const int SE_LAZER00 = 9;
const int SE_LAZER01 = 10;
const int SE_MSL = 11;
const int SE_POWER0 = 12;
const int SE_POWER1 = 13;
const int SE_SLASH = 14;
const int SE_TAN00 = 15;
const int SE_TAN01 = 16;
const int SE_TAN02 = 17;
const int SE_WARPL = 18;
const int SE_WARPR = 19;
const int SE_WATER = 20;

const static std::string se[] = {
	"alert",
	"cat00",
	"cat01",
	"ch00",
	"ch01",
	"gun00",
	"kira00",
	"kira01",
	"kira02",
	"lazer00",
	"lazer01",
	"msl",
	"power0",
	"power1",
	"slash",
	"tan00",
	"tan01",
	"tan02",
	"warpl",
	"warpr",
	"water"
};

void libSE_playSe(double* arg, void* obj) {
	double x = 0;
	if (obj) {
		double y;
		static_cast<GameObjectWithDmslControl*>(obj)->GetPos(x, y);
	}
	Basic::Sound::SeManager::Play(se[(int)*arg], x, 12);
}

void libSE_playSeHigh(double* arg, void* obj) {
	double x = 0;
	if (obj) {
		double y;
		static_cast<GameObjectWithDmslControl*>(obj)->GetPos(x, y);
	}
	Basic::Sound::SeManager::PlayHigh(se[(int)*arg], x, 12);
}

void Dmsl::LoadSePlayer(DmslVirtualMachine & dvm)
{
	dvm.LinkCMet("Se", libSE_playSe);
	dvm.LinkCMet("SeHigh", libSE_playSeHigh);
}