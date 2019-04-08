#pragma once

#include "Sprite2D.h"

class PlayerSheild final
{
private:
	double m_px, m_py;

	bool m_isLeft;
	int m_time;
	const int m_fullTime;

	bool m_giveLife = true;	//给一个残机碎片

	void halfKillBullet();	//小范围消弹
public:
	PlayerSheild(bool isLeft);
	bool OnUpdate(double x, double y);

	void OnDraw() const;

	inline bool IsLeft() const
	{
		return m_isLeft;
	}

	inline double GetPercent()
	{
		return m_time / double(m_fullTime);
	}

	void Bomb();
	void KillPlayer();

	inline bool Avliable() const
	{
		return m_time > 10;
	}
	bool IsItemPowerX2() const;		//是否P点获得亮乘以2
	bool IsShootPowerX1dot3() const;	//是否射击火力乘以1.3
};