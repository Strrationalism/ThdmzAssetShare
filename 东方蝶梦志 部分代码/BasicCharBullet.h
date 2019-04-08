#pragma once
#include <memory>
#include "GLTexture.h"
#include "GameObjectWithDmslControl.h"
#include "Enemy.h"

namespace Char {
	/* 基本的我方子弹 */
	class BasicCharBullet :public GameObjectWithDmslControl {
	protected:
		double m_colliRadius = 0;	// 碰撞半径
		bool m_checkColli = true;	// 是否检测碰撞
	public:

		virtual ~BasicCharBullet() {}

		void Update() override = 0;	//更新
		void Draw() const override = 0;	//绘制

		void Kill() override = 0;	//杀死之前，在此后可以有一段存活时间。
		virtual void Create(double x, double y) = 0;	//创建时，传入创建坐标和火力

		// 发生碰撞时调用的函数
		virtual void OnColli(Enemy::Enemy &enemy) = 0;

		void SetRadius(double radius)
		{
			m_colliRadius = radius;
		}

		void SetCheckCollision(bool check)
		{
			m_checkColli = check;
		}

		// 获取碰撞半径
		double GetRadius()
		{
			return m_colliRadius;
		}

		// 是否检测碰撞
		bool GetCheckCollision()
		{
			return m_checkColli;
		}
		//virtual Collision& GetCollision() = 0;	//获取碰撞体
	};
}