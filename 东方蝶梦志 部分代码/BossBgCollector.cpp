#include "Precompiled.h"
#include "BossBgCollector.h"
#include "CSVReader.h"
#include "BossBgLayer.h"
#include "Game.h"

Boss::BossBgCollector::~BossBgCollector()
{
	if (!m_active) {
		for (auto p : m_gos)
			delete p;
	}
}

void Boss::BossBgCollector::LoadBossBG(const std::string & layerDefinedCSV)
{
	Basic::CSVReader csv;
	csv.Load(layerDefinedCSV);

	m_gos.clear();
	while (!csv.IsLastLine()) {
		auto dmslUnit = csv.PopInt();
		auto tex = csv.PopString();
		m_gos.push_back(new BossBgLayer(dmslUnit, tex, csv.PopInt()));
		csv.NextLine();
	}
}

void Boss::BossBgCollector::Active()
{
	for (auto p = m_gos.rbegin(); p != m_gos.rend(); ++p) {
		Game::GetGame().GetMiscManagerBottom().Insert(*p);
	}
	m_active = true;
}

void Boss::BossBgCollector::Show()
{
	for (auto& p : m_gos)
		((BossBgLayer*)p)->Show();

	Game::GetGame().GetTaskList().AddTask([]() {
		Game::GetGame().SetStageBackgroundEnabled(false);
	}, 40);
}

void Boss::BossBgCollector::Hide()
{
	Game::GetGame().SetStageBackgroundEnabled(true);
	for (auto& p : m_gos)
		((BossBgLayer*)p)->Hide();
}

void Boss::BossBgCollector::Kill()
{
	for (auto& p : m_gos)
		((BossBgLayer*)p)->Kill();
}