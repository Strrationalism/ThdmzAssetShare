#include "Precompiled.h"
#include "ConversationTask.h"
#include "TextWindow.h"
#include "ImageCacheManager.h"
#include "Game.h"
#include "UIManager.h"

void StageTask::Conversation::Create(Basic::CSVReader & c, const StageData&)
{
	m_scriptLoader.LoadScript(c.PopString() + "_" + Game::GetGame().GetPlayer().GetChar()->GetName());
	m_leftLHCache = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage("Charater/" + m_scriptLoader.GetRightLHName());
	m_rightLHCache = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage(std::string("Charater/") + Game::GetGame().GetPlayer().GetChar()->GetName());

	m_clickEnabled = c.PopInt() != 0;

	if (c.PopInt() > 0)
		m_logic.EnableNameCard();
}

void StageTask::Conversation::Proc()
{
	if (m_first) {
		Game::GetGame().GetItemMgr().GetAll();
		Game::GetGame().GetUI().GetGrazeDisplay().Hide();
		m_first = false;
		Game::GetGame().GetPlayer().SetShootBombEnabled(false);
		m_textWindow = new ::Conversation::TextWindow;
		m_logic.UseScript(m_scriptLoader, Game::GetGame().GetPlayer().GetChar()->GetName());
		m_logic.ClickEnabled(m_clickEnabled);
		m_display = new ::Conversation::ConversationLogicDisplay(&m_logic);
		Game::GetGame().GetTaskList().AddTask([this]() {
			this->m_logicRunning = true;
			Game::GetGame().GetUILayerGameObjectManager().Insert(m_display);
		}, 60);
		Game::GetGame().GetPlayer().SetShootKeyEvent([this]() {
			m_logic.Click();
		});

		m_left = new ::Conversation::ConversationStandImage();
		m_right = new ::Conversation::ConversationStandImage();
		((::Conversation::ConversationStandImage*)m_right)->SetCharater(m_scriptLoader.GetRightLHName());
		((::Conversation::ConversationStandImage*)m_right)->SetXPos(400, 500, 600);
		((::Conversation::ConversationStandImage*)m_left)->SetCharater(Game::GetGame().GetPlayer().GetChar()->GetName());
		((::Conversation::ConversationStandImage*)m_left)->SetXPos(-400, -500, -600);
		Game::GetGame().GetUILayerGameObjectManager().Insert(m_textWindow);
		Game::GetGame().GetUILayerGameObjectManager().Insert(m_left);
		Game::GetGame().GetUILayerGameObjectManager().Insert(m_right);
		m_logic.SetLHObject(((::Conversation::ConversationStandImage*)m_left), ((::Conversation::ConversationStandImage*)m_right));
	}

	if (m_logicRunning) {
		m_logicRunning = m_logic.Update();
		if (!m_logicRunning)
			Over();
	}
}

//Ö´ÐÐÈÎÎñ

void StageTask::Conversation::Over() {
	m_logicRunning = false;
	m_textWindow->Kill();
	m_display->Kill();
	m_left->Kill();
	m_right->Kill();
	Game::GetGame().GetPlayer().SetShootKeyEvent([]() {});
	Game::GetGame().GetTaskList().AddTask([this]() {
		this->m_end = true;
	}, 60);
	Game::GetGame().GetUI().GetGrazeDisplay().Show();
	Game::GetGame().GetPlayer().SetShootBombEnabled(true);
}

bool StageTask::Conversation::Finished()
{
	return m_end;
}