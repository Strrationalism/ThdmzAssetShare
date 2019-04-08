#include "Precompiled.h"
#include "EnemyAnimation.h"
#include "Sprite2D.h"
#include "Shaders2.h"

void Enemy::EnemyAnimation::Push(double x, double y, double r, double g, double b)
{
	Animation ani;
	ani.angle = Rand() * 360;
	ani.frame = 0;
	ani.r = r;
	ani.g = g;
	ani.b = b;
	ani.x = x;
	ani.y = y;

	m_animation.push_back(ani);
}

void Enemy::EnemyAnimation::Update()
{
	for (auto& p : m_animation)
		p.frame += 0.5;

	const auto nend = std::remove_if(m_animation.begin(), m_animation.end(), [](auto& a) {
		return int(a.frame) >= 14;
	});

	m_animation.erase(nend, m_animation.end());
}

void Enemy::EnemyAnimation::Draw() const
{
	Basic::Video::Renderer2D::Sprite2D::ClearFastBuffer();

	Basic::Video::Renderer2D::Sprite2D sp;
	for (const auto& p : m_animation)
	{
		sp.UseImage("EnemyBombAnimation", int(p.frame));
		sp.SetColorFliter(p.r, p.g, p.b);
		sp.SetPos(p.x, p.y);
		sp.SetAngle(p.angle);
		sp.SetAlpha(0.4f);
		sp.UpdateVertex();
		sp.DrawToFastBuffer(Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR);
	}
	Basic::Shaders::ColorConverter(0.4f);
	Basic::Video::Renderer2D::Sprite2D::DrawFastBuffer();
	glUseProgram(0);
}
