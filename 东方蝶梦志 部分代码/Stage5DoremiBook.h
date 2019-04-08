#pragma once
#include "Animation2D.h"

namespace Boss
{
	class Stage5DoremiBook
	{
	private:
		Basic::Video::Renderer2D::Animation2D m_img;
		bool m_bossFront = true;
		
		struct Tail
		{
			int frame;
			int startTime;
			double x, y;
			bool bossFront;
		};

		std::deque<Tail> m_tails;

	public:
		Stage5DoremiBook();

		void Draw() const;

		void Update(double x,double y);

		inline bool BossFront() const
		{
			return m_bossFront;
		}
	};
}
