#include "Precompiled.h"
#include "Sp2DObject.h"

void Sp2DObject::Draw() const
{
	m_sp.Draw();
}

void Sp2DObject::Update()
{
	GameObjectWithDmslControl::Update();
	double x, y;
	GameObjectWithDmslControl::GetPos(x, y);
	m_sp.SetPos(x, y);
	m_sp.UpdateVertex();
}

void Sp2DObject::Kill()
{
	m_died = true;
}

