#include "Precompiled.h"
#include "Bullet.h"
#include "Game.h"
#include "CircleBullet.h"
#include "DmslLib.h"
#include "DmslLoader.h"

Dmsl::GameObjectLibraryLinkTable EnemyBullet::Bullet::m_dmslLinkTable;
std::unique_ptr<Dmsl::DmslVirtualMachine> EnemyBullet::Bullet::m_bulletScript;

static void libEB_StopDmsl(double*, void* ptr) {
	(dynamic_cast<EnemyBullet::Bullet*>((GameObject*)(ptr)))->StopDmsl();
}

static void libEB_CirFade(double* arg, void* ptr) {
	(dynamic_cast<EnemyBullet::CircleBullet*>(((GameObject*)ptr)))->Fade((int)arg[0], arg[2] / 255, (int)arg[1]);
}

static void libEB_CirColor(double* arg, void* ptr) {
	(dynamic_cast<EnemyBullet::CircleBullet*>((GameObject*)ptr))->ColorChange(arg[2] / 255, arg[3] / 255, arg[4] / 255, (int)arg[1], (int)arg[0]);
}

static void libEB_CirZoom(double* arg, void* ptr) {
	(dynamic_cast<EnemyBullet::CircleBullet*>((GameObject*)ptr))->Zoom((int)arg[0], arg[2], int(arg[1]));
}

static void libEB_EnableHDR(double* arg, void* ptr) {
	((EnemyBullet::Bullet*)ptr)->EnableHDR(true);
	//((EnemyBullet::Bullet*)ptr)->HDRSharpness(*arg);
}

static void libEB_DisableHDR(double* arg, void* ptr) {
	((EnemyBullet::Bullet*)ptr)->EnableHDR(false);
}

static void libEB_EnableTail(double*, void* ptr) {
	((EnemyBullet::Bullet*)ptr)->EnableTail(true);
}

static void libEB_DisableTail(double*, void* ptr) {
	((EnemyBullet::Bullet*)ptr)->EnableTail(false);
}

static void libEB_SetInvincible(double* arg, void* ptr) {
	((EnemyBullet::Bullet*)ptr)->SetInvincible(arg[0] > 0);
}

static void libEB_CirSetStyle(double* arg, void* ptr) {
	((EnemyBullet::CircleBullet*)ptr)->ChangeStyle((int)arg[0], (int)arg[1]);
}

static void libEB_CirSetImageAngleEnabled(double* arg, void* ptr) {
	((EnemyBullet::CircleBullet*)ptr)->SetImageAngleEnabled(*arg > 0);
}

static void libEB_CirSetImageAngle(double* arg, void* ptr) {
	((EnemyBullet::CircleBullet*)ptr)->SetImageAngle(*arg);
}

static void libEB_StopAllChange(double*, void* ptr) {
	((EnemyBullet::Bullet*)ptr)->GetDataActor().Clear();
}

static void libEB_CirSetZoom(double* arg, void* ptr) {
	((EnemyBullet::CircleBullet*)ptr)->SetZoom(*arg);
}

static void libEB_CirSetForcedColli(double* arg, void* ptr)
{
	((EnemyBullet::CircleBullet*)ptr)->SetForcedColli(*arg > 0);
}

static void libEB_CirAutoRotate(double*, void* ptr)
{
	((EnemyBullet::CircleBullet*)ptr)->EnableAutoRotate();
}

#include "LimitedLazer.h"

static void libEB_LLaSetLength(double* arg, void* ptr) {
	((EnemyBullet::LimitedLazer*)ptr)->SetLength(*arg);
}

static void libEB_LLaChangeLength(double* args, void* ptr) {
	((EnemyBullet::LimitedLazer*)ptr)->ChangeLength(args[0], args[1], args[2]);
}

static void libEB_LLaSetRotateCenter(double* args, void* ptr)
{
	((EnemyBullet::LimitedLazer*)ptr)->SetRotateCenter(1 - *args);
}

static void libEB_LLaSetWidth(double* arg, void* ptr) {
	((EnemyBullet::LimitedLazer*)ptr)->SetWidth(*arg);
}

static void libEB_LLaChangeWidth(double* args, void* ptr) {
	((EnemyBullet::LimitedLazer*)ptr)->ChangeWidth(args[0], args[1], args[2]);
}

static void libEB_LLaSetShowShooter(double* args, void* ptr)
{
	((EnemyBullet::LimitedLazer*)ptr)->SetShowShooter(*args > 0);
}

EnemyBullet::Bullet::~Bullet()
{
}
void EnemyBullet::Bullet::linkScript(Dmsl::DmslVirtualMachine& dmsl)
{
	dmsl.LinkCMet("StopDmsl", &libEB_StopDmsl);

	dmsl.LinkCMet("CirZoom", &libEB_CirZoom);
	dmsl.LinkCMet("CirFade", &libEB_CirFade);
	dmsl.LinkCMet("CirColor", &libEB_CirColor);
	dmsl.LinkCMet("CirSetStyle", &libEB_CirSetStyle);

	dmsl.LinkCMet("EnableHDR", &libEB_EnableHDR);
	dmsl.LinkCMet("DisableHDR", &libEB_DisableHDR);

	dmsl.LinkCMet("EnableTail", &libEB_EnableTail);
	dmsl.LinkCMet("DisableTail", &libEB_DisableTail);

	dmsl.LinkCMet("SetInvi", &libEB_SetInvincible);

	dmsl.LinkCMet("StopAllChange", &libEB_StopAllChange);
	dmsl.LinkCMet("CirSetZoom", &libEB_CirSetZoom);

	dmsl.LinkCMet("CirSetImageAngle", &libEB_CirSetImageAngle);
	dmsl.LinkCMet("CirSetImageAngleEnabled", &libEB_CirSetImageAngleEnabled);
	dmsl.LinkCMet("CirAutoRotate", &libEB_CirAutoRotate);
	dmsl.LinkCMet("CirSetForcedColli", &libEB_CirSetForcedColli);

	dmsl.LinkCMet("LLaSetLength", &libEB_LLaSetLength);
	dmsl.LinkCMet("LLaChangeLength", &libEB_LLaChangeLength);
	dmsl.LinkCMet("LLaSetWidth", &libEB_LLaSetWidth);
	dmsl.LinkCMet("LLaChangeWidth", &libEB_LLaChangeWidth);
	dmsl.LinkCMet("LLaSetRotateCenter", &libEB_LLaSetRotateCenter);
	dmsl.LinkCMet("LLaSetShowShooter", &libEB_LLaSetShowShooter);
}

void EnemyBullet::Bullet::SetLevel(StageLevel l)
{
	m_bulletScript->SetDMOVFSSelect((int)l);
}

#include "SnowRVReader.h"
void EnemyBullet::Bullet::LoadScript()
{
	//m_dmslLinkTable = linkTable;
	//m_bulletScript.reset(vm);
	//TODO:发布时去掉这些代码，并且只加载一个编译好的Bullets.elf

	Basic::SnowRVReader bulletList("StageData/BulletScriptList.rv");
	int scriptCount = bulletList.GetInt("SCRIPT_COUNT");

	Dmsl::DmslLoader loader;
	for (int i = 0; i < scriptCount; i++) {
		loader.IncludeMain(bulletList.GetString("_" + std::to_string(i)));
	}

	m_bulletScript = std::move(loader.CompileUnique());
	Dmsl::LoadMathLibrary(*m_bulletScript);	//链接数学库
	EnemyBullet::Bullet::linkScript(*m_bulletScript);	//链接敌人子弹库
	Dmsl::LoadSePlayer(*m_bulletScript);	//链接音效播放器
	Dmsl::LoadGameObjectLibrary(*m_bulletScript, m_dmslLinkTable);

	m_bulletScript->Ready();
}