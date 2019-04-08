#pragma once
#include "Sprite2D.h"
#include "NumberDisplay.h"

/* 注意：这里实际上显示了POWER和GRAZE两个控件。 */
namespace UI {
	class GrazeDisplayer
	{
	private:
		Basic::Video::Renderer2D::Sprite2D m_graze, m_power;
		Basic::Video::Renderer2D::NumberDisplay m_grazeDisplay, m_powerDisplay;

		double m_alpha = 1;
		bool m_halfAlpha = false;

		bool m_visib = false;
	public:
		GrazeDisplayer();
		void Reset();
		void Draw() const;
		void Update();
		inline void SetGraze(unsigned number) {
			m_grazeDisplay.SetNumber(number);
		}
		inline void SetPower(double power) {
			m_powerDisplay.SetNumber(uint64_t(power * 100));
		}
		void Hide();
		void Show();
	};
}
