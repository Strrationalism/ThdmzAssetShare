#pragma once
#include "Enemy.h"
namespace Enemy {
	namespace SpecialEnemy {
		class Enemy_14_to_18 : public Enemy
		{
		protected:
			Basic::Video::Renderer2D::Animation2D m_background;
			int m_particleNumber;

			double m_startEffect;
		public:
			using Enemy::Enemy;
			Enemy_14_to_18();
			~Enemy_14_to_18() override;
			void Create(const StageData::EnemyData&, bool createdByScript = false, Enemy* hpLink = nullptr) override;
			void Update() override;
			void Draw() const override;

			/*void* operator new(size_t);
			void operator delete(void* ptr);*/
		};
	}
}
