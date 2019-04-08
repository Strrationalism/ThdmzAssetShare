#pragma once
#include "BasicChar.h"
#include "GLTexture.h"
#include <memory>
namespace Char {
	namespace Reimu {
		class CharReimu final :public BasicChar
		{
		private:
			int m_timer = 0;

			bool m_bombing = false;
			//double m_nearestEnemyAngle = 0;

			int m_nearestEnemy = -1;
		public:
			~CharReimu() override;

			static CharReimu& Get();

			void Reset() override;

			int GetSprite2DImageNum() override;

			void InitWithSnowRV(const Basic::SnowRVReader&);

			double GetPowerTop() override;
			void PowerUp(double now, double add) override;

			void ShootBegin(double x, double y, bool flip) override;
			void ShootUpdate(double power, double x, double y, bool flip) override;	//发射子弹时
			void ShootEnd(bool flip) override;	//停止发射子弹时

			void Update(double x, double y) override;

			std::unique_ptr<BasicOption> CreateOption() override;

			inline int GetNearestEnemy() {
				return m_nearestEnemy;
			}

			/*inline double GetNearestEnemyAngle() {
				return m_nearestEnemyAngle;
			}*/

			void Bomb(bool flip) override;

			bool Bombing() override;

			const char* GetName() override;
		};
	}
}
