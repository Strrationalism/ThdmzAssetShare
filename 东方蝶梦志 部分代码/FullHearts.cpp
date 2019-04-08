#include "Precompiled.h"
#include "FullHearts.h"
#include "Game.h"

void UI::FullHearts::incHeart()
{
	m_heartX += 24;
	m_hearts.push_back(std::make_unique<Basic::Video::Renderer2D::Sprite2D>());
	m_hearts.back()->UseImage("GameUI/SmallUI", 2);
	m_hearts.back()->SetPos(-555 + m_heartX, -280);
	m_hearts.back()->UpdateVertex();
	m_label.UseImage("GameUI/SmallUI", 1);
	m_label.SetPos(-515, -275);
	m_label.UpdateVertex();
}

void UI::FullHearts::decHeart()
{
	m_heartX -= 24;
	m_hearts.pop_back();
}

UI::FullHearts::FullHearts()
{
	m_heartX = 0;
	m_label.UseImage("GameUI/SmallUI", 1);
	m_alpha = 1;
}

void UI::FullHearts::Draw() const
{
	m_label.Draw();
	for (auto& p : m_hearts)
		p->Draw();
}

void UI::FullHearts::Update()
{
	m_label.SetAlpha(m_alpha);
	for (auto& p : m_hearts)
		p->SetAlpha(m_alpha);
}

void UI::FullHearts::SetData(int lifeNum)
{
	if (lifeNum < 0)
		lifeNum = 0;

	auto num = static_cast<unsigned>(lifeNum);

	while (num < m_hearts.size())
		decHeart();
	while (num > m_hearts.size())
		incHeart();
}

void UI::FullHearts::HalfAlpha()
{
	Game::GetGame().GetDataActor().Add(m_alpha, 0.4, 40, 1);
}

void UI::FullHearts::FullAlpha()
{
	Game::GetGame().GetDataActor().Add(m_alpha, 1.0, 40, 1);
}