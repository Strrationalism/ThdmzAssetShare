#pragma once
#include "BasicOption.h"
#include "Sprite2D.h"
#include "DataActor.h"
#include <array>
namespace Char {
	namespace Reimu {
		class ReimuOption :
			public Char::BasicOption
		{
		private:
			std::array<Basic::Video::Renderer2D::Sprite2D,4> m_sp;
			double m_pos[4][2];

			double m_angle = 0;
			double m_angleSpeed = 1.5;

			int m_nowPower = 1;

			Basic::DataActor m_dataActor;

			bool m_slowMode = false;

			double m_px, m_py;	//玩家位置
			uint32_t m_timer = 0;	//在0~999内循环

		public:
			ReimuOption();

			void OnUpdate(double x, double y) override;	//更新
			void OnDraw() const override;	//要求绘制时

			void OnSlowStart() override;	//开始Slow模式时
			void OnSlowStopped() override;	//Slow模式停止时

			void ShootBegin(bool flip) override;	//开始发射子弹时
			void ShootUpdate(bool flip) override;	//发射子弹时
			void ShootEnd(bool flip) override;	//停止发射子弹时

			void SetPower(int) override;	//设置Power

			static void SetPowerData(double fast, double slow);
			static void SetNonSlowPowerAngle(double angle);
		};
	}
}