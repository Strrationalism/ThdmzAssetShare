#pragma once
#include "BaseAfterEffect.h"
#include "TaskList.h"
#include "DataActor.h"

namespace AfterEffect {
	class NegaCircleZoom :
		public AfterEffect::BaseAfterEffect
	{
	private:
		bool m_killed = false;
		bool m_useShader1 = true;

		double m_x, m_y, m_size = 0;

		Basic::DataActor m_dataActor;
		Basic::TaskList m_taskList;
	public:
		NegaCircleZoom(double x, double y);
		void Draw() const override;
		void Update() override;
		bool Killed() override;
	};
}