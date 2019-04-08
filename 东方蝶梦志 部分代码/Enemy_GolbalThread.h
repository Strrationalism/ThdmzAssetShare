#pragma once
#include "Enemy.h"
#include "StageTask.h"
namespace Enemy {
	namespace SpecialEnemy {
		class Enemy_GolbalThread :public Enemy
		{
		public:
			using Enemy::Enemy;
			void Create(int dmsl, double x, double y);
			void Kill() override;
			void Update() override;
			void Draw() const override;

			/*void* operator new(size_t);
			void operator delete(void* ptr);*/
		};
	}
}
namespace StageTask {
	class CreateStageDmsl :public BaseTask {
	private:
		bool m_ok = false;
		double m_x, m_y;
		int m_dmsl;
	public:
		virtual void Create(Basic::CSVReader&, const StageData&) override;
		virtual void Proc() override;
		virtual bool Finished() override;
	};
}
