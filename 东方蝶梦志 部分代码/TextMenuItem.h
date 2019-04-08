#pragma once
#include "MenuItem.h"
#include "Sprite2D.h"
#include "Sprite2DWStreamLayout.h"
#include <string>
#include <functional>
namespace Menus
{
	class TextMenuItem : public Basic::Menu::MenuItem
	{
	private:
		Basic::Video::Renderer2D::Sprite2DWStreamLayout m_sps;
		const std::function<void()> m_func;
	public:
		TextMenuItem(const std::wstring& ws,int y, const std::function<void()>&);
		void OnInitSelect(int offset) override;
		void OnSelectChanged(int offset) override;
		void OnEntry(int offset) override;
		void Update(float deltaTime) override;
		void Draw() const override;
	};
}
