#pragma once
#include "Sprite2D.h"
#include <list>
#include <vector>
#include <memory>
namespace UI {
	class FullHearts
	{
	private:
		Basic::Video::Renderer2D::Sprite2D m_label;
		std::vector<std::unique_ptr<Basic::Video::Renderer2D::Sprite2D>> m_hearts;
		double m_heartX, m_alpha;

		void incHeart();
		void decHeart();
	public:
		FullHearts();
		void Draw() const;
		void Update();

		void SetData(int lifeNum);

		void HalfAlpha();
		void FullAlpha();
	};
}
