#include "Precompiled.h"
#include "EnemyWarning.h"

Enemy::EnemyWarning::EnemyWarning(double y)
{
	m_died = false;
	m_sprite.UseImage("GameUI/EnemyWarning");
	m_sprite.SetPos(-420, y);
	m_sprite.SetZoom(0.5);
	m_sprite.UpdateVertex();

	m_alpha = 1;
	for (int i = 0; i < 5; ++i)
	{
		m_taskList.AddTask([this]() {
			m_dataActor.Add(m_alpha, 1.0, 10, 1);
		}, i * 50);

		m_taskList.AddTask([this]() {
			m_dataActor.Add(m_alpha, 0.0, 10, 1);
		}, i * 50 + 40);
	}

	m_taskList.AddTask([this]() {
		Kill();
	}, 6 * 50 + 40);
}

void Enemy::EnemyWarning::Update()
{
	m_sprite.SetAlpha(m_alpha);
	m_dataActor.Update();
	m_taskList.Update();
}

void Enemy::EnemyWarning::Draw() const
{
	m_sprite.Draw();
}

void Enemy::EnemyWarning::Kill()
{
	m_died = true;
}
