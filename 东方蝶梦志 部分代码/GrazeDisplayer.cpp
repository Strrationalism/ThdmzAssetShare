#include "Precompiled.h"
#include "GrazeDisplayer.h"
#include "Globals.h"
#include "Game.h"

#define YOFFSET 40

#define DISPLAY_LOFFSET 64

UI::GrazeDisplayer::GrazeDisplayer() :m_grazeDisplay("GameUI/Number2"), m_powerDisplay("GameUI/Number2")
{
	m_graze.UseImage("GameUI/Graze", 0);
	m_graze.SetPos(-WIDTH / 2 + 14 + 76 / 2 + 2, 292 + YOFFSET);

	m_power.UseImage("GameUI/Graze", 1);
	m_power.SetPos(-WIDTH / 2 + 14 + 76 / 2 + 2, 292 + YOFFSET - 23 - 2);
	m_power.SetSize(70, 23);

	m_grazeDisplay.SetPos(-WIDTH / 2 + 290 - DISPLAY_LOFFSET, HEIGHT / 2 - 16 - 41 - 23 + YOFFSET);
	m_grazeDisplay.LeftTopPos(true);
	m_grazeDisplay.SetNumber(0);

	m_powerDisplay.SetPos(-WIDTH / 2 + 290 - DISPLAY_LOFFSET, HEIGHT / 2 - 16 - 41 - 23 + YOFFSET - 23 - 2);
	m_powerDisplay.LeftTopPos(true);
	m_powerDisplay.SetNumber(0);

	m_graze.UpdateVertex();
	m_power.UpdateVertex();
}

void UI::GrazeDisplayer::Reset()
{
	Show();
}

void UI::GrazeDisplayer::Draw() const
{
	m_graze.Draw();
	m_grazeDisplay.Draw();

	m_power.Draw();
	m_powerDisplay.Draw();
}

void UI::GrazeDisplayer::Update()
{
	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	bool halfAlpha = y > 270;
	if (halfAlpha != m_halfAlpha) {
		m_halfAlpha = halfAlpha;
		if (m_visib) {
			if (m_halfAlpha) {
				Game::GetGame().GetDataActor().Add(m_alpha, 0.4, 20, 1);
				Game::GetGame().GetUI().GetDreamSystem().HalfAlpha();
			}
			else {
				Game::GetGame().GetDataActor().Add(m_alpha, 1, 20, 1);
				Game::GetGame().GetUI().GetDreamSystem().FullAlpha();
			}
		}
	}

	m_grazeDisplay.SetAlpha(m_alpha);
	m_graze.SetAlpha(m_alpha);

	m_powerDisplay.SetAlpha(m_alpha);
	m_power.SetAlpha(m_alpha);
}
void UI::GrazeDisplayer::Hide()
{
	m_visib = false;
	Game::GetGame().GetDataActor().Add(m_alpha, 0, 20, 1);
	Game::GetGame().GetUI().GetDreamSystem().ZeroAlpha();
}

void UI::GrazeDisplayer::Show()
{
	m_visib = true;
	if (m_halfAlpha) {
		Game::GetGame().GetDataActor().Add(m_alpha, 0.4, 20, 1);
		Game::GetGame().GetUI().GetDreamSystem().HalfAlpha();
	}
	else {
		Game::GetGame().GetDataActor().Add(m_alpha, 1, 20, 1);
		Game::GetGame().GetUI().GetDreamSystem().FullAlpha();
	}
}