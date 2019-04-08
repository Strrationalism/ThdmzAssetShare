#include "NuzPrecompiled.h"
#include "TextMenuItem.h"
#include "Globals.h"

Menus::TextMenuItem::TextMenuItem(const std::wstring & ws, int y, const std::function<void()>& f) :
	m_sps{ SDL_Rect{ -250,y,500,300 } },
	m_func{ f }
{
	m_sps.SetSpriteSizeOffset(8, 10);
	GetCtxFont1().SetSprite2DWStreamToString(ws, m_sps);
	m_sps.UpdateStream();
}

void Menus::TextMenuItem::OnInitSelect(int offset)
{
	for (auto& sp : m_sps)
		sp->SetAlpha(offset == 0 ? 1 : 0.5);
}

void Menus::TextMenuItem::OnSelectChanged(int offset)
{
	for (auto& sp : m_sps)
		sp->SetAlpha(offset == 0 ? 1 : 0.5);
}

void Menus::TextMenuItem::OnEntry(int offset)
{
	if (offset == 0)
		m_func();
}

void Menus::TextMenuItem::Update(float deltaTime)
{
}

void Menus::TextMenuItem::Draw() const
{
	m_sps.Draw();
}