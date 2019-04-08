#pragma once
#include "StageData.h"
class ScoreManager
{
private:
	unsigned m_graze = 0;
	unsigned long long m_score = 0;

	double m_dreamSystem = 0;

	int m_life = 3;
	int m_lifeKagera = 0;

	bool m_dreamEnabled = true;
public:
	ScoreManager();
	void GrazeAdd(unsigned graze);
	inline auto GetGraze() const
	{
		return m_graze;
	}
	void SetGraze(unsigned graze);
	void ScoreAdd(unsigned long long score);
	inline void SetScore(unsigned long long score)
	{
		m_score = 0;
		ScoreAdd(score);
	}
	void AddLifeKagera();	//‘ˆº”…˙√¸ÀÈ∆¨
	void KillALife();	//µÙ≤–
	inline static double GetLevelScoreMul(StageLevel v) {
		switch (v) {
		case StageLevel::E:
			return 0.5;
		case StageLevel::N:
			return 1.0;
		case StageLevel::H:
			return 1.5;
		case StageLevel::L:
			return 2.0;
		default:
			return -1;
		}
	}


	void AddDreamPoint(double point);

	void Reset();

	void ResetWithGameStage();

	inline auto GetScore() {
		return m_score;
	}

	inline auto GetDream() {
		return m_dreamSystem;
	}
	void SetDream(double dream);

	void SetLife(int life);

	inline auto GetLife() {
		return m_life;
	}

	void SetLifeKagera(int kagera);

	inline auto GetLifeKagera() {
		return m_lifeKagera;
	}
};
