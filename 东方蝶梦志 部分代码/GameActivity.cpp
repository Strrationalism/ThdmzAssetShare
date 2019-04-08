#include "NuzPrecompiled.h"
#include "GameActivity.h"
#include "Game.h"
#include "CharReimu.h"
#include "CharMarisa.h"
#include "StageData.h"
#include "ActivityEngine.h"
#include "GamePausedActivity.h"
#include "GetItemBorderHint.h"
#include "TitleMainMenu.h"
#include "RecoderInputer.h"
#include "WrappedSaveRepActivity.h"
#include "GameRebuilder.h"
#include "StaffActivity.h"
#include "PlayerProfile.h"
#include "ImageHintActivity.h"
using namespace Activities;
using namespace std;

void Activities::GameActivity::nextStage()
{
	auto stageName = m_stages.front();
	if (stageName == "End")
	{
		m_running = false;
		m_darkness.Tween(1, 2, 1);
		m_taskList.AddTask([this] {
			if (m_bridge.CanSaveAct())
				Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Goto(
					make_shared<Title::TitleMainMenu>()		//make_shared<Activities::WrappedSaveRepActivity>(std::move(m_movieData))	REP系统移除
				);
			else
				Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Goto(
					make_shared<Title::TitleMainMenu>()
				);
		},3);
		return;
	}
	else if (stageName == "StaffEnd")
	{
		if (m_bridge.CanSaveAct())
		{
			SaveSystem::PlayerProfile::Get().EnableGameStartExtra();
		}

		m_running = false;
		m_darkness.Tween(1, 2, 1);
		m_taskList.AddTask([this] {
			Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Goto(
				make_shared<Activities::StaffActivity>(
					false ?					//m_bridge.CanSaveAct() ?			REP系统移除
					std::move(m_movieData) :
					nullptr
				)
			);
		}, 3);

		return;
	}
	else if (stageName == "StageEx")
	{
		m_movieData->head.isExtra = true;
	}
	m_stages.pop();

	SaveSystem::PlayerProfile::Get().EnablePractice(stageName);

	auto stageData = LoadStage(stageName, m_gameLevel);
	Game::GetGame().Reset();
	Game::GetGame().SetStageData(stageData);
	
	Basic::Sound::BgmPlayer::Play(stageData->stageBgm);

	m_stageName = std::move(stageName);
	wcscpy_s(m_movieData->head.lastStage, Str2WStr(m_stageName).c_str());
}

void Activities::GameActivity::saveStageMovieToMovieData()
{
	try
	{
		//auto& movie = dynamic_cast<Inputer::RecoderInputer&>(*m_inputer).GetMovie();
		//m_movieData->stages[m_stageName] = (std::make_pair(m_stageInitData, movie));
		//Log2("Movie saved.", m_stageName);
	}
	catch (std::bad_cast)
	{
		Crash(L"未能获得录像机实例。");
	}
}

Activities::GameActivity::GameActivity(const std::queue<std::string>& stages,StageLevel level, Char::BasicChar& charater,bool practice) :
	m_bridge(*this,m_pauseActivity,practice),
	m_gameLevel(level),
	m_pauseActivity(std::make_shared<Activities::GamePausedActivity>()),
	m_charater{charater},
	m_stages { std::move(stages) },
	m_param { std::make_tuple(stages,level,&charater,practice) },
	m_inputer { std::make_unique<Inputer::NormalInputer>() }
{
	RebuildGame();

	Game::GetGame().SetGameActBridge(&m_bridge);
	Game::GetGame().GetPlayer().SetChar(m_charater);

	Game::GetGame().SetInputer(*m_inputer);
	Game::GetGame().ReleaseKeys();

	//dynamic_cast<Inputer::RecoderInputer*>(m_inputer.get())->GetMovie().Clear();

	m_movieData->head.level = (int32_t)level;
	m_movieData->head.charater = charater.GetName() == std::string("Reimu") ? MovieData::Head::Reimu : MovieData::Head::Charater::Marisa;
	m_movieData->head.score = 0;
	m_movieData->head.isExtra = false;
	wcscpy_s(m_movieData->head.lastStage, L"");
	wcscpy_s(m_movieData->head.playerName, L"NONAME");

	nextStage();

	Game::GetGame().GetScoreManager().Reset();
	Game::GetGame().GetPlayer().SetPower(3);
	Game::GetGame().GetScoreManager().SetLife(practice ? 10 : 3);

	Game::GetGame().GetUILayerGameObjectManager().Insert(new Item::GetItemBorderHint);

	Game::GetGame().WriteToInitData(m_stageInitData);


	m_darkness = 0;
}

Activities::GameActivity::~GameActivity()
{
}

void Activities::GameActivity::OnEntry()
{
}

void Activities::GameActivity::OnDraw() const
{
	Game::GetGame().Draw();

	if (m_darkness > 0)
	{
		glColor4f(0, 0, 0, m_darkness);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		{
			glVertex2f(-640, -360);
			glVertex2f(640, -360);
			glVertex2f(640, 360);
			glVertex2f(-640, 360);
		}
		glEnd();
		glEnable(GL_TEXTURE_2D);
	}
}

void Activities::GameActivity::OnUpdate(float deltaTime)
{
	if (m_running)
	{
		//for(int i =0;i<100;++i)
		Game::GetGame().Update();

		if (m_accMode)
			for(int i = 0;i < 10;++i)
				Game::GetGame().Update();
	}

	m_darkness.Update(deltaTime);
	m_taskList.Update(deltaTime);
}

void Activities::GameActivity::pause()
{
	m_pauseActivity->BuildMenu({
		GamePausedActivity::MenuItem::Restart,
		GamePausedActivity::MenuItem::Resume,
		GamePausedActivity::MenuItem::Exit
		}, true, GamePausedActivity::BGM::None, 5);

	Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Call(
		m_pauseActivity
	);
}

void Activities::GameActivity::OnEvent(const Basic::Input::Message & e)
{
	using namespace Basic::ActivityEngine;
	using namespace Basic::Input;
	if (e.type == Message::Type::KeyDown)
	{
		if (e.key == decltype(Message::key)::Esc)
		{
			pause();
		}
		/*else if (e.key == decltype(e.key)::F1)
			m_accMode = true;
		else if (e.key == decltype(e.key)::F4)
			Game::GetGame().GetPlayer().Invincible(9999);*/
	}
	else if(e.type == Message::Type::WinFocusLost)
		pause();
	/*else if (e.type == Message::Type::KeyUp)
	{
		if (e.key == decltype(e.key)::F1)
			m_accMode = false;
	}*/
	m_inputer->PushMessage(e);
}

Activities::GameActivity::GameActivityParam Activities::GameActivity::GetParam()
{
	return m_param;
}

std::shared_ptr<GameActivity> Activities::GameActivity::CreateGameActivity(GameActivityParam && param)
{
	return std::make_shared<GameActivity>(
		std::get<0>(param),
		std::get<1>(param),
		*std::get<2>(param),
		std::get<3>(param)
		);
}

Activities::GameActivity::Bridge::Bridge(GameActivity & act, const std::shared_ptr<Activities::GamePausedActivity>& pauser,bool practice):
	m_pauser{pauser},
	m_practice{practice}
{
	m_act = &act;
}

void Activities::GameActivity::Bridge::OnPlayerAllDied()
{
	if (m_canSaveReplay)
	{
		m_act->m_movieData->head.score = Game::GetGame().GetScoreManager().GetScore();
		m_act->saveStageMovieToMovieData();
	}

	Game::GetGame().GetScoreManager().Reset();
	Game::GetGame().GetPlayer().Birth();

	std::vector<GamePausedActivity::MenuItem> itemIDs
	{
		GamePausedActivity::MenuItem::Restart,
		GamePausedActivity::MenuItem::Resume,
	};

	if (m_canSaveReplay)
	{
		itemIDs.push_back(GamePausedActivity::MenuItem::SaveReplay);
		m_act->m_pauseActivity->SetMovieData(m_act->m_movieData.get());
	}

	itemIDs.push_back(GamePausedActivity::MenuItem::Exit);

	m_canSaveReplay = false;
	

	m_pauser->BuildMenu(std::move(itemIDs),false,GamePausedActivity::BGM::GameOver,4);

	Basic::ActivityEngine::ActivityEngine::GetActivityEngine()
		.Call(m_pauser);

	//m_act->m_inputer = std::make_unique<Inputer::NormalInputer>();
	//Game::GetGame().SetInputer(*m_act->m_inputer);
}

void Activities::GameActivity::Bridge::OnStageClear()
{
	if (m_canSaveReplay)
	{
		//m_act->saveStageMovieToMovieData();
		//m_act->m_inputer = std::make_unique<Inputer::RecoderInputer>();
		//Game::GetGame().SetInputer(*m_act->m_inputer);
	}

	Game::GetGame().WriteToInitData(m_act->m_stageInitData);
	m_act->nextStage();
	Game::GetGame().LoadFromInitData(m_act->m_stageInitData);

	m_act->m_movieData->head.score = Game::GetGame().GetScoreManager().GetScore();
}


void Activities::GameActivity::Bridge::FinishSpell(int spellHash, bool bonusGot)
{
	if (!m_practice)
		SaveSystem::PlayerProfile::Get().SpellFinished(spellHash, bonusGot);
}
