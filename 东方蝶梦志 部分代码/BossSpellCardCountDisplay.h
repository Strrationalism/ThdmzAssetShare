#pragma once
#include "GameObject.h"
#include "Sprite2D.h"
#include "DataActor.h"
#include <memory>
#include <stack>

namespace Boss {
	class BossSpellCardCountDisplay :
		public GameObject
	{
	private:
		class ASpell :public GameObject {
		private:
			Basic::Video::Renderer2D::Sprite2D m_sp;
			Basic::DataActor m_dataActor;
			double m_alpha = 0, m_y, m_x;
		public:
			void Update() override;
			void Draw() const override;
			void Kill() override;
			void Init(double x, int i);
		};
		std::stack<ASpell*> m_spells;
		Basic::Video::Renderer2D::Sprite2D m_sp;
		double m_alpha = 0;
		Basic::DataActor m_dataActor;
	public:
		BossSpellCardCountDisplay();
		void InitStart(int spellNum);
		void KillASpell();
		void Draw() const override;
		void Update() override;
		void Kill() override;
	};
}