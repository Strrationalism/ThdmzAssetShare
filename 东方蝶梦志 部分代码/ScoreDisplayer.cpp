#include "Precompiled.h"
#include "ScoreDisplayer.h"
#include "Globals.h"
#include "Game.h"

UI::ScoreDisplayer::ScoreDisplayer() :m_score("GameUI/Number1")
{
	m_score.SetDisplayFormat(11);
	m_score.SetPos(-WIDTH / 2 + 16, -HEIGHT / 2 + 16);
	m_score.SetNumber(0);
	m_alpha = 1;
}

void UI::ScoreDisplayer::Reset()
{
}

void UI::ScoreDisplayer::Update()
{
	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	bool halfAlpha = y < -240;
	if (halfAlpha != m_halfAlpha) {
		m_halfAlpha = halfAlpha;

		if (m_halfAlpha) {
			Game::GetGame().GetDataActor().Add(m_alpha, 0.4, 20, 1);
			Game::GetGame().GetUI().GetNonFullHeart().HalfAlpha();
			Game::GetGame().GetUI().GetFullHearts().HalfAlpha();
		}
		else {
			Game::GetGame().GetDataActor().Add(m_alpha, 1, 20, 1);
			Game::GetGame().GetUI().GetNonFullHeart().FullAlpha();
			Game::GetGame().GetUI().GetFullHearts().FullAlpha();
		}
	}

	m_score.SetAlpha(m_alpha);
}

void UI::ScoreDisplayer::Draw() const
{
	m_score.Draw();
}