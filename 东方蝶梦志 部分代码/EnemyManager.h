#pragma once
#include "StageData.h"
#include "Enemy.h"
#include "GameObjectManager.h"
#include <stack>
namespace Enemy {
	class EnemyManager final
	{
	private:
		std::shared_ptr<StageData> m_stageData;	//关卡数据
		int m_nextEnemy = 0;	//下一个敌人的编号

		GameObjectManager m_enemys;	//敌人容器

		bool m_canCreateByCSV = true;

	public:
		EnemyManager();
		void Reset();	//重设状态
		void ForceKillAll();	//强制杀除所有敌人
		void KillAll();	//杀除所有敌人
		void SetStageEnemyData(const std::shared_ptr<StageData>& e);	//设置敌人出现数据
		inline auto GetStageEnemyData() {
			return m_stageData;
		}

		inline auto GetEnemyCSVEnabled() {
			return m_canCreateByCSV;
		}

		//给出EnemtData创建敌人
		//BirthTime将会被自动忽略
		/*inline void InsertEnemy(const StageData::EnemyData& em)
		{
			auto newEnemy = CreateEnemy(em.style);
			newEnemy->Create(em);
			m_enemys.Insert(newEnemy);
		}*/

		//添加已创建好的敌人e
		inline void InsertEnemy(Enemy* e) {
			m_enemys.Insert(e);
		}

		void Update();
		void Draw() const;	//使用Sprite2D::UseShader

		auto Size() const {
			return m_enemys.Size();
		}

		auto& operator [](int n) {
			return dynamic_cast<Enemy&>(m_enemys[n]);
		}

		inline auto begin() {
			return m_enemys.begin();
		}

		inline auto end() {
			return m_enemys.end();
		}

		inline void EnableCreateByCSV(bool b) {
			m_canCreateByCSV = b;
		}
	};
}