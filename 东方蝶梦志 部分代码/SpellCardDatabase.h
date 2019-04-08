#pragma once
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include "StageData.h"

class SpellCardDatabase
{
public:
	struct SpellCard
	{
		int title[4];
		uint16_t hash[4];
	};

	enum class Charater
	{
		Reimu = 0,
		Marisa = 1
	};

private:
	SpellCardDatabase();

	//[2]为角色，string为关卡名
	std::map<std::string, std::vector<SpellCard>> m_spellCards[2];
public:
	static SpellCardDatabase& Get();

	static uint16_t GetHash(int title, StageLevel level, Charater ch);

	const std::vector<std::pair<int, int>> GetSpellTitleHash(Charater ch, StageLevel lv, bool isExtra);
};
