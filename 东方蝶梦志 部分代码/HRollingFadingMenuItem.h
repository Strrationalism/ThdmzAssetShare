#pragma once
#include "Sprite2D.h"
#include <string>
#include <functional>
#include "SecTweenData.h"
#include "MenuItem.h"

namespace Menus
{
	class HRollingFadingMenuItem : public Basic::Menu::MenuItem
	{
	private:
		Basic::Video::Renderer2D::Sprite2D m_sp;
		Basic::SecTweenData<float> m_x;
		const float m_y;
		const float m_yOffset;
		Basic::SecTweenData<float> m_ytween, m_systemAlpha,m_alpha = 1, m_zoom;
		const std::function<void()> m_onSelected;

		void updateSelectAnimation(int offset);

		
	public:
		HRollingFadingMenuItem(
			const std::string& imageName,
			int imageNum,
			float x, 
			float y,
			float yOffset,
			const std::function<void()>& onSelected);

		virtual void OnInitSelect(int offset);
		virtual void OnSelectChanged(int offset);
		virtual void OnEntry(int offset);
		virtual void Update(float deltaTime);
		virtual void Draw() const;

		inline auto& GetAlpha()
		{
			return m_alpha;
		}

		inline auto& GetX()
		{
			return m_x;
		}
	};
}