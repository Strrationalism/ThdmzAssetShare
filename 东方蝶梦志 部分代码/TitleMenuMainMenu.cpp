#include "NuzPrecompiled.h"
#include "TitleMenuMainMenu.h"
#include "SeManager.h"
#include "ActivityEngine.h"
#include "BGMPlayer.h"
#include "HRollingFadingMenuItem.h"
#include "GameActivity.h"
#include "TitleMenuMainMenu.h"
#include "TitleCharaterSelectMenu.h"
#include "MenuItem.h"
#include "Shaders.h"
#include "TitleActionMenu.h"
#include "GameActivity.h"
#include "BasicChar.h"
#include "TitleStageSelectMenu.h"
#include "TitleJoystickNotConnectedHint.h"
#include "TitleToGame.h"
#include "Game.h"
#include "TitlePlayerData.h"
#include "TitleReplay.h"
#include "PlayerProfile.h"
using namespace std;
using namespace Menus;



std::shared_ptr<Basic::Menu::MenuItem> Activities::Title::MenuStack::TitleMenuMainMenu::makeMainMenu(int imgID, const std::function<void()>& func)
{
	return std::make_shared<Menus::HRollingFadingMenuItem>(
		"TitleMenu",
		imgID,
		-400,
		100,
		70,
		func
		);
}



Activities::Title::MenuStack::TitleMenuMainMenu::TitleMenuMainMenu(TitleMainMenu* m) :
	m_menuManager{ m },
	m_menu(std::vector<shared_ptr<Basic::Menu::MenuItem>>
{

		makeMainMenu(0, [this] {
			auto selectCharLevelMenu = make_unique<TitleCharaterSelectMenu>(m_menuManager,SaveSystem::PlayerProfile::Get().GameStartExtraEnabled());
			auto menuPointer = selectCharLevelMenu.get();
			selectCharLevelMenu->SetCallback([menuPointer,this](bool isExtra, StageLevel l, Char::BasicChar& ch)
			{
				auto actionMenu = std::make_unique<TitleActionMenu>([this, isExtra,l,&ch]() {
					std::queue<std::string> stages;
					if (isExtra)
					{
						stages.push("StageEx");
						stages.push("End");
					}
					else
					{
						stages.push("Stage1");
						stages.push("Stage2");
						stages.push("Stage3");
						stages.push("Stage4");
						stages.push("Stage5");
						stages.push("Stage6");
						stages.push("StaffEnd");
					}
					
					auto gameAct = make_shared<GameActivity>(std::move(stages), l, ch,false);

					auto subMenu = make_unique<TitleToGame>(gameAct);
					m_menuManager->CallSubMenu(std::move(subMenu));
				},1.0f);

				m_menuManager->CallSubMenu(std::move(actionMenu));
			});

			m_menuManager->CallSubMenu(std::move(selectCharLevelMenu));
		}),
			makeMainMenu(1, [this] {
			auto selectCharLevelMenu = make_unique<TitleCharaterSelectMenu>(m_menuManager,SaveSystem::PlayerProfile::Get().GetPracticeEnabled(6));

			selectCharLevelMenu->SetCallback([this](bool isExtra, StageLevel l, Char::BasicChar& ch)
			{
				auto stageSelect = make_unique<TitleStageSelectMenu>(m_menuManager, isExtra);

				stageSelect->SetCallback([l, &ch, this, isExtra](int stageID) {
					auto subMenu = make_unique<TitleActionMenu>([l, &ch, this, isExtra, stageID] {
						
						std::queue<std::string> stages;
						if (stageID == 6)
							stages.push("StageEx");
						else
						{
							stages.push("Stage" + std::to_string(stageID + 1));
						}
						stages.push("End");

						auto gameAct = make_shared<GameActivity>(std::move(stages), l, ch,true);
						m_menuManager->CallSubMenu(std::make_unique<TitleToGame>(gameAct));
					},1.0f);
					m_menuManager->CallSubMenu(std::move(subMenu));
				});

				m_menuManager->CallSubMenu(std::move(stageSelect));

			});

			m_menuManager->CallSubMenu(std::move(selectCharLevelMenu));
		}),
		/*makeMainMenu(2, [this] {
			m_menuManager->CallSubMenu(make_unique<TitleReplay>(m_menuManager));
		}),*/
		makeMainMenu(3, [this] {
			m_menuManager->CallSubMenu(make_unique<TitlePlayerData>(m_menuManager));
		}),
		makeMainMenu(4, [this] {
			m_menuManager->CallSubMenu(make_unique<TitleJoystickNotConnectedHint>(m_menuManager));
		}),
		makeMainMenu(5, [this] {
			m_menu.SetEnabled(false);
			Basic::Sound::BgmPlayer::Stop(true);
			Basic::Sound::SeManager::Play("exit", 0,120);
			
			m_logoAlpha.Tween(0, 1, 0);
			
			for (auto& p : m_menu)
				dynamic_pointer_cast<HRollingFadingMenuItem>(p)->GetAlpha().Tween(0, 0.25f, 0);

			m_menuManager->GetBackground().FadeToBlackScreen();

			Basic::ActivityEngine::ActivityEngine::GetActivityEngine().GetTaskList().AddTask([this] {
				Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Exit();
			}, 150); 
	})
}, 0),
m_vmacther{ [this] {m_menu.Down(); },[this] {m_menu.Up(); } },
m_version{{580,340,600,600}}
{
	m_taskList.AddTask([this] {
		m_logoAlpha.Tween(1, 1, 1);
	}, 2.9f);

	auto menuClick = []() {
		Basic::Sound::SeManager::Play("timeout2", 0);
	};
	m_menu.SetOnUp(menuClick);
	m_menu.SetOnDown(menuClick);
	m_menu.SetClamp(true);

	for (auto& p : m_menu)
		dynamic_pointer_cast<HRollingFadingMenuItem>(p)->GetAlpha() = 0;


	m_awanljanCharLight.UseImage("AwanLjanWingYDLight");
	m_awanljanCharLight.SetPos(200, 200);
	m_awanljanCharLight.SetZoom(0.7);
	m_awanljanCharLight.UpdateVertex();

	m_awanljanChar.UseImage("AwanLjanWing");
	m_awanljanChar.SetPos(200, 200);
	m_awanljanChar.SetZoom(0.7);
	m_awanljanChar.UpdateVertex();

	m_titleZoom = 1;
	m_titleX = 0;
	m_titleY = -100;


	m_titleShadow.UseImage("SSLogo", 1);

	m_titleSpec.UseImage("SSLogo", 2);

	m_titleAddtive.UseImage("SSLogo", 3);

	m_ssLogo.UseImage("SSLogo", 0);
	m_ssLogo.SetZoom(0.2);
	m_ssLogo.SetPos(-500, 320);
	m_ssLogo.UpdateVertex();


	extern Basic::Video::Renderer2D::CtxFont scFont;
	Basic::SnowRVReader verInfo("Version.rv");
	std::wstring verStr = std::to_wstring(verInfo.GetInt("MAJOR_VER")) + L"." + std::to_wstring(verInfo.GetInt("MINOR_VER"));
	scFont.SetSprite2DWStreamToString(verStr, m_version);
	m_version.SetSpriteSizeOffset(5,0);
	m_version.UpdateStream();

	for(auto& p:m_version)
		p->SetAlpha(0);
}

void Activities::Title::MenuStack::TitleMenuMainMenu::OnEntryToThis()
{
	m_taskList.AddTask([this]() {
		m_menu.SetEnabled(true);

		for (auto& p : m_menu)
			dynamic_pointer_cast<HRollingFadingMenuItem>(p)->GetAlpha().Tween(1, 1.4f, 1);
	}, 4.5);
}

void Activities::Title::MenuStack::TitleMenuMainMenu::OnEntryOther()
{
	Basic::Sound::SeManager::Play("entry", 0,120);

	for (auto& p : m_menu)
	{
		dynamic_pointer_cast<HRollingFadingMenuItem>(p)->GetAlpha().Tween(0, 0.25f, 0);
	}

	m_orgMenuX = dynamic_pointer_cast<HRollingFadingMenuItem>(*m_menu.begin())->GetX();

	m_awanljanAlpha.Tween(0, 0.75f, 1);

	m_titleZoom.Tween(0.5f, 0.5f, 1);
	m_titleX.Tween(400, 0.5f, 1);
	m_titleY.Tween(280, 0.5f, 1);

	m_menuManager->GetBackground().GetStarSkyAlpha().Tween(0, 0.25f, 1);
}

void Activities::Title::MenuStack::TitleMenuMainMenu::OnWannaBackToThisShow()
{
	for (auto& p : m_menu)
	{
		dynamic_pointer_cast<HRollingFadingMenuItem>(p)->GetAlpha().Tween(1, 0.25f, 0);
	}
	m_awanljanAlpha.Tween(1, 0.25f, 1);

	m_titleZoom.Tween(1, 0.25f, 1);
	m_titleX.Tween(0, 0.25f, 1);
	m_titleY.Tween(-100, 0.25f, 1);

	m_menuManager->GetBackground().GetStarSkyAlpha().Tween(1, 0.25f, 1);
}



void Activities::Title::MenuStack::TitleMenuMainMenu::OnDraw() const
{
	m_titleShadow.OldDraw();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	m_titleAddtive.OldDraw();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Activities::Title::MenuStack::TitleMenuMainMenu::OnDrawYSJD() const
{
	m_ssLogo.OldDraw();

	if (m_awanljanAlpha > 0)
	{
		if(Background::BaseBackground::UsingBeautifulBG())
			Basic::Shaders::GaussBlurW(0.000001f + 16 * (1- m_awanljanAlpha));
		m_awanljanChar.OldDraw();
		m_awanljanCharLight.OldDraw();
		glUseProgram(0);
	}

	m_titleSpec.OldDraw();

	m_menu.Draw();

	m_version.Draw();
}

void Activities::Title::MenuStack::TitleMenuMainMenu::OnDrawPost() const
{
	
}

void Activities::Title::MenuStack::TitleMenuMainMenu::OnUpdate(float deltaTime)
{
	m_titleZoom.Update(deltaTime);
	m_titleX.Update(deltaTime);
	m_titleY.Update(deltaTime);


	m_titleAddtive.SetAlpha(m_logoAlpha);
	m_titleAddtive.SetPos(m_titleX, m_titleY);
	m_titleAddtive.SetZoom(m_titleZoom);
	m_titleAddtive.UpdateVertex();

	m_titleShadow.SetAlpha(m_logoAlpha);
	m_titleShadow.SetPos(m_titleX, m_titleY);
	m_titleShadow.SetZoom(m_titleZoom);
	m_titleShadow.UpdateVertex();

	m_titleSpec.SetAlpha(m_logoAlpha);
	m_titleSpec.SetPos(m_titleX, m_titleY);
	m_titleSpec.SetZoom(m_titleZoom);
	m_titleSpec.UpdateVertex();

	m_awanljanChar.SetAlpha(m_awanljanAlpha * 0.55F);
	m_awanljanCharLight.SetAlpha(m_awanljanAlpha);

	m_awanljanAlpha.Update(deltaTime);

	m_logoAlpha.Update(deltaTime);
	m_menu.Update(deltaTime);
	m_taskList.Update(deltaTime);

	m_ssLogo.SetAlpha(0.5 * m_awanljanAlpha);

	for (auto& p : m_version)
		p->SetAlpha(m_awanljanAlpha * m_logoAlpha);
}

void Activities::Title::MenuStack::TitleMenuMainMenu::OnEvent(const Basic::Input::Message & msg)
{

	if (msg.type == Basic::Input::Message::Type::KeyDown)
	{
		if (msg.key == Basic::Input::Key::Shoot || msg.key == Basic::Input::Key::Enter)
			m_menu.Enter();
		else if (msg.key == Basic::Input::Key::Bomb || msg.key == Basic::Input::Key::Esc)
		{
			const auto quitID = m_menu.Size() - 1;
			if (m_menu.GetSelected() == quitID)
			{
				m_menu.Enter();
			}
			else
			{
				if(m_menu.Enabled())
					m_menu.SelectItem(quitID);
			}
		}
	}

	else if (msg.type == Basic::Input::Message::Type::VecMotion)
	{

		m_vmacther.OnEvent(msg.vec.y);
	}

}
