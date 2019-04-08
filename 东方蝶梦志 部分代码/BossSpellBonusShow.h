#pragma once
#include "GameObject.h"
#include "Sprite2D.h"
#include "TaskList.h"
#include "DataActor.h"

namespace Boss {
	class BossSpellBonusShow :
		public GameObject
	{
	private:
		Basic::Video::Renderer2D::Sprite2D m_sp;
		double m_y, m_alpha = 0;
		Basic::TaskList m_taskList;
		Basic::DataActor m_dataActor;
	public:
		BossSpellBonusShow(bool getBonus);

		void Draw() const override;
		void Update() override;
		void Kill() override;
	};
}