#include "Precompiled.h"
#include "TextWindow.h"
#include "Game.h"

using namespace Conversation;

TextWindow::TextWindow()
{
	Sp2DObject::GetSprite().UseImage("Conversation", 0);
	Sp2DObject::GetSprite().SetAlpha(0);
	GameObjectWithDmslControl::SetPos(0, 400);
	GameObjectWithDmslControl::GetDataActor().Add(m_alpha, 0.8, 60, 2);
	GameObjectWithDmslControl::GetDataActor().Add(m_y, 250, 60, 2);
	m_died = false;
}

void Conversation::TextWindow::Update()
{
	Sp2DObject::GetSprite().SetAlpha(m_alpha);
	GameObjectWithDmslControl::SetPos(0, m_y);
	Sp2DObject::Update();
}

void Conversation::TextWindow::Kill()
{
	GameObjectWithDmslControl::GetDataActor().Add(m_alpha, 0, 60, 2);
	GameObjectWithDmslControl::GetDataActor().Add(m_y, 200, 60, 2);
	Game::GetGame().GetTaskList().AddTask([this]() {
		this->m_died = true;
	}, 40);
}