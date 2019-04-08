#include "Precompiled.h"
#include "SpellCardDisplay.h"
#include "Game.h"

using namespace Basic::Video::Renderer2D;

Boss::SpellCardDisplay::SpellCardDisplay()
{
	m_died = false;
	m_drawOnTop = true;
}

void Boss::SpellCardDisplay::Reset(const std::shared_ptr < Basic::Video::Renderer2D::Sprite2DWStreamLayout >& s)
{

	m_visible = true;
	m_alpha = 0;
	if (m_halfAlpha)
		m_dataActor.Add(m_alpha, 0.35, 20, 1);
	else
		m_dataActor.Add(m_alpha, 1, 20, 1);

	m_sp = s;
	m_sp->SetMode(Basic::Video::Renderer2D::Sprite2DWStreamLayout::LayoutMode::CenterTop);

	m_y = -250;

	m_taskList.AddTask([this]() {
		/*GameObjectWithDmslControl::SetTask({
			GameObjectWithDmslControl::Task::MOVE,
			0,-200,
			x,-320,
			20,2
		});*/

		m_dataActor.Add(m_y, -320, 40, 2);
	}, 100);
}

void Boss::SpellCardDisplay::Hide()
{
	m_alpha = 0;
	m_visible = false;
}

void Boss::SpellCardDisplay::Kill()
{
	m_died = true;
}

void Boss::SpellCardDisplay::Update()
{
	m_dataActor.Update();
	m_taskList.Update();

	if (m_sp) {
		m_sp->SetRect(SDL_Rect{ -640,(int)m_y,1280,96 });
		for (auto& p : *m_sp)
		{
			p->SetAlpha(m_alpha);
		}
		m_sp->UpdateStream();
	}

	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	bool halfAlpha = y < -290;
	if (halfAlpha != m_halfAlpha) {
		m_halfAlpha = halfAlpha;
		if (m_halfAlpha)
			m_dataActor.Add(m_alpha, 0.25, 20, 1);
		else
			m_dataActor.Add(m_alpha, 1, 20, 1);
	}
}

void Boss::SpellCardDisplay::Draw() const
{
	if (m_sp) m_sp->Draw();
}