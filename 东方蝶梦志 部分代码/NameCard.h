#pragma once
#include "GameObject.h"
#include "Sprite2D.h"
#include "DataActor.h"

namespace Conversation {
	class NameCard : public GameObject
	{
	private:
		Basic::Video::Renderer2D::Sprite2D m_nameCard;

		double m_alpha = 0;

		Basic::DataActor m_dataActor;
		Basic::TaskList m_taskList;
		
	public:
		NameCard(const std::string& charName);

		void Update() override;
		void Draw() const override;
		void Kill() override;
	};
}
