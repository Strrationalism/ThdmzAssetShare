#include "NuzPrecompiled.h"
#include "HRollingFadingMenuItem.h"

static constexpr float FADE_TIME = 0.45f;

void Menus::HRollingFadingMenuItem::updateSelectAnimation(int offset)
{

	switch (offset)
	{
	default:
		break;
	case 6:
		offset = -1;
		break;
	case -6:
		offset = 1;
		break;
	case 5:
		offset = -2;
		break;
	case -5:
		offset = 2;
		break;
	case 4:
		offset = -3;
		break;
	case -4:
		offset = 3;
		break;
	};

	m_ytween.Tween(m_y + m_yOffset * offset * -1, FADE_TIME,2);

	if (offset == 0)
		m_systemAlpha.Tween(1, FADE_TIME, 2);
	else if (offset == 1 || offset == -1)
		m_systemAlpha.Tween(0.5, FADE_TIME, 2);
	else
		m_systemAlpha.Tween(0, FADE_TIME, 2);

	m_zoom.Tween(offset ? 1 : 1.1, FADE_TIME, 2);


}

Menus::HRollingFadingMenuItem::HRollingFadingMenuItem(const std::string & imageName, int imageNum, float x, float y,float yOffset, const std::function<void()>& onSelected):
	m_y{ y },
	m_yOffset{ yOffset },
	m_onSelected{ onSelected }
{
	m_sp.UseImage(imageName, imageNum);
	m_ytween = m_y;

	int sx, sy;
	m_sp.GetSize(sx, sy);
	m_x = x + sx / 2.0f;
}

void Menus::HRollingFadingMenuItem::OnInitSelect(int offset)
{
	m_ytween = m_y + m_yOffset * offset * -1;
	m_zoom = 1.0;
	updateSelectAnimation(offset);
}

void Menus::HRollingFadingMenuItem::OnSelectChanged(int offset)
{
	updateSelectAnimation(offset);
}

void Menus::HRollingFadingMenuItem::OnEntry(int offset)
{
	if(!offset) m_onSelected();
}

void Menus::HRollingFadingMenuItem::Update(float deltaTime)
{
	m_zoom.Update(deltaTime);
	m_systemAlpha.Update(deltaTime);
	m_alpha.Update(deltaTime);
	m_ytween.Update(deltaTime);
	m_sp.SetPos(m_x, m_ytween);
	m_sp.SetAlpha(m_systemAlpha * m_alpha);
	m_sp.SetZoom(m_zoom);
	m_sp.UpdateVertex();
	m_x.Update(deltaTime);
}

void Menus::HRollingFadingMenuItem::Draw() const
{
	m_sp.OldDraw();
}
