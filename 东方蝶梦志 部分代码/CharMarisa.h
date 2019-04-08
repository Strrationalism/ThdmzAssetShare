#pragma once
#include "BasicChar.h"
namespace Char
{
	namespace Marisa
	{
		class CharMarisa final
			: public BasicChar
		{
		private:
			bool m_bombing = false;
			int m_shootTimer = 0;				// 射击的持续时间
		public:
			virtual ~CharMarisa();

			static CharMarisa& Get();

			void Reset() override;

			int GetSprite2DImageNum() override;

			void InitWithSnowRV(const Basic::SnowRVReader&);

			double GetPowerTop() override;
			void PowerUp(double now, double add) override;

			void ShootBegin(double x, double y, bool flip) override;
			void ShootUpdate(double power, double x, double y, bool flip) override;
			void ShootEnd(bool flip) override;

			void Update(double x, double y) override;

			std::unique_ptr<BasicOption> CreateOption() override;

			void Bomb(bool flip) override;

			bool Bombing() override
			{
				return m_bombing;
			};

			inline const char* GetName() override {
				return "Marisa";
			};
		};
	}
}
