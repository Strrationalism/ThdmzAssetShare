#include "NuzPrecompiled.h"
#include "GamePausedActivity.h"
#include "ActivityEngine.h"
#include "BGMPlayer.h"
#include "SeManager.h"
#include "Game.h"
#include "glew.h"
#include "Shaders.h"
#include "HRollingFadingMenuItem.h"
#include "TitleMainMenu.h"
#include "GameActivity.h"
#include "RollingBookPage.h"
#include "GetPath.h"
#include "ReplayerActivity.h"
#include "SaveRepActivity.h"
#include "Window.h"
static const float FADE_IN_FRAMES = 0.25f;
static const float FADE_OUT_FRAMES = 0.25f;

void Activities::GamePausedActivity::refreshGameScreenshot()
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Game::GetGame().Draw();

	int w, h;
	Game::GetGame().GetScreenSize(w, h);
	glBindTexture(GL_TEXTURE_2D,*m_gameScreenShot);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, w, h, 0);

	glClear(GL_COLOR_BUFFER_BIT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glColor4f(1, 1, 1, 1);
	Basic::Shaders::GaussBlurW(16);

	glBegin(GL_QUADS);
	{
		glTexCoord2d(0, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(0, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);
	}
	glEnd();
	glUseProgram(0);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, w, h, 0);

	glClear(GL_COLOR_BUFFER_BIT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	Basic::Shaders::GaussBlurH(32);
	glBegin(GL_QUADS);
	{
		glTexCoord2d(0, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(0, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);
	}
	glEnd();
	glUseProgram(0);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, w, h, 0);
}

void Activities::GamePausedActivity::onKey(Basic::Input::Key key,bool down)
{
	switch (key)
	{
	case Basic::Input::Key::Bomb:
	case Basic::Input::Key::Esc:
		if (down && m_bgm == BGM::None)
			resumeGame();
		break;

	case Basic::Input::Key::Shoot:
	case Basic::Input::Key::Enter:
		if (down)
			m_menu->Enter();
		break;
	}
}

void Activities::GamePausedActivity::resumeGame()
{
	hideComponent();

	if(m_seEnabled)
		Basic::Sound::SeManager::PlayHigh("pause2", 0, 16);
	m_eventEnabled = false;
	m_blurDepth.Tween(0.0, FADE_OUT_FRAMES, 2);
	m_taskList.AddTask([]() {
		Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Return();
	}, FADE_OUT_FRAMES);
}

void Activities::GamePausedActivity::hideComponent()
{
	for (const auto& menuItem : *m_menu)
		dynamic_cast<Menus::HRollingFadingMenuItem&>(*menuItem).GetAlpha().Tween(0, 0.25f, 1);
	m_titleAlpha.Tween(0, 0.25f, 1);
}

std::shared_ptr<Basic::Menu::MenuItem> Activities::GamePausedActivity::createMenuItem(
	int imageID,
	std::function<void()>&& func)
{
	auto pItem = std::make_shared<Menus::HRollingFadingMenuItem>(
		"GameUI/PauseActivity",
		imageID,
		0,
		100,
		70,
		std::move(func)
	);

	pItem->GetX() = 0;

	return pItem;
}


Activities::GamePausedActivity::GamePausedActivity():
	m_matcher{
		[this] { m_menu->Down(); },
		[this] { m_menu->Up(); }
	}
{
	m_gameTitle.UseImage("SSLogo", 1);
	m_gameTitle.SetPos(400, 280);
	m_gameTitle.SetAlpha(0);
	m_gameTitle.SetZoom(0.5f);
	m_gameTitle.UpdateVertex();

	m_gameTitle2.UseImage("SSLogo", 2);
	m_gameTitle2.SetPos(400, 280);
	m_gameTitle2.SetAlpha(0);
	m_gameTitle2.SetZoom(0.5f);
	m_gameTitle2.UpdateVertex();

	m_menuItems[(int)MenuItem::Restart] =
		createMenuItem(1, [this] {
		m_gameTitleAlpha.Tween(0, 0.25f, 1);
		Basic::Sound::SeManager::Play("cardget", 0, 120);

		hideComponent();

		m_taskList.AddTask([this] {
			auto pActNow =
				dynamic_cast<Activities::GameActivity::Bridge*>(
					Game::GetGame().GetGameActBridge())->GetAct();

			Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Return();
			Basic::ActivityEngine::ActivityEngine::GetActivityEngine()
				.Goto(Activities::GameActivity::CreateGameActivity(std::move(pActNow->GetParam())));

			Game::GetGame().AddAfterEffect(
				new AfterEffect::RollingBookPage(
					75,
					1,
					m_gameScreenShot
				)
			);
		}, 0.25f);
	});

	m_menuItems[(int)MenuItem::RestartRep] =
		createMenuItem(1, [this] {
		m_gameTitleAlpha.Tween(0, 0.25f, 1);

		Basic::Sound::SeManager::Play("cardget", 0, 120);

		hideComponent();

		m_taskList.AddTask([this] {
			auto pActNow =
				dynamic_cast<Activities::ReplayerActivity::Bridge*>(
					Game::GetGame().GetGameActBridge())->GetAct();

			Game::GetGame().GetScoreManager().SetLife(3);
			Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Return();
			Basic::ActivityEngine::ActivityEngine::GetActivityEngine()
				.Goto(std::make_shared<Activities::ReplayerActivity>(pActNow->GetPath()));

			Game::GetGame().AddAfterEffect(
				new AfterEffect::RollingBookPage(
					75,
					1,
					m_gameScreenShot
				)
			);
		}, 0.25f);
	});


	m_menuItems[(int)MenuItem::Resume] =
		createMenuItem(0, [this] {
		resumeGame();
	});

	m_menuItems[(int)MenuItem::ResumeRep] =
		createMenuItem(6, [this] {
		resumeGame();
	});

	m_menuItems[(int)MenuItem::Exit] =
		createMenuItem(2, [this] {
		hideComponent();
		if(m_bgm != BGM::None)
			Basic::Sound::LowerSound::StopBGM(true);

		Basic::Sound::SeManager::Play("exit", 0,130);
		auto pTitle = std::make_shared<Title::TitleMainMenu>();
		m_mainAlpha.Tween(0, 0.45f, 1);
		pTitle->OnUpdate(0.01f);

		m_taskList.AddTask([pTitle] {
			Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Return();
			Basic::ActivityEngine::ActivityEngine::GetActivityEngine()
				.Goto(pTitle);
		}, 0.45f);
	});

	m_menuItems[(int)MenuItem::SaveReplay] = 
		createMenuItem(3, [this] {
		hideComponent();

		Basic::Sound::SeManager::Play("entry2", 0, 120);		

		m_taskList.AddTask([this] {

			Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Call(
				std::make_shared<SaveRepActivity>(*m_movieData,
				[this] {
					this->OnDraw();
				},
				[this] (float t){
					this->OnUpdate(t);
				},
				[this] {
					if (m_bgm != BGM::None)
						Basic::Sound::LowerSound::StopBGM(true);

					m_mainAlpha.Tween(0, 0.45f, 1);

					m_taskList.AddTask([] {
						auto pTitle = std::make_shared<Title::TitleMainMenu>();
						pTitle->OnUpdate(0.01f);
						Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Return();
						Basic::ActivityEngine::ActivityEngine::GetActivityEngine()
							.Goto(pTitle);
					},0.5f);
				})
			);
		}, 0.45f);
	});
}


void Activities::GamePausedActivity::BuildMenu(std::vector<MenuItem>&& items, bool seEnabled, BGM bgm,int title)
{
	std::vector<std::shared_ptr<Basic::Menu::MenuItem>> itemObjects;

	for (auto itemID : items)
		if(itemID != decltype(itemID)::SaveReplay)		//ÒÆ³ýREPÏµÍ³
			itemObjects.push_back(m_menuItems[(int)itemID]);

	m_menu.emplace(std::move(itemObjects), items.size() >= 3 ? 1 : 0);

	auto onMoveCallback = [] {
		Basic::Sound::SeManager::Play("timeout2", 0);
	};

	m_menu->SetOnDown(onMoveCallback);
	m_menu->SetOnUp(onMoveCallback);
	m_menu->SetEnabled(true);
	m_menu->SetClamp(true);

	m_seEnabled = seEnabled;
	m_bgm = bgm;

	m_title.UseImage("GameUI/PauseActivity", title);
	m_title.SetPos(0, -200);
	m_title.SetZoom(1.25);
	m_title.SetAlpha(0);
	m_title.UpdateVertex();

	m_titleAlpha = 0;
}

void Activities::GamePausedActivity::OnEntry()
{
	Game::GetGame().ReleaseKeys();

	m_taskList.AddTask(
		[this]() {
		m_eventEnabled = true;
	}, FADE_IN_FRAMES);

	Basic::Sound::BgmPlayer::SetPaused(true);
	Basic::Sound::LowerSound::PauseBGM(false);

	if (m_bgm != BGM::None)
	{
		Basic::Sound::LowerSound::PushBGMStack();
		switch (m_bgm)
		{
		case BGM::GameOver:
			Basic::Sound::LowerSound::PlayBGM(Basic::Sound::BGMData::GetBGMData(524), true);
			break;
		case BGM::GameOverWin:
			Basic::Sound::LowerSound::PlayBGM(Basic::Sound::BGMData::GetBGMData(525), true);
			break;
		};
	}

	if(m_seEnabled)
		Basic::Sound::SeManager::PlayHigh("pause",0,16);
	refreshGameScreenshot();

	m_blurDepth.Tween(1.0, FADE_IN_FRAMES, 1);

	for (const auto& menuItem : *m_menu)
		dynamic_cast<Menus::HRollingFadingMenuItem&>(*menuItem).GetAlpha().Tween(1, 0.25f, 1);
	m_titleAlpha.Tween(1, 0.25f, 1);
}

void Activities::GamePausedActivity::OnLeave()
{
	if (m_bgm != BGM::None)
	{
		Basic::Sound::LowerSound::StopBGM(false);
		Basic::Sound::LowerSound::PopBGMStack();
	}
	Basic::Sound::BgmPlayer::SetPaused(false);
	Basic::Sound::LowerSound::ResumeBGM(false);

	m_movieData = nullptr;
}

void Activities::GamePausedActivity::OnDraw() const
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	if (m_blurDepth < 1.0 && m_mainAlpha >= 1)
		Game::GetGame().Draw();

	float r = 1, g = 1, b = 1;

	if(m_bgm == BGM::GameOver)
	{
		r = Mix(1,1, m_gameTitleAlpha);
		g = Mix(1, 0.682352941176471,  m_gameTitleAlpha);
		b = Mix(1, 0.682352941176471, m_gameTitleAlpha);
	}

	glColor4d(r, g, b, m_blurDepth * m_mainAlpha);
	glBindTexture(GL_TEXTURE_2D, *m_gameScreenShot);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	glBegin(GL_QUADS);
	{
		glTexCoord2d(0, 1); glVertex2d(-WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 1); glVertex2d(WIDTH / 2, -HEIGHT / 2);
		glTexCoord2d(1, 0); glVertex2d(WIDTH / 2, HEIGHT / 2);
		glTexCoord2d(0, 0); glVertex2d(-WIDTH / 2, HEIGHT / 2);
	}
	glEnd();
	glUseProgram(0);
	m_title.Draw();
	m_menu->Draw();
	m_gameTitle.Draw();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_gameTitle2.Draw();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Activities::GamePausedActivity::OnUpdate(float deltaTime)
{
	m_mainAlpha.Update(deltaTime);
	m_blurDepth.Update(deltaTime);
	m_taskList.Update(deltaTime);
	m_titleAlpha.Update(deltaTime);
	m_gameTitleAlpha.Update(deltaTime);
	
	m_menu->Update(deltaTime);
	m_title.SetAlpha(m_titleAlpha);
	m_gameTitle.SetAlpha(m_blurDepth * m_mainAlpha * m_gameTitleAlpha);
	m_gameTitle2.SetAlpha(m_blurDepth * m_mainAlpha * m_gameTitleAlpha);
}

void Activities::GamePausedActivity::OnEvent(const Basic::Input::Message & e)
{
	if (!m_eventEnabled) return;
	switch (e.type)
	{
	case Basic::Input::Message::Type::KeyDown:
		onKey(e.key, true);
		break;
	case Basic::Input::Message::Type::KeyUp:
		onKey(e.key, false);
		break;
	case Basic::Input::Message::Type::VecMotion:
		m_matcher.OnEvent(e.vec.y);
		break;
	default:
		break;
	}
}
