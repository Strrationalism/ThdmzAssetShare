#pragma once
#include "GameObject.h"
#include "Sprite2D.h"
#include "DataActor.h"
#include "TaskList.h"
namespace Enemy
{
	class EnemyWarning : public GameObject
	{
	private:
		Basic::DataActor m_dataActor;
		Basic::TaskList m_taskList;

		Basic::Video::Renderer2D::Sprite2D m_sprite;

		double m_alpha = 0;
	public:
		EnemyWarning(double y);
		void Update() override;
		void Draw() const override;
		void Kill() override;
	};
}