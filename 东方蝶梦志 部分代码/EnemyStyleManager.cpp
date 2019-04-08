#include "Precompiled.h"
#include "EnemyStyleManager.h"
#include "CSVReader.h"

std::array< Enemy::EnemyStyleManager::EnemyStyle, 25>  Enemy::EnemyStyleManager::m_styles;

void Enemy::EnemyStyleManager::Init()
{
	Basic::CSVReader csv;
	csv.Load("EnemyStyles.csv");

	int styleID = 0;
	while (!csv.IsLastLine()) {
		m_styles.at(styleID).textureName = csv.PopString();

		m_styles.at(styleID).gameFrame_animationFrame = csv.PopInt();
		m_styles.at(styleID).collRadius = (uint8_t)csv.PopDouble();

		m_styles.at(styleID).particleWhenKilled.num = csv.PopInt();
		m_styles.at(styleID).particleWhenKilled.speed = csv.PopDouble();
		m_styles.at(styleID).particleWhenKilled.r = csv.PopDouble();
		m_styles.at(styleID).particleWhenKilled.g = csv.PopDouble();
		m_styles.at(styleID).particleWhenKilled.b = csv.PopDouble();

		m_styles.at(styleID).xFilpEnabled = csv.PopInt() > 0;

		csv.NextLine();
		styleID++;
	}
}
