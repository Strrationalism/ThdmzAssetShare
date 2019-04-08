#include "Precompiled.h"
#include "SpellCardDatabase.h"
#include "SnowRVReader.h"
#include "CSVReader.h"
#include <cassert>
#include <fstream>
#include "DMSLUtils.h"
#include "UnicodeSite.h"

#ifdef _DEBUG
//#define PRINT_SPELLCARD_TO_FILE
#endif

#ifdef PRINT_SPELLCARD_TO_FILE
std::string WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

	if (nLen <= 0) return std::string("");

	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;

	std::string strTemp(pszDst);
	delete[] pszDst;

	return strTemp;
}

std::string ws2s(const std::wstring& inputws) { return WChar2Ansi(inputws.c_str()); }
#endif

static std::string applyChar(std::string str,const char* charName)
{
	while (1)
	{
		auto p = str.find('&');
		if (p == std::string::npos) break;
		auto before = str.substr(0, p);
		auto after = str.substr(p + 1, str.length() - p - 1);

		str = before + charName + after;
	}
	return str;
}

SpellCardDatabase::SpellCardDatabase()
{

#ifdef _DEBUG
	std::set<uint16_t> hash;
#endif

	const char* stages[] =
	{
		"Stage1",
		"Stage2",
		"Stage3",
		"Stage4",
		"Stage5",
		"Stage6",
		"StageEx"
	};

	const char* charName[] =
	{
		"Reimu",
		"Marisa"
	};

	for (auto& charater : { 0,1 })
	{
		for (auto& stage : stages)
		{

			Basic::SnowRVReader stageManifest("StageData/" + std::string(stage) + "/Main.rv");
			Basic::CSVReader stageTasklist;
			stageTasklist.Load("StageData/" + std::string(stage) + "/" + stageManifest.GetString("TASKLIST"));

			auto& spellcards = (m_spellCards[charater])[std::string(stage)];

			while (!stageTasklist.IsLastLine())
			{
				stageTasklist.PopString();
				const auto taskAction = stageTasklist.PopString();
				if (taskAction == "CreateBoss" || taskAction == "CreateBossWB")
				{
					stageTasklist.PopString();
					const auto spellCardList = applyChar(stageTasklist.PopString(),charName[charater]);

					Basic::CSVReader bossSkills;
					bossSkills.Load(spellCardList);

					while (!bossSkills.IsLastLine())
					{
						bossSkills.PopString();	//A
						bossSkills.PopString();	//B
						bossSkills.PopString();	//C
						bossSkills.PopString();	//D
						bossSkills.PopString();	//E
						const auto spellName = bossSkills.PopString();	//F
						const bool isSpellCard = bossSkills.PopInt() > 0;
						if (isSpellCard)
						{
							std::vector<std::string> splitedName;
							Dmsl::Utils::Split(spellName, splitedName, ':');

							SpellCard sc;
							for (int i = 0; i < 4; ++i)
							{
								sc.title[i] = Dmsl::Utils::ToInt(splitedName[i]);
								sc.hash[i] = GetHash(sc.title[i], (StageLevel)i, (Charater)charater);

#ifdef _DEBUG
								if (hash.count(sc.hash[i]) > 0)
									Crash(L"·û¿¨Hash³åÍ»£¡");
								else
									hash.insert(sc.hash[i]);
#endif // _DEBUG

							}
							spellcards.push_back(sc);
						}
						bossSkills.NextLine();
					}

				}
				stageTasklist.NextLine();
			}
		}
	}

#ifdef PRINT_SPELLCARD_TO_FILE
	std::ofstream wo("SpellCardNames.txt");

	for (auto stage : stages)
	{
		wo << stage << std::endl;
		
		for (auto ch : { 0,1 })
		{
			wo << "\t" << charName[ch] << std::endl;

			for (auto& sc : m_spellCards[ch][stage])
			{
				wo << "\t\t" << "·û¿¨£º" << std::endl;

				const char* lvs[] = 
				{
					"Easy\t",
					"Normal\t",
					"Hard\t",
					"Lunatic\t"
				};
				for (int i = 0; i < 4 ; ++i)
				{
					auto name = Basic::UnicodeText(sc.title[i]);
					if (name.size() < 9)
						name.append(L"\t\t");
					else if (name.size() < 14)
						name.append(L"\t");
					wo << "\t\t\t" << lvs[i] << ws2s(name) << "\t" << sc.title[i] <<" "<< sc.hash[i] << "\t" << std::endl;
				}
			}
		}
	}

	wo.close();
#endif
}

SpellCardDatabase & SpellCardDatabase::Get()
{
	static SpellCardDatabase db;
	return db;
}

uint16_t SpellCardDatabase::GetHash(int title, StageLevel level, Charater ch)
{
	int base = int(ch) * 4 + int(level);
	return uint16_t(title + base*1000);
}

const std::vector<std::pair<int, int>> SpellCardDatabase::GetSpellTitleHash(Charater ch, StageLevel lv, bool isExtra)
{
	const auto& spells = m_spellCards[(int)ch];
	const std::vector<std::string> stages = 
		isExtra ?
		std::vector<std::string>{
			"StageEx"
		}
		:
		std::vector<std::string>{
			"Stage1",
			"Stage2",
			"Stage3",
			"Stage4",
			"Stage5",
			"Stage6"
		};

	if (isExtra) lv = StageLevel::L;

	std::vector<std::pair<int, int>> ret;
	for (const auto& stage : stages)
	{
		auto& scs = spells.at(stage);
		for (const auto& sc : scs)
		{
			ret.push_back(std::make_pair(sc.title[(int)lv], sc.hash[(int)lv]));
		}
	}


	return ret;
}
