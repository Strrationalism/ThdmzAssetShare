#include "Sprite2DWStreamLayout.h"
#include "NuzPrecompiled.h"
#include "Sprite2DWStreamLayout.h"

Basic::Video::Renderer2D::Sprite2DWStreamLayout::Sprite2DWStreamLayout(SDL_Rect rect, LayoutMode mode)
{
	SetRect(rect);
	m_mode = mode;
}

void Basic::Video::Renderer2D::Sprite2DWStreamLayout::SetMode(LayoutMode mode)
{
	m_mode = mode;
}

std::pair<int, int> Basic::Video::Renderer2D::Sprite2DWStreamLayout::GetContentSize()
{
	int allHeight = 0;
	int allWidth = 0;

	int thisLineHeight = 0;
	int thisLineWidth = 0;
	for (const auto& sp : m_sprites)
	{
		auto[spW, spH] = sp->GetSpriteSize();
		spW += m_spriteSizeOffsetW;
		spH += m_spriteSizeOffsetH;

		if (spH > thisLineHeight) thisLineHeight = spH;

		if (thisLineWidth + spW >= m_rect.w)
		{
			//»»ÐÐ
			if (thisLineWidth + spW > allWidth) allWidth = thisLineWidth + spW;
			allHeight += thisLineHeight;
			thisLineHeight = 0;
			thisLineWidth = 0;
		}
		else {
			thisLineWidth += spW;
		}
	}

	if (thisLineWidth > allWidth) allWidth = thisLineWidth;
	return std::pair<int, int>(allWidth,allHeight);
}

void Basic::Video::Renderer2D::Sprite2DWStreamLayout::SetSpriteSizeOffset(int w, int h)
{
	m_spriteSizeOffsetW = w;
	m_spriteSizeOffsetH = h;
}

void Basic::Video::Renderer2D::Sprite2DWStreamLayout::SetRect(SDL_Rect rect)
{
	m_rect = rect;
}

void Basic::Video::Renderer2D::Sprite2DWStreamLayout::UpdateStream()
{
	int x = 0, y = 0;

	

	for (auto& p : m_sprites)
	{
		auto[spW,spH] = p->GetSpriteSize();
		spW += m_spriteSizeOffsetW;
		spH += m_spriteSizeOffsetH;
		if (x + spW >= m_rect.w) {
			x = 0;
			y += spH;
		}
		switch (m_mode)
		{
		case Basic::Video::Renderer2D::Sprite2DWStreamLayout::LayoutMode::LeftTop:
			p->SetPos(x + m_rect.x, y + m_rect.y);
			break;
		case Basic::Video::Renderer2D::Sprite2DWStreamLayout::LayoutMode::CenterTop: {
			auto cW = GetContentSize().first;
			p->SetPos(x + m_rect.x + (m_rect.w/2 - cW/2), y + m_rect.y);
			break;
		}
		case Basic::Video::Renderer2D::Sprite2DWStreamLayout::LayoutMode::RightTop: {
			auto cW = GetContentSize().first;
			p->SetPos(m_rect.x + m_rect.w - cW + x, y + m_rect.y);
			break;
		}
		default:
			break;
		}
		
		x += spW;

		p->UpdateVertex();
	}
}

void Basic::Video::Renderer2D::Sprite2DWStreamLayout::Draw() const
{
	for (const auto& p : m_sprites)
		p->Draw();
}

void Basic::Video::Renderer2D::Sprite2DWStreamLayout::DrawToFastBuffer(int fastBufferIndex) const
{
	for (const auto& p : m_sprites)
		p->DrawToFastBuffer(fastBufferIndex);
}

void Basic::Video::Renderer2D::Sprite2DWStreamLayout::AddSprite(std::shared_ptr<Sprite2D>& sprite)
{
	m_sprites.push_back(sprite);
}
