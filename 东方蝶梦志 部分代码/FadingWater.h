#pragma once
#include "BaseAfterEffect.h"
#include "DataActor.h"

namespace AfterEffect {
	class FadingWater :
		public BaseAfterEffect
	{
	private:
		double m_cnt = -1.25, m_texFactor = 0, m_ampFactor = 0;
		Basic::DataActor m_dataActor;
		bool m_killed = false;
	public:
		FadingWater(int time);
		void Draw() const override;
		void Update() override;
		bool Killed() override;
	};
}