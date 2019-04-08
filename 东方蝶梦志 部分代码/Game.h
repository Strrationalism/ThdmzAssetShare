#pragma once
#include "GameObjectManager.h"
#include "BaseInputer.h"
#include "Player.h"
#include "StageData.h"
#include "PlayerBulletManager.h"
#include "EnemyManager.h"
#include "TaskList.h"
#include "Particle3D.h"
#include "ItemManager.h"
#include "DataActor.h"
#include "StageTaskRunner.h"
#include "Sp2DObject.h"
#include "GameActBridge.h"
#include "UIManager.h"
#include "BaseAfterEffect.h"
#include "ScoreManager.h"
#include "GLTexture.h"
#include "EnemyBulletManager.h"
#include "WarningLineSystem.h"
#include "EnemyAnimation.h"
/* 游戏状态初始数据 */
struct GameInitData {
	/* Player数据 */
	double power;	//火力

	/* 生命数据 */
	uint16_t life, lifeKagera;
	
	/* 分数 */
	uint64_t score;

	/* 梦秤 */
	double dream;

	/* 玩家坐标 */
	double playerX, playerY;

	/* Graze */
	uint32_t graze;

	void WriteToStream(std::ostream&) const;
	void ReadFromStream(std::istream&);
};

/* 游戏逻辑中心 */
class Game final
{
private:
	std::optional<UI::UIManager> m_ui;

	ScoreManager m_score;

	Player m_player;	//一个玩家
	Inputer::BaseInputer* m_inputer = nullptr;	//输入器
	std::shared_ptr<StageData> m_stageData;	//关卡数据
	PlayerBulletManager m_playerBulletManager;	//我方子弹管理器
	Enemy::EnemyManager m_enemyManager;	//敌人管理器
	Enemy::EnemyAnimation m_enemyAnimation;	//敌人击破特效管理器

	int m_mainTimer;	//游戏主计时器

	Basic::Video::Renderer3D::Particle3D m_parTop;	//最上面的粒子系统
	Basic::Video::Renderer3D::Particle3D m_parBottom;	//游戏图层下面的粒子系统

	EnemyBullet::EnemyBulletManager m_enemyBulletManager;	//敌人子弹管理器
	GameObjectManager m_miscManager, m_miscManagerBottom;	//其它游戏物体管理器
	GameObjectManager m_inManager;

	Sp2DObject m_playerDeathCollDisplay, m_playerGrazeCollDisplay;	//死亡判定显示器和擦弹判定显示器

	Item::ItemManager m_items;

	Basic::TaskList m_taskList;
	Basic::DataActor m_dataActor;

	StageTask::StageTaskRunner m_runningStageTask;

	GameActBridge* m_gameActBridge = nullptr;

	std::vector<std::unique_ptr<AfterEffect::BaseAfterEffect>> m_afterEffects;

	WarningLineSystem m_warningLineSystem;

	bool m_drawStageBackground = true;

	int m_screenWidth, m_screenHeight;

	double m_bossPosY = -9999;	//-9999为关闭
	double m_bossPosYAlpha = 0;
	Basic::Video::Renderer2D::Sprite2D m_bossPosDisplay;

	void loadStageTaskFromStageData();

	std::vector<std::function<void()>> m_importantTask;

	std::optional<std::shared_ptr<Basic::Video::LowerRenderer::GLTexture>> m_mirror;
public:

	void Init();
	void Quit();
	void Reset();	//重设一切游戏逻辑

	inline void SetBossPosY(double y) {
		if (y == -9999 && m_bossPosY != -9999) {
			m_taskList.AddTask([this]() {
				m_bossPosY = -9999;
			}, 50);
			m_dataActor.Add(m_bossPosYAlpha, 0, 50, 1);
		}
		else if (m_bossPosY == -9999 && y != -9999) {
			m_dataActor.Add(m_bossPosYAlpha, 1, 50, 1);
			m_bossPosY = y;
		}
		else m_bossPosY = y;
	}

	inline bool HaveBoss() {
		return m_bossPosY != -9999;
	}

	void SetMirrorMode(bool b);
	bool GetMirrorMode() const;

	bool IsMirrorMode() const;

	void Draw();
	void Update();
	void SetInputer(Inputer::BaseInputer&);	//设置输入器
	void ReleaseKeys();	//取消所有的按键状态
	inline Inputer::BaseInputer& GetInputer() { return *m_inputer; }	//获取输入器

	inline void AddAfterEffect(AfterEffect::BaseAfterEffect* b) {
		m_afterEffects.push_back(std::unique_ptr<AfterEffect::BaseAfterEffect>(b));
	}

	inline void GetScreenSize(int& w, int& h) {
		w = m_screenWidth;
		h = m_screenHeight;
	}

	inline Player& GetPlayer() { return m_player; }	//获取玩家对象
	inline PlayerBulletManager& GetPlayerBulletManager() { return m_playerBulletManager; }	//获取玩家对象
	inline auto& GetEnemyBulletManager() { return m_enemyBulletManager; }	//获取敌人子弹管理器
	inline auto& GetEnemyManager() { return m_enemyManager; }

	void SetStageData(const std::shared_ptr<StageData>&);	//设置关卡数据
	const std::shared_ptr<StageData>& GetStageData() {	//获取关卡数据
		return m_stageData;
	}

	inline int GetGameTime() { return m_mainTimer; }	//获取当前游戏时间

	inline Basic::Video::Renderer3D::Particle3D& GetParticleTop() {	//获取最上方粒子系统
		return m_parTop;
	}
	inline Basic::Video::Renderer3D::Particle3D& GetParticleBottom() {	//获取下方粒子系统
		return m_parBottom;
	}

	inline Sp2DObject& GetPlayerDeathCollDisplay() {
		return m_playerDeathCollDisplay;
	}

	inline Sp2DObject& GetPlayerGrazeCollDisplay() {
		return m_playerGrazeCollDisplay;
	}

	inline GameObjectManager& GetUILayerGameObjectManager() {
		return m_miscManager;
	}

	inline void SetBackground(std::unique_ptr<Background::BaseBackground>&& bk)
	{
		m_stageData->background = std::move(bk);
	}

	inline auto& GetMiscManagerBottom() {
		return m_miscManagerBottom;
	}

	inline auto& GetMiscManagerTop() {
		return m_inManager;
	}

	inline Item::ItemManager& GetItemMgr() {
		return m_items;
	}

	inline Basic::TaskList& GetTaskList() {
		return m_taskList;
	}

	inline Basic::DataActor& GetDataActor() {
		return m_dataActor;
	}

	inline auto& GetEnemyAnimation()
	{
		return m_enemyAnimation;
	}

	inline void SetGameActBridge(GameActBridge* b) {
		m_gameActBridge = b;
	}

	inline GameActBridge* GetGameActBridge() {
		return m_gameActBridge;
	}

	inline void SetStageBackgroundEnabled(bool b) {
		m_drawStageBackground = b;
	}

	inline auto& GetUI() {
		return *m_ui;
	}

	inline auto& GetScoreManager() {
		return m_score;
	}

	inline auto GetLevel() {
		return m_stageData->level;
	}

	inline auto& GetWarningLineSystem() {
		return m_warningLineSystem;
	}

	void LoadFromInitData(GameInitData&);
	void WriteToInitData(GameInitData&);

	inline void AddImportantTask(const std::function<void()>& func)
	{
		m_importantTask.push_back(func);
	}

	std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> Screenshot() const;

	static Game& GetGame();
};
