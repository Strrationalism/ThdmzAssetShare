#pragma once
#include "GameObject.h"
#include "NumberDisplay.h"
#include "DataActor.h"
#include "TaskList.h"

namespace Boss
{
	class SpellScoreResultDisplay : public GameObject
	{
	private:
		Basic::Video::Renderer2D::NumberDisplay m_num;

		Basic::TaskList m_tl;
		Basic::DataActor m_dataActor;

		double m_alpha = 0;
	public:
		SpellScoreResultDisplay(unsigned score);
		void Kill() override;
		void Update() override;
		void Draw() const override;
	};
}