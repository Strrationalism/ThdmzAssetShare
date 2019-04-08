#pragma once
#include "GameObject.h"
#include "Sprite2D.h"
#include "TaskList.h"
#include "DataActor.h"
namespace Boss {
	class SpellCardDisplay :
		public GameObject
	{
	private:
		std::shared_ptr < Basic::Video::Renderer2D::Sprite2DWStreamLayout > m_sp;
		double m_alpha;
		Basic::TaskList m_taskList;
		Basic::DataActor m_dataActor;

		bool m_halfAlpha = false;

		double m_y;
	public:
		SpellCardDisplay();
		void Reset(const std::shared_ptr < Basic::Video::Renderer2D::Sprite2DWStreamLayout >&);
		void Hide();
		void Kill() override;
		void Update() override;
		void Draw() const override;
	};
}