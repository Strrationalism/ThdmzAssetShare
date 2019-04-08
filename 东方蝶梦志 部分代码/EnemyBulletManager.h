#pragma once
#include "GameObjectManager.h"
#include "Sprite2D.h"
#include "Collision.h"
#include <set>
namespace EnemyBullet {
	class EnemyBulletManager
	{
	private:
		std::vector<GameObjectManager> m_gom;
		std::set<int> m_playerDiedNoKillLayer;
	public:
		inline void SetLayerNum(size_t num) {
			m_gom.resize(num);
			for (auto& p : m_gom)
				p.Prep(4000);
		}

		inline void Update()
		{
			for (auto& p : m_gom)
				p.Update();
		}
		inline void Draw() const
		{
			for (auto& p : m_gom) {
				if (p.Size() > 0) {
					Basic::Video::Renderer2D::Sprite2D::ClearFastBuffer();
					p.Draw();
					Basic::Video::Renderer2D::Sprite2D::DrawFastBuffer();
				}
			}
		}

		void KillAll();

		void KillAllKeepNotKillLayer();

		void KillInCircle(Collision::Circle cir);

		void KillKeepNotKillLayerInCircle(Collision::Circle cir);

		void KillAllKeepNotKillLayerWithCenterPosition(double x, double y);
		void KillAllWithCenterPosition(double x, double y);

		inline void Reset() {
			m_playerDiedNoKillLayer.clear();
			ForceKillAll();
		}

		inline void SetNoKillLayerByPlayerDied(int l) {
			m_playerDiedNoKillLayer.insert(l);
		}

		void ForceKillAll();

		inline void Insert(GameObject* p, int level = 0)
		{
			m_gom.at(level).Insert(p);
		}

		inline size_t Size() const
		{
			size_t ret = 0;
			for (auto& p : m_gom)
				ret += p.Size();
			return ret;
		}

		inline auto begin() {
			return m_gom.begin();
		}

		inline auto end() {
			return m_gom.end();
		}
	};
}
