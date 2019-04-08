#pragma once
#include "BasicCharBullet.h"
#include "Sprite2D.h"
#include "Game.h"

namespace Char
{
	namespace Marisa
	{
		class BulletMarisa2 final
			: public BasicCharBullet
		{
		private:
			Basic::Video::Renderer2D::Sprite2D m_sprite;
			double m_imageAlpha = 1.0;

			double m_maxSpeed = 0;
			double m_nowSpeed = 0;
			int m_timer = 0;

			bool m_powerX1dot3 = false;
		public:
			void Update() override;
			void Draw() const override;

			void Kill() override;
			void Create(double x, double y) override;

			void OnColli(Enemy::Enemy &enemy) override;
			auto& GetSprite() {
				return m_sprite;
			}

			static void SetPowerData(double);

			void* operator new(size_t);
			void operator delete(void* ptr);
		};
	}
}