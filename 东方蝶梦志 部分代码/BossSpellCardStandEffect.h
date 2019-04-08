#pragma once
#include "GameObject.h"
#include "Sprite2D.h"
#include "TaskList.h"
#include <memory>
#include "DataActor.h"

namespace Boss {
	class BossSpellCardStandEffect :
		public GameObject
	{
	private:
		std::shared_ptr<Basic::Video::Renderer2D::Sprite2D> m_lh;

		double m_alpha = 0, m_x = 400, m_y = -100;

		Basic::TaskList m_tasks;
		Basic::DataActor m_dataActor;
	public:
		BossSpellCardStandEffect(std::shared_ptr<Basic::Video::Renderer2D::Sprite2D>& s);

		void Draw() const override;
		void Update() override;
		void Kill() override;
	};
}
