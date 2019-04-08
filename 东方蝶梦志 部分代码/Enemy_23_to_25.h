#pragma once
#include "Enemy.h"
#include "Sprite2D.h"
namespace Enemy {
	namespace SpecialEnemy {
		class Enemy_23_to_25 :
			public Enemy
		{
		private:
			Basic::Video::Renderer2D::Sprite2D m_backLight;
			double m_beginEffect = 0;
		public:
			using Enemy::Enemy;
			void Update() override;
			void Draw() const override;
			void Create(const StageData::EnemyData&, bool createdByScript = false, Enemy* hpLink = nullptr) override;

			/*void* operator new(size_t);
			void operator delete(void* ptr);*/
		};
	}
}