#include "NuzPrecompiled.h"
#include "CtxFont.h"
#include "UnicodeSite.h"
#include "TitlePlayerDataSpellCardItem.h"
#include "PlayerProfile.h"
using namespace Basic::Video::Renderer2D;
extern Basic::Video::Renderer2D::CtxFont scFont;
Activities::Title::MenuStack::TitlePlayerDataSpellCardItem::TitlePlayerDataSpellCardItem(int title,int hash):
	m_spellcardName{
		{0,0,640,96},
		Sprite2DWStreamLayout::LayoutMode::LeftTop
	},
	m_display { hash }
{
	auto[ignore, gotTimes] = SaveSystem::PlayerProfile::Get().GetSpellGotNumber(hash);

	m_spellcardName.SetSpriteSizeOffset(15, 0);
	scFont.SetSprite2DWStreamToString(gotTimes > 0 ? Basic::UnicodeText(title) : L"£¿£¿£¿£¿£¿£¿", m_spellcardName);
	for (auto& p : m_spellcardName)
	{
		p->SetZoom(1);
	}

	m_spellcardName.UpdateStream();
}

void Activities::Title::MenuStack::TitlePlayerDataSpellCardItem::Draw() const
{
	if (m_visible)
	{
		m_spellcardName.Draw();
		m_display.OnDraw();
	}
}

void Activities::Title::MenuStack::TitlePlayerDataSpellCardItem::Update(float t)
{
	m_alpha.Update(t);
	for (auto& p : m_spellcardName)
	{
		p->SetAlpha(m_alpha);
	}
	m_display.SetAlpha(m_alpha);
}

void Activities::Title::MenuStack::TitlePlayerDataSpellCardItem::Visible(bool b)
{
	m_alpha.Tween(b ? 1 : 0, 0.25f, 1);
}

void Activities::Title::MenuStack::TitlePlayerDataSpellCardItem::SetPageLine(int y)
{
	m_spellcardName.SetRect({ 0, y * 45 - 200,640,96 });
	m_display.SetPos(500, y * 45 - 200);
	m_spellcardName.UpdateStream();
}
