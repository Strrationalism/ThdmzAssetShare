#include "Precompiled.h"
#include "PlayerProfile.h"
#include "GetPath.h"

using namespace SaveSystem;

const static auto savePath = GetPath().string() + "\\profile.sav";

SaveSystem::PlayerProfile::PlayerProfile()
{
	std::ifstream in(savePath, std::ios::binary);
	if (in.good())
	{
		in.read((char*)&m_podContent, sizeof(m_podContent));
		in.close();
	}
	else
	{
		wcscpy_s(m_podContent.playerName, L"");
	}
}

void SaveSystem::PlayerProfile::flush()
{
	std::ofstream out(savePath, std::ios::binary);
	if (!out.good())
		Crash(L"´æµµÐ´³öÊ§°Ü£¡");

	out.write((char*)&m_podContent, sizeof(m_podContent));
	out.close();
}

void SaveSystem::PlayerProfile::SetPlayerName(const wchar_t * name)
{
	wcscpy_s(m_podContent.playerName, name);
	flush();
}

std::wstring SaveSystem::PlayerProfile::GetPlayerName() const
{
	return m_podContent.playerName;
}

bool SaveSystem::PlayerProfile::GetPracticeEnabled(int stageNumber) const
{
	return m_podContent.practiceEnabled[stageNumber];
}

void SaveSystem::PlayerProfile::EnablePractice(const std::string& stageName)
{
	std::map<std::string, int> v;
	v["Stage1"] = 0;
	v["Stage2"] = 1;
	v["Stage3"] = 2;
	v["Stage4"] = 3;
	v["Stage5"] = 4;
	v["Stage6"] = 5;
	v["StageEx"] = 6;
	m_podContent.practiceEnabled[v.at(stageName)] = true;
	flush();
}

bool SaveSystem::PlayerProfile::GameStartExtraEnabled() const
{
	return m_podContent.gameStartExtraEnabled;
}

void SaveSystem::PlayerProfile::EnableGameStartExtra()
{
	m_podContent.gameStartExtraEnabled = true;
	flush();
}

std::tuple<uint32_t, uint32_t> SaveSystem::PlayerProfile::GetSpellGotNumber(int spellHash)
{
	return std::make_tuple(m_podContent.spellGotTimes[spellHash], m_podContent.spellTimes[spellHash]);
}

void SaveSystem::PlayerProfile::SpellFinished(int spellHash,bool gotBonus)
{
	if (gotBonus)
		m_podContent.spellGotTimes[spellHash]++;
	m_podContent.spellTimes[spellHash]++;
	flush();
}

PlayerProfile & SaveSystem::PlayerProfile::Get()
{
	static PlayerProfile pf;
	return pf;
}
