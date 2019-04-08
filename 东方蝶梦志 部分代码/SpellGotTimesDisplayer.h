#pragma once
#include "SecTweenData.h"
#include "NumberDisplay.h"

namespace Activities::Title::MenuStack
{
	class SpellGotTimesDisplayer
	{
	private:
		const int m_spellHash;
		Basic::Video::Renderer2D::NumberDisplay m_num1, m_num2;
	public:
		SpellGotTimesDisplayer(int spellHash);

		void OnDraw() const;
		void SetPos(float x, float y);

		void SetAlpha(float alpha);
	};
}