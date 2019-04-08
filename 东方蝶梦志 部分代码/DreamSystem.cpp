#include "Precompiled.h"
#include "DreamSystem.h"
#include "Game.h"
UI::DreamSystem::DreamSystem()
{
	m_data = 0;
	m_line.UseImage("GameUI/Dream", 0);
	m_line.SetPos(0, HEIGHT / 2 - 35);
	m_main.UseImage("GameUI/Dream", 1);
	m_main.SetPos(0, HEIGHT / 2 - 25);
	m_pointer.UseImage("GameUI/Dream", 2);
	m_alpha = 1;
}

void UI::DreamSystem::Reset()
{
	m_data = Game::GetGame().GetScoreManager().GetDream();
	m_alpha = 1;
	m_enabled = true;
	m_dataActor.Clear();
}

void UI::DreamSystem::Draw() const
{
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_main.Draw();
	m_pointer.Draw();
	m_line.Draw();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void UI::DreamSystem::Update()
{
	m_dataActor.Update();

	auto displayData = m_data;
	if (Game::GetGame().GetPlayer().GetSheild().has_value())
	{
		auto& sheild = Game::GetGame().GetPlayer().GetSheild().value();
		displayData = sheild.GetPercent();
		const bool isLeft = sheild.IsLeft();
		if (isLeft)
			displayData = -displayData;

		Game::GetGame().GetParticleTop().AddDot(2,
			[isLeft](auto& dot)
		{
			dot.r = isLeft ? 0.609375 : 1;
			dot.g = isLeft ? 0.26953125 : 1;
			dot.b = isLeft ? 0.859375 : 1;
			dot.blend_dst = GL_ONE;// isLeft ? GL_ONE_MINUS_SRC_ALPHA : GL_ONE;
			dot.alpha = 1;
			dot.size = 32;
			dot.speed = 3;
			dot.angle = Rand() * M_PI + 0.5 * M_PI;
			dot.self_angle = Rand() * 360;

			dot.x += (Rand() * 2 - 1) * 80;
		},
			[](auto& dot,auto& sys)
		{
			dot.speed *= 0.95;
			dot.alpha = dot.alpha * 0.92;
			if (dot.alpha < 0.01)
				dot.alpha = -1;
		}, 0, 360,5);
	}
	
	m_pointer.SetPos(displayData * 87.0, HEIGHT / 2 - 35);

	m_pointer.SetAlpha(m_alpha);
	m_line.SetAlpha(m_alpha);
	m_main.SetAlpha(m_alpha);

	//¼ÆËãÑÕÉ«
	double r, g, b, data = (displayData + 1) / 2;
	/*if (m_data > 0) {
		r = 170.0 / 255;
		g = 21.0 / 255;
		b = 34.0 / 255;
		data = m_data;
	}
	else if (m_data < 0) {
		r = 255 / 255;
		g = 255 / 255;
		b = 255 / 255;
		data = -m_data;
	}*/

	//if (m_data != 0) {
	r = Mix(0.609375, 1, data);
	g = Mix(0.26953125, 1, data);
	b = Mix(0.859375, 1, data);
	//}
	//else {
		//r = 0;
		//g = 64.0 / 255;
		//b = 129.0 / 255;
	//}
	m_main.SetColorFliter(r, g, b);
	m_pointer.SetColorFliter(r, g, b);
	m_main.UpdateVertex();
	m_pointer.UpdateVertex();
	m_line.UpdateVertex();
}

void UI::DreamSystem::SetData(double d)
{
	m_dataActor.Add(m_data, d, 20, 1);
}

void UI::DreamSystem::ZeroAlpha()
{
	if (m_enabled)
		m_dataActor.Add(m_alpha, 0, 20, 1);
}

void UI::DreamSystem::HalfAlpha()
{
	if (m_enabled)
		m_dataActor.Add(m_alpha, 0.3, 20, 1);
}

void UI::DreamSystem::FullAlpha()
{
	if (m_enabled)
		m_dataActor.Add(m_alpha, 1, 20, 1);
}

/*
void UI::DreamSystem::Enable(bool b)
{
	m_enabled = b;
	Game::GetGame().GetScoreManager().SetDreamEnabled(b);
	m_dataActor.Clear();
	m_dataActor.Add(m_alpha, b ? 1 : 0, 20, 1);
}*/