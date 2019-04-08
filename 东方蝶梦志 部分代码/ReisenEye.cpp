#include "Precompiled.h"
#include "ReisenEye.h"
#include "Boss.h"


ReisenEye::ReisenEye()
{
	m_died = false;

	m_circle.UseImage("ReisenEye", 1);
	double x, y;
	Boss::Boss::GetBoss()->GetPos(x, y);
	m_circle.SetPos(x, y);
	m_circleSize = 0;
	m_circleAlpha = 1;
	m_datas.Add(m_circleSize, 6, 75, 1);

	m_sp.UseImage("ReisenEye",0);
	m_sp.SetPos(0, 0);

	m_alpha = 0;
	m_size = 1.5;
	m_datas.Add(m_size, 1, 25,1);
	m_datas.Add(m_alpha, 1, 25,1);

	m_tasks.AddTask([this]() {
		m_datas.Add(m_size, 1.5, 25, 1);
		m_datas.Add(m_alpha, 0, 25, 1);
		m_datas.Add(m_circleAlpha, 0, 25, 1);
	}, 50);

	m_tasks.AddTask([this]() {
		m_died = true;
	}, 75);
}

void ReisenEye::Update()
{
	m_tasks.Update();
	m_datas.Update();

	m_sp.SetAlpha(m_alpha);
	m_sp.SetZoom(m_size);
	m_sp.UpdateVertex();

	m_circle.SetAlpha(m_circleAlpha);
	m_circle.SetZoom(m_circleSize);
	m_circle.UpdateVertex();
}

void ReisenEye::Draw() const
{
	m_circle.DrawToFastBuffer(Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR);
	m_sp.DrawToFastBuffer(Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR);
}

void ReisenEye::Kill()
{
	m_died = true;
}


