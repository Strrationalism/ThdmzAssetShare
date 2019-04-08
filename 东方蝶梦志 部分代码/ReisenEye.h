#pragma once
#include "GameObject.h"
#include "Sprite2D.h"
#include "TaskList.h"
#include "DataActor.h"

class ReisenEye :
	public GameObject
{
private:
	Basic::Video::Renderer2D::Sprite2D m_sp,m_circle;

	double m_alpha, m_size, m_circleSize, m_circleAlpha;

	Basic::TaskList m_tasks;
	Basic::DataActor m_datas;
public:
	ReisenEye();
	void Update() override;
	void Draw() const override;
	void Kill() override;
};
