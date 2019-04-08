#pragma once
#include "Sprite2D.h"
#include "DataActor.h"

namespace UI {
	class DreamSystem
	{
	private:
		Basic::Video::Renderer2D::Sprite2D m_main, m_line, m_pointer;
		Basic::DataActor m_dataActor;
		double m_data;
		double m_alpha;
		bool m_enabled = true;
	public:
		DreamSystem();
		void Reset();
		void Draw() const;
		void Update();

		void SetData(double);	//左为负数，右为正数

		void ZeroAlpha();
		void HalfAlpha();	//玩家靠近时隐藏
		void FullAlpha();	//玩家远离时显示

		//void Enable(bool);
	};
}