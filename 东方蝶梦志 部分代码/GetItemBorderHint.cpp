#include "Precompiled.h"
#include "GetItemBorderHint.h"
#include "Game.h"

Item::GetItemBorderHint::GetItemBorderHint()
{
	m_died = false;
	m_hint.UseImage("GameUI/GetItemBorderLine");
	const bool mirror = Game::GetGame().GetMirrorMode();
	const auto posX = WIDTH / 2 - 400;
	m_hint.SetPos(mirror ? - posX : posX, 0);
	m_hint.UpdateVertex();

	m_dataActor.Add(m_alpha, 1, 15, 5);
	m_taskList.AddTask([this]()
	{
		m_dataActor.Add(m_alpha, 0, 15, 5);
	}, 300);

	m_taskList.AddTask([this]()
	{
		Kill();
	}, 315);
}

void Item::GetItemBorderHint::Update()
{
	m_taskList.Update();
	m_dataActor.Update();

	m_hint.SetAlpha(m_alpha);
}

void Item::GetItemBorderHint::Draw() const
{
	m_hint.DrawToFastBuffer(Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR);
}

void Item::GetItemBorderHint::Kill()
{
	m_died = true;
}
