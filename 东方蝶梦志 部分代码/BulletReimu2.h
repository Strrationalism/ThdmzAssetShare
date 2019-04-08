#pragma once
/*
#include "BasicCharBullet.h"
#include "DataActor.h"
#include "Enemy.h"
#include "Sprite2D.h"

namespace Char {
	namespace Reimu {
		class BulletReimu2 final :public BasicCharBullet	//带有跟踪弹
		{
		private:
			Basic::Video::Renderer2D::Sprite2D m_sprite;
			double m_angle = 0;	//角度
			//double m_enemyAngle = 0;	//敌人所在角度
			int m_nearestEnemy;	//最近的敌人
			//Enemy::Enemy* m_targetEnemy = nullptr;

			double m_startX, m_startY;	//起始位置
			double m_berzierNow = 0;	//贝塞尔参数t

			Basic::DataActor m_dataActor;

			//int m_timer;

			void findNearestEnemy();	//寻找最近的敌人
		public:
			void Update() override;	//更新
			void Draw() const override;	//绘制

			void Kill() override;	//杀死之前
			void Create(double x, double y) override;	//创建时，传入创建坐标
			void OnColli(Enemy::Enemy &enemy) override;

			inline void SetAngle(double angle) {
				m_angle = angle;
			}
		};
	}
}*/