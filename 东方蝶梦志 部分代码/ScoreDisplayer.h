#pragma once
#include "NumberDisplay.h"

namespace UI {
	class ScoreDisplayer
	{
	private:
		Basic::Video::Renderer2D::NumberDisplay m_score;
		double m_alpha;
		bool m_halfAlpha = false;
	public:
		ScoreDisplayer();
		void Reset();
		void Update();
		void Draw() const;

		inline void SetScore(unsigned long long n) {
			m_score.SetNumber(n);
		}
	};
}