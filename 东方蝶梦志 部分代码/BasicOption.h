#pragma once

namespace Char {
	class BasicOption {
	protected:
		bool m_died;
	public:
		virtual ~BasicOption() {};

		inline bool Died() {
			return m_died;
		}

		virtual void OnUpdate(double x, double y) = 0;	//更新
		virtual void OnDraw() const = 0;	//要求绘制时

		virtual void OnSlowStart() = 0;	//开始Slow模式时
		virtual void OnSlowStopped() = 0;	//Slow模式停止时

		virtual void ShootBegin(bool flip) = 0;	//开始发射子弹时
		virtual void ShootUpdate(bool flip) = 0;	//发射子弹时
		virtual void ShootEnd(bool flip) = 0;	//停止发射子弹时

		virtual void SetPower(int) = 0;	//设置火力
	};
}