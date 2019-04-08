#pragma once
#include "BasicCharBullet.h"
#include <vector>

class PlayerBulletManager final
{
private:
	std::vector<std::unique_ptr<Char::BasicCharBullet>> m_bullets;

	// 检查子弹与敌人的碰撞
	void CheckCollision();
public:
	void Draw() const;
	void Update();

	void KillAll();	//杀死已存在的子弹
	void ForceKillAll();	//强制杀死所有已存在的子弹
	void Shoot(Char::BasicCharBullet*, double x, double y);	//发射子弹，将子弹委托于此后无需手动释放。
};
