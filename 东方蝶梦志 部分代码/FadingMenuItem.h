#pragma once
#include "MenuItem.h"
#include "SecTweenData.h"
#include "Sprite2D.h"
namespace Menus
{
	class FadingMenuItem : public Basic::Menu::MenuItem
	{
	public:
		using Callback = std::function<void()>;
	private:
		Basic::SecTweenData<float> m_alpha;
		Basic::Video::Renderer2D::Sprite2D m_sp;
		const Callback m_cb;
	public:
		FadingMenuItem(const std::string& tex, int texImage, float x, float y, Callback&&);
		void OnInitSelect(int offset) override;
		void OnSelectChanged(int offset) override;
		void OnEntry(int offset) override;
		void Update(float) override;
		void Draw() const override;

		void FadeOut();
	};
}
