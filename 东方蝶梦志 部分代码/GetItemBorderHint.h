#pragma once
#include "GameObject.h"
#include "Sprite2D.h"

namespace Item {
	class GetItemBorderHint:
		public GameObject
	{
	private:
		Basic::Video::Renderer2D::Sprite2D m_hint;

		double m_alpha = 0;
		Basic::DataActor m_dataActor;
		Basic::TaskList m_taskList;
	public:
		GetItemBorderHint();

		void Update() override;
		void Draw() const override;
		void Kill() override;
	};
}
