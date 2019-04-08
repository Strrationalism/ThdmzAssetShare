#include "NuzPrecompiled.h"
#include "FadingMenuItem.h"

Menus::FadingMenuItem::FadingMenuItem(const std::string & tex, int texImage,float x,float y, Callback&& c):
	m_cb{ std::move(c) }
{
	m_sp.UseImage(tex, texImage);
	m_sp.SetPos(x, y);
	m_sp.SetAlpha(0);
	m_sp.UpdateVertex();
}

void Menus::FadingMenuItem::OnInitSelect(int offset)
{
	OnSelectChanged(offset);
}

void Menus::FadingMenuItem::OnSelectChanged(int offset)
{
	m_alpha.Tween(offset == 0 ? 1 : 0, 0.25f, 0);
}

void Menus::FadingMenuItem::OnEntry(int offset)
{
}

void Menus::FadingMenuItem::Update(float t)
{
	m_alpha.Update(t);
	m_sp.SetAlpha(m_alpha);
}

void Menus::FadingMenuItem::Draw() const
{
	m_sp.Draw();
}

void Menus::FadingMenuItem::FadeOut()
{
	m_alpha.Tween(0, 0.25f, 0);
}
