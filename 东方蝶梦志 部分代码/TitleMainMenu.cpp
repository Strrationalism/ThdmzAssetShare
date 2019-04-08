#include "NuzPrecompiled.h"
#include "TitleMainMenu.h"
#include "Shaders.h"
#include "BGMPlayer.h"
#include "Window.h"
#include "ImageCacheManager.h"
#include "ActivityEngine.h"
#include "GameActivity.h"
#include "HRollingFadingMenuItem.h"
#include "SeManager.h"
#include "GameActivity.h"
#include "TitleMenuMainMenu.h"
using namespace std;
using namespace Basic::Video::LowerRenderer;
using namespace Basic::ActivityEngine;
using namespace Activities::Title;
using namespace Menus;

Activities::Title::TitleMainMenu::TitleMainMenu()
{
	CallSubMenu(make_unique<MenuStack::TitleMenuMainMenu>(this));
}

void TitleMainMenu::OnEntry()
{
	Basic::Sound::BgmPlayer::Play(523);
}

void TitleMainMenu::OnDraw() const
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	m_background.Draw(
		[this] {
			for (const auto& p : m_menuStack)
				p->OnDrawYSJD();
		},
		[this] {
			for (const auto& p : m_menuStack)
				p->OnDraw();
		});

	for (const auto& p : m_menuStack)
		p->OnDrawPost();
}

void TitleMainMenu::OnUpdate(float deltaTime)
{
	m_background.Update(deltaTime);
	m_taskList.Update(deltaTime);

	for (const auto& p : m_menuStack)
		p->OnUpdate(deltaTime);

	while (m_backMenu)
	{
		m_backMenu--;
		m_menuStack.pop_back();
		m_menuStack.back()->OnBackToThisShow();
		m_menuStackEventPass = true;
	}

	if (m_prepAdd.has_value())
	{
		m_menuStack.push_back(std::move(m_prepAdd.value()));
		m_prepAdd.reset();
		m_menuStack.back()->OnEntryToThis();
	}
}

void Activities::Title::TitleMainMenu::OnEvent(const Basic::Input::Message & e)
{
	if(m_menuStackEventPass && !m_menuStack.empty())
		m_menuStack.back()->OnEvent(e);
}

void Activities::Title::TitleMainMenu::CallSubMenu(std::unique_ptr<TitleMenuStackFrame>&& p)
{
	SDL_assert(!m_prepAdd.has_value());

	if(!m_menuStack.empty())
		m_menuStack.back()->OnEntryOther();

	m_prepAdd = std::move(p);
}

void Activities::Title::TitleMainMenu::BackThisMenu()
{
	m_backMenu++;
}

void Activities::Title::TitleMainMenu::WannaBackThisMenu()
{
	m_menuStack[m_menuStack.size() - 2]->OnWannaBackToThisShow();
	m_menuStackEventPass = false;
}


