#pragma once
#include "Sprite2D.h"
#include "BasicOption.h"
#include "SnowRVReader.h"
#include <string>
#include <memory>

namespace Char {
	class BasicChar	//基本游戏人物
	{
	private:
		double m_normalSpeed, m_slowSpeed;
	public:
		virtual ~BasicChar() {};

		virtual void Reset() {};

		virtual int GetSprite2DImageNum() = 0;	//玩家的图像编号

		inline void SetNormalSpeed(double s) {
			m_normalSpeed = s;
		}
		inline void SetSlowSpeed(double s) {
			m_slowSpeed = s;
		}
		inline double GetNormalSpeed() const {	//获取普通速度
			return m_normalSpeed;
		}
		inline double GetSlowSpeed() const {	//获取慢速速度
			return m_slowSpeed;
		}

		virtual double GetPowerTop() = 0;	//获取火力上限
		virtual void PowerUp(double now, double add) = 0;	//火力上升时，当前上升后的火力以及火力增量

		virtual void ShootBegin(double x, double y, bool flip) = 0;	//开始发射子弹时
		virtual void ShootUpdate(double power, double x, double y, bool flip) = 0;	//发射子弹时
		virtual void ShootEnd(bool flip) = 0;	//停止发射子弹时

		virtual void Update(double x, double y) = 0;	//对人物的更新

		virtual std::unique_ptr<BasicOption> CreateOption() = 0;	//返回一个Option指针，交给Player管理。

		virtual void Bomb(bool flip) = 0;	//扔雷时

		virtual bool Bombing() = 0;

		virtual const char* GetName() = 0;
	};

	void LoadAndSetConfig();
}