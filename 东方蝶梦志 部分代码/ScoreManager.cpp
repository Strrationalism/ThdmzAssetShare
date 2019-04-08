#include "Precompiled.h"
#include "ScoreManager.h"
#include "DreamWaterWave.h"
#include "Game.h"

ScoreManager::ScoreManager()
{
}

void ScoreManager::GrazeAdd(unsigned graze)
{
	m_graze += graze;
	Game::GetGame().GetUI().GetGrazeDisplay().SetGraze(m_graze);

	ScoreAdd(100 * graze);
}

void ScoreManager::SetGraze(unsigned graze)
{
	m_graze = 0;
	GrazeAdd(graze);
}

void ScoreManager::ScoreAdd(unsigned long long score)
{
	m_score += score;
	Game::GetGame().GetUI().GetScoreDisplay().SetScore(m_score);
}


void ScoreManager::AddLifeKagera()
{
	m_lifeKagera++;
	Game::GetGame().GetUI().GetNonFullHeart().SetDataUseDefaultHeight(m_lifeKagera * 2);
	if (m_lifeKagera == 3) {
		m_lifeKagera = 0;
		Game::GetGame().GetTaskList().AddTask([this]() {
			Game::GetGame().GetUI().GetNonFullHeart().SetDataUseDefaultHeight(0);
			Game::GetGame().GetUI().GetFullHearts().SetData(m_life - 1);

			double x, y;
			Game::GetGame().GetPlayer().GetPos(x, y);
			Basic::Sound::SeManager::Play("extend",x,40);
		}, 40);
		m_life++;
	}
}

void ScoreManager::KillALife()
{
	m_life--;
	if (m_life == 0) {
		Game::GetGame().GetTaskList().AddTask([]() {
			Game::GetGame().GetGameActBridge()->OnPlayerAllDied();
		}, 60);
	}
	else {
		Game::GetGame().GetTaskList().AddTask([]() {
			Game::GetGame().GetPlayer().Birth();
		}, 60);
		Game::GetGame().GetUI().GetFullHearts().SetData(m_life - 1);
	}
}


#include "FlashObject.h"
void ScoreManager::AddDreamPoint(double point)
{
	if (point < 0)
		point *= 1.5;
	if (!m_dreamEnabled) return;
	if (Game::GetGame().GetPlayer().GetSheild().has_value()) return;
	if (Game::GetGame().GetPlayer().Bombing()) return;
	if (Game::GetGame().GetPlayer().GetState() != Player::PlayerState::NORMAL) return;
	m_dreamSystem += point;
	

	bool bonus = false;
	bool isLeft = false;
	if (m_dreamSystem > 1) {
		bonus = true;
		m_dreamSystem = 0;
		isLeft = false;
	}
	else if (m_dreamSystem < -1) {
		bonus = true;
		m_dreamSystem = 0;
		isLeft = true;
	}

	if (bonus) {
		double x, y;
		Game::GetGame().GetPlayer().GetPos(x, y);
		
		Basic::Sound::SeManager::Play("bonus2",x);

		Game::GetGame().GetPlayer().GetSheild().emplace(isLeft);

		
	}

	Game::GetGame().GetUI().GetDreamSystem().SetData(m_dreamSystem);
}



void ScoreManager::Reset() {	//不随着Game一起Reset
	m_graze = 0;
	m_score = 0;
	m_life = 3;
	m_lifeKagera = 0;
	m_dreamSystem = 0;


	Game::GetGame().GetUI().GetFullHearts().SetData(m_life - 1);
	Game::GetGame().GetUI().GetGrazeDisplay().SetGraze(0);
	Game::GetGame().GetUI().GetNonFullHeart().SetDataUseDefaultHeight(0);
	Game::GetGame().GetUI().GetScoreDisplay().SetScore(0);
	Game::GetGame().GetUI().GetDreamSystem().SetData(m_dreamSystem);
}

void ScoreManager::ResetWithGameStage() {

	m_dreamEnabled = true;

	Game::GetGame().GetUI().GetFullHearts().SetData(m_life - 1);
	Game::GetGame().GetUI().GetNonFullHeart().SetDataUseDefaultHeight(m_lifeKagera);
}

void ScoreManager::SetDream(double dream) {
	Game::GetGame().GetUI().GetDreamSystem().SetData(m_dreamSystem = dream);
}

void ScoreManager::SetLife(int life) {
	m_life = life;
	Game::GetGame().GetUI().GetFullHearts().SetData(m_life - 1);
}

void ScoreManager::SetLifeKagera(int kagera) {
	m_lifeKagera = kagera;
	Game::GetGame().GetUI().GetNonFullHeart().SetDataUseDefaultHeight(m_lifeKagera);
}
