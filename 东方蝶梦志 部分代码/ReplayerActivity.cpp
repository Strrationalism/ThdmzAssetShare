#include "NuzPrecompiled.h"
#include "ReplayerActivity.h"
#include "Game.h"
#include "CharMarisa.h"
#include "CharReimu.h"
#include "GamePausedActivity.h"
#include "ActivityEngine.h"
#include "GameRebuilder.h"

void Activities::ReplayerActivity::loadStageAndMovie()
{
	if (m_stageQueue.empty())
	{
		m_pause->BuildMenu(
			{
				Activities::GamePausedActivity::MenuItem::Exit,
				Activities::GamePausedActivity::MenuItem::RestartRep
			},
			false,
			Activities::GamePausedActivity::BGM::GameOverWin,
			13
		);

		Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Call(m_pause);
		return;
	}

	const auto stageName = m_stageQueue.front();
	m_stageQueue.pop();

	Game::GetGame().GetScoreManager().SetLife(3);

	//ÔØÈëÂ¼Ïñ
	auto& movie = m_movieData->stages.at(stageName);
	m_inputer.GetMovie() = movie.second;

	Game::GetGame().SetGameActBridge(&m_bridge);
	Game::GetGame().GetPlayer().SetChar(m_movieData->head.charater == MovieData::Head::Reimu ?
		(Char::BasicChar&)Char::Reimu::CharReimu::Get() :
		(Char::BasicChar&)Char::Marisa::CharMarisa::Get());

	Game::GetGame().SetInputer(m_inputer);

	auto stageData = LoadStage(stageName, (StageLevel)m_movieData->head.level);
	Game::GetGame().Reset();
	Game::GetGame().SetStageData(stageData);
	Game::GetGame().GetScoreManager().Reset();

	Game::GetGame().LoadFromInitData(movie.first);

	Basic::Sound::BgmPlayer::Play(stageData->stageBgm);
}

Activities::ReplayerActivity::ReplayerActivity(const std::string& path) :
	m_movieData{std::make_unique<MovieData::MovieData>()},
	m_bridge{ this },
	m_pause{ std::make_shared<Activities::GamePausedActivity>() },
	m_path{ path }
{
	RebuildGame();
	std::ifstream in(path, std::ios::binary);
	if (!in.good())
	{
		Crash(L"Â¼Ïñ¶ÁÈ¡Ê§°Ü¡£");
	}

	MovieData::ReadMovie(*m_movieData,in);
	in.close();

	if (m_movieData->head.version != MovieData::MOVIE_VERSION)
		Crash(L"Â¼Ïñ°æ±¾Óëµ±Ç°°æ±¾²»¼æÈÝ¡£");

	m_repMark.UseImage("GameUI/Replay");
	m_repMark.SetPos(-300, 330);
	m_repMark.SetAlpha(0);
	m_repMark.UpdateVertex();

	for (const auto& p : m_movieData->stages)
		m_stageQueue.push(p.first);

	loadStageAndMovie();

	m_timer = 0;
}

void Activities::ReplayerActivity::OnDraw() const
{
	Game::GetGame().Draw();

	m_repMark.Draw();
}

void Activities::ReplayerActivity::OnUpdate(float deltaTime)
{
	m_frameTimer++;
	if (m_superacc)
	{
		m_timer += 10 * deltaTime;
		for (int i = 0; i < 10; ++i)
			Game::GetGame().Update();
	}
	else if (m_accmode)
	{
		m_timer += 3 * deltaTime;
		for (int i = 0; i < 3; ++i)
			Game::GetGame().Update();
	}
	else if (m_slowmode)
	{
		m_timer += 0.25f * deltaTime;
		if (m_frameTimer % 4 == 0)
			Game::GetGame().Update();
	}
	else
	{
		m_timer += deltaTime;
		Game::GetGame().Update();
	}
		

	
	m_repMark.SetAlpha(sin(8 * m_timer) * 0.5f + 0.5f);
}

void Activities::ReplayerActivity::OnEvent(const Basic::Input::Message & msg)
{
	if (msg.type == Basic::Input::Message::Type::KeyDown)
	{
		if (msg.key == Basic::Input::Key::Shoot)
			m_accmode = true;
		else if (msg.key == Basic::Input::Key::Slow)
			m_slowmode = true;
		else if (msg.key == Basic::Input::Key::Bomb)
			m_superacc = true;
		else if (msg.key == Basic::Input::Key::Esc)
		{
			m_accmode = false;
			m_slowmode = false;
			m_superacc = false;
			m_pause->BuildMenu(
				{
					Activities::GamePausedActivity::MenuItem::RestartRep,
					Activities::GamePausedActivity::MenuItem::ResumeRep,
					Activities::GamePausedActivity::MenuItem::Exit
				},
				true,
				Activities::GamePausedActivity::BGM::None,
				5
			);

			Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Call(m_pause);
		}
	}
	else if (msg.type == Basic::Input::Message::Type::KeyUp)
	{
		if (msg.key == Basic::Input::Key::Shoot)
			m_accmode = false;
		else if (msg.key == Basic::Input::Key::Slow)
			m_slowmode = false;
		else if (msg.key == Basic::Input::Key::Bomb)
			m_superacc = false;
	}
}

Activities::ReplayerActivity::Bridge::Bridge(ReplayerActivity * act) :
	m_act{ act }
{
}

void Activities::ReplayerActivity::Bridge::OnStageClear()
{
	m_act->loadStageAndMovie();
}

void Activities::ReplayerActivity::Bridge::OnPlayerAllDied()
{
	m_act->m_pause->BuildMenu(
		{
			Activities::GamePausedActivity::MenuItem::Exit,
			Activities::GamePausedActivity::MenuItem::RestartRep
		},
		false,
		Activities::GamePausedActivity::BGM::GameOver,
		13
	);

	Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Call(m_act->m_pause);
}

void Activities::ReplayerActivity::Bridge::FinishSpell(int,bool)
{
}