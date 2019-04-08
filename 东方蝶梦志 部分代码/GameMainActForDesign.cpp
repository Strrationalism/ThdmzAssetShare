#include "NuzPrecompiled.h"
#include "GameMainActForDesign.h"
#include "StageData.h"
#include "BossBgLayer.h"
#include "Bullet.h"
#include "Shaders.h"
#include "Haptic.h"
#include "ActivityEngine.h"
#include "CharReimu.h"
#include "CharMarisa.h"
#include "Log.h"
#include "GameRebuilder.h"
using namespace Activities;



Activities::GameMainActForDesign::GameMainActForDesign(bool replayMode)
{
	RebuildGame();
	m_replayMode = replayMode;

	if (m_replayMode)
		m_inputer = std::make_unique<Inputer::ReplayerInputer>();
	else
		m_inputer = std::make_unique<Inputer::RecoderInputer>();

	m_pauseActivity = std::make_shared<Activities::GamePausedActivity>();
}

Activities::GameMainActForDesign::~GameMainActForDesign()
{
	if (!m_replayMode) {
		std::ofstream out("../replay.rpy", std::ios::binary);
		m_gameInitData.WriteToStream(out);
		dynamic_cast<Inputer::RecoderInputer*>(m_inputer.get())->GetMovie().SaveToStream(out);
		out.close();
	}
}

void Activities::GameMainActForDesign::OnEntry()
{
	m_debugInfo.emplace("debug.rv");
	Char::LoadAndSetConfig();
	Game::GetGame().SetGameActBridge(&m_bridge);

	if (m_debugInfo->GetString("CHAR") == "MARISA")
		Game::GetGame().GetPlayer().SetChar(Char::Marisa::CharMarisa::Get());
	else if (m_debugInfo->GetString("CHAR") == "REIMU")
		Game::GetGame().GetPlayer().SetChar(Char::Reimu::CharReimu::Get());

	Game::GetGame().SetInputer(*m_inputer);
	
	if (m_replayMode) {
		auto& inp = *dynamic_cast<Inputer::ReplayerInputer*>(m_inputer.get());
		std::ifstream in("../replay.rpy", std::ios::binary);
		GameInitData initData;
		initData.ReadFromStream(in);
		Game::GetGame().LoadFromInitData(initData);
		inp.GetMovie().LoadFromStream(in);
		in.close();
		inp.Reset();
	}
	else {
		dynamic_cast<Inputer::RecoderInputer*>(m_inputer.get())->GetMovie().Clear();
	}
	m_stageData = LoadStage(m_debugInfo->GetString("STAGE"), (StageLevel)m_debugInfo->GetInt("LEVEL"));
	Game::GetGame().Reset();
	Game::GetGame().SetStageData(m_stageData);
	if (!m_replayMode) {
		Game::GetGame().WriteToInitData(m_gameInitData);
	}
	
	Game::GetGame().GetPlayer().SetPower(m_debugInfo->GetDouble("POWER"));

	const auto startFrame = m_debugInfo->GetInt("START_FRAME");

	if(startFrame > 10)
		Game::GetGame().GetPlayer().Invincible(m_debugInfo->GetInt("START_FRAME") + 800);
	for (int i = 0; i < startFrame; ++i) {
		if(i % 4000 == 0)
			Log("Preping:%d Frame %lf %%.", Game::GetGame().GetGameTime(),((double)Game::GetGame().GetGameTime() / m_debugInfo->GetInt("START_FRAME") * 100));
		Game::GetGame().Update();
		Basic::Sound::BgmPlayer::Update();

		if (startFrame - i >= 500) {
			Game::GetGame().GetItemMgr().Clear();
			Game::GetGame().GetEnemyBulletManager().ForceKillAll();
			Game::GetGame().GetParticleBottom().Clear();
			Game::GetGame().GetParticleTop().Clear();
		}
	}

	//if (startFrame) Basic::Sound::LowerSound::DebugSetBGMPosition(Game::GetGame().GetGameTime());

	
	//exit(0);
}

void Activities::GameMainActForDesign::OnDraw() const
{
	Game::GetGame().Draw();
}

void Activities::GameMainActForDesign::OnUpdate(float)
{
	
		Game::GetGame().Update();
		if (m_debug_3xSpeed) {
			for (int i = 0; i < m_debugInfo->GetInt("DEBUG_SPEED"); ++i) {
				Game::GetGame().Update();
				Basic::Sound::BgmPlayer::Update();
			}

		}
	
}

#include <cstdio>

void Activities::GameMainActForDesign::OnEvent(const Basic::Input::Message & m)
{
	
		if (m.type == Basic::Input::Message::Type::KeyDown) {
			switch (m.key) {
			case Basic::Input::Key::F1:
				m_debug_3xSpeed = true;
				break;
			case Basic::Input::Key::F2:
				if (Game::GetGame().GetScoreManager().GetLife() == 0)
					Game::GetGame().GetScoreManager().SetLife(3);
				EnemyBullet::Bullet::LoadScript();
				OnEntry();
				break;

			case Basic::Input::Key::F3:
				Log("---- Game Status ----");
				Log("Bullets:%d", (int)Game::GetGame().GetEnemyBulletManager().Size());
				Log("Enemies:%d", (int)Game::GetGame().GetEnemyManager().Size());
				Log("Particles:%d", (int)Game::GetGame().GetParticleTop().Size() + (int)Game::GetGame().GetParticleBottom().Size());
				Log("Timer:%d", (int)Game::GetGame().GetGameTime());
				{
					double x, y;
					Game::GetGame().GetPlayer().GetPos(x, y);
					Log("Player Pos:%f,%f", x, y);
				}

				break;
			case Basic::Input::Key::F4:
				Game::GetGame().GetPlayer().Invincible(9999999);
				break;
			case Basic::Input::Key::Esc:
				//Pause();
				//SDL_PauseAudio(1);
				//Basic::Sound::BgmPlayer::SetPaused(true);
				Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Call(m_pauseActivity);
				break;
			}
		}
		else if (m.type == Basic::Input::Message::Type::KeyUp) {
			switch (m.key) {
			case Basic::Input::Key::F1:
				m_debug_3xSpeed = false;
				//Basic::Sound::LowerSound::DebugSetBGMPosition(Game::GetGame().GetGameTime());
				break;
			}
		}

		m_inputer->PushMessage(m);
	

}

void Activities::GameMainActForDesign::BridgeForDesign::OnPlayerAllDied()
{
	//Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Exit();
	Game::GetGame().GetScoreManager().Reset();
	Game::GetGame().GetPlayer().Birth();

}

void Activities::GameMainActForDesign::BridgeForDesign::OnStageClear()
{
	//m_act->Pause();
}
