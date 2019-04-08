#include "Precompiled.h"
#include "Stage5DoremiBook.h"
#include "Game.h"

Boss::Stage5DoremiBook::Stage5DoremiBook()
{
	m_img.UseImage("Charater/DoremiBookPx", 12, 0);
}

void Boss::Stage5DoremiBook::Draw() const
{
	for (const auto& p : m_tails)
	{
		Basic::Video::Renderer2D::Sprite2D sp;
		sp.UseImage("Charater/DoremiBookPx", p.frame);
		const int life = Game::GetGame().GetGameTime() - p.startTime;
		const float lerp = (30.0 - life) / 30.0;
		
		sp.SetPos(p.x, p.y);
		sp.UpdateVertex();


		sp.SetAlpha(lerp * 0.5);
		sp.Draw();

		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		sp.SetAlpha(0.5 * (0.5 - lerp * 0.5));
		sp.Draw();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	m_img.Draw();
}

void Boss::Stage5DoremiBook::Update(double x, double y)
{
	auto yoff = 30 * cos(Game::GetGame().GetGameTime() / 40.0);
	auto spx = x + 100 * sin(Game::GetGame().GetGameTime() / 40.0);
	auto spy = y - yoff;
	m_img.SetPos(spx, spy);
	m_img.Update();
	m_img.UpdateVertex();

	m_bossFront = yoff < 0;

	if (Game::GetGame().GetGameTime() % 2 == 0)
	{
		m_tails.push_back(
			Tail{
				m_img.GetNowFrame(),
				Game::GetGame().GetGameTime(),
				spx,spy,
				m_bossFront
			}
		);

		while (m_tails.size() > 15)
			m_tails.pop_front();
	}

	
}
