#include "Precompiled.h"
#include "BossSpellScorer.h"
#include "Game.h"
#include "SpellScoreResultDisplay.h"

constexpr double
ScoreBeginSpellCard = 5000000,
ScoreEndSpellCard = 1000000;

Boss::BossSpellScorer::BossSpellScorer():
	m_scoreDisplayer{"GameUI/Number2"}
{
	m_died = false;

	m_scoreDisplayer.SetDisplayFormat(7);
	m_scoreDisplayer.SetAlpha(0);
	m_scoreDisplayer.SetPos(530, -300);
}

void Boss::BossSpellScorer::SpellCardStart(int spellTime,bool isTimeSpell,int spellHash)
{
	m_inSpellCard = true;

	m_score = ScoreBeginSpellCard;

	m_tl.AddTask([this] {
		m_dataActor.Add(m_mainAlpha, 1, 20, 1);
	}, 140);

	if (!isTimeSpell)
	{
		m_tl.AddTask([this, spellTime] {
			m_dataActor.Add(m_score, ScoreEndSpellCard, spellTime - 1000, 0);
		}, 500);
	}

	m_spellHash = spellHash;
}

void Boss::BossSpellScorer::SpellCardEnd()
{
	if (!m_inSpellCard) return;
	m_inSpellCard = false;

	
	m_dataActor.Clear();
	const auto score = ((unsigned long long)m_score) / 10 * 10;

	Game::GetGame().GetGameActBridge()->FinishSpell(m_spellHash, score > 0);

	if (score > 0)
	{
		Game::GetGame().GetScoreManager().ScoreAdd(score);
		Game::GetGame().GetUILayerGameObjectManager().Insert(
			new SpellScoreResultDisplay(score)
		);
	}

	m_score = 0;
	m_tl.Clear();

	m_dataActor.Add(m_mainAlpha, 0, 20, 1);

	m_spellHash = 0;
}

void Boss::BossSpellScorer::ClearScore()
{
	if (!m_inSpellCard) return;

	m_dataActor.Clear();
	m_score = 0;
	m_tl.Clear();
}

void Boss::BossSpellScorer::Kill()
{
	m_died = true;
}

void Boss::BossSpellScorer::Update()
{
	m_tl.Update();
	m_dataActor.Update();

	m_scoreDisplayer.SetNumber((unsigned long long)m_score);

	m_scoreDisplayer.SetAlpha(m_mainAlpha * m_alpha);


	//°ëÍ¸Ã÷ÅÐ¶¨
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

void Boss::BossSpellScorer::Draw() const
{
	m_scoreDisplayer.Draw();
}
