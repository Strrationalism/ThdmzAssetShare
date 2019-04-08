#include "Precompiled.h"
#include "DreamPointItem.h"
#include "Game.h"

static MemoryPool<Item::DreamPointItem, 100> pool;

Item::DreamPointItem::DreamPointItem(double x, double y, double dreamPower,int wait):
	m_x(x),
	m_y(y),
	m_isDarkTheme(dreamPower < 0)
{
	m_died = false;

	m_sp.UseImage("Particle3D", 3);
	m_sp.SetZoom(0.75);
	const auto r = m_isDarkTheme ? 0.609375 : 4 * 0.780392156862745;
	const auto g = m_isDarkTheme ? 0.26953125 : 4 * 0.294117647058824;
	const auto b = m_isDarkTheme ? 0.859375 : 4 * 0.847058823529412;

	m_sp.SetColorFliter(r,g,b);

	m_taskList.AddTask([this]()
	{
		m_dataActor.Add(m_x, 0, 50, 1);
		m_dataActor.Add(m_y, HEIGHT / 2 + 25, 50, 1);
		m_part = true;
	}, wait);

	m_taskList.AddTask([this, dreamPower]()
	{
		Game::GetGame().GetScoreManager().AddDreamPoint(dreamPower);
		Kill();
		m_part = false;
	}, 50 + wait);
}

void Item::DreamPointItem::AddToGame(double x, double y, double dreamPower,int wait)
{
	if (!Game::GetGame().GetPlayer().Died())
	{
		Game::GetGame().GetMiscManagerBottom().Insert(
			new DreamPointItem(x, y, dreamPower, wait)
		);
	}
}

void Item::DreamPointItem::Update()
{
	using namespace Basic::Video::Renderer3D;
	m_taskList.Update();
	m_dataActor.Update();
	m_sp.SetPos(m_x, m_y);
	m_sp.UpdateVertex();

	if (m_part && Game::GetGame().GetGameTime() % 3 == 0)
	{
		Game::GetGame().GetParticleBottom().AddDot(3,[this](Particle3D::Dot3D& dot)
		{
			dot.r = m_isDarkTheme ? 0.609375 : 4 * 0.780392156862745;
			dot.g = m_isDarkTheme ? 0.26953125 : 4 * 0.294117647058824;
			dot.b = m_isDarkTheme ? 0.859375 : 4 * 0.847058823529412;
			dot.blend_dst = m_isDarkTheme ? GL_ONE_MINUS_SRC_ALPHA : GL_ONE;
			dot.alpha = 1;
			dot.size = 256;
		}, [this](Particle3D::Dot3D& dot,Particle3D& sys) {
			dot.alpha = dot.alpha * 0.8;
			dot.size = dot.size * 0.8;
			if (dot.alpha < 0.01)
				dot.alpha = -1;
		}, m_x, m_y);
	}
}

void Item::DreamPointItem::Draw() const
{
	m_sp.DrawToFastBuffer(
		m_isDarkTheme ? 
		Basic::Video::Renderer2D::FASTBUFFER_BLEND_NORMAL :
		Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR);
}

void Item::DreamPointItem::Kill()
{
	m_died = true;
}

void * Item::DreamPointItem::operator new(size_t)
{
	return pool.Get();
}

void Item::DreamPointItem::operator delete(void * ptr)
{
	pool.Return(ptr);
}
