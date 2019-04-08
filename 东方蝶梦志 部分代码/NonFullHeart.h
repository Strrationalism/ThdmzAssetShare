#pragma once
#include "Sprite2D.h"
namespace UI {
	class NonFullHeart
	{
	private:
		Basic::Video::Renderer2D::Sprite2D m_cup;
		Basic::Video::Renderer2D::Sprite2D m_water;

		double m_alpha, m_data, m_time;
		double m_timeHeartBeat;
	public:
		NonFullHeart();

		void Draw() const;
		void Update();

		void HalfAlpha();
		void FullAlpha();

		void SetData(double height);

		void SetDataUseDefaultHeight(int kagera);
	};
}
