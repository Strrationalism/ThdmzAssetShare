#pragma once
#include "GameObject.h"
#include "DataActor.h"
#include "TaskList.h"
#include "NumberDisplay.h"

namespace Boss
{
	class BossSpellScorer : public GameObject
	{
	private:
		bool m_inSpellCard = false;

		Basic::TaskList m_tl;
		Basic::DataActor m_dataActor;
		double m_score = 0;
		double m_mainAlpha = 0;
		double m_alpha = 1;
		bool m_halfAlpha = false;

		int m_spellHash = 0;

		Basic::Video::Renderer2D::NumberDisplay m_scoreDisplayer;
	public:
		BossSpellScorer();
		void SpellCardStart(int spellTime,bool isTimeSpell,int spellHash);
		void SpellCardEnd();
		void ClearScore();

		void Kill() override;
		void Update() override;
		void Draw() const override;
	};
}