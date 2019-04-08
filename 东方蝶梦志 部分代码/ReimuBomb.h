#pragma once
#include "GameObjectWithDmslControl.h"
#include "Particle3D.h"
#include "Collision.h"

namespace Char {
	namespace Reimu {
		class ReimuBomb :
			public GameObjectWithDmslControl
		{
		private:
			Basic::Video::Renderer3D::Particle3D m_particle3D;

			Collision::Circle m_coll;

			int m_forceKill = 400;
			int m_timeOffset;

			int m_nearestEnemy = -1;

			static int m_searchNewEnemy;
			int m_nowSearch;

			enum {
				STATE0,	//模式0，刚刚出现时旋转，并跟着自机旋转
				STATE1	//模式1，只是跟着自机旋转
			}m_state;

			static int m_num;

			double m_angleFromPlayer, m_radFromPlayer;	//state0时公转角度，半径

			static double m_angleChanging;

			void updateState0();	//刚刚出现时,转圈时
			void updateState1();	//跟着自机旋转

		public:
			ReimuBomb(double startAngle);
			~ReimuBomb();
			static inline bool Bombing() {
				return m_num != 0;
			}
			void Update() override;
			void Draw() const override;
			void Kill() override;

			static void SetPowerData(double single);
		};
	}
}
