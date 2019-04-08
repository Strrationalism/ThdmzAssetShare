#pragma once
#include "Sprite2D.h"
#include "Sprite2DWStreamLayout.h"
#include "SpellCardDatabase.h"
#include "SecTweenData.h"
#include "SpellGotTimesDisplayer.h"

namespace Activities::Title::MenuStack
{
	class TitlePlayerDataSpellCardItem
	{
	private:
		Basic::Video::Renderer2D::Sprite2DWStreamLayout m_spellcardName;
		SpellGotTimesDisplayer m_display;

		bool m_visible = true;

		Basic::SecTweenData<float> m_alpha = 0;
	public:
		TitlePlayerDataSpellCardItem(int title,int hash);
		void Draw() const;

		void Update(float);
		void Visible(bool);
		void SetPageLine(int);
	};
}