#pragma once
#include <string>
namespace SaveSystem
{

	//禁止在游戏逻辑中引用此类！！！
	class PlayerProfile
	{
	private:
		struct PODFileContent
		{
			wchar_t playerName[10];
			bool practiceEnabled[7] = { true,false,false,false,false,false,false };
			bool gameStartExtraEnabled = false;
			bool mirrorModeEnabled = false;
			bool mirrorEnabled = false;

			uint32_t spellGotTimes[10000];
			uint32_t spellTimes[10000];
		};

		PODFileContent m_podContent;

		PlayerProfile();

		void flush();
	public:
		void SetPlayerName(const wchar_t* name);
		std::wstring GetPlayerName() const;

		//stageNumber - 0为Stage1，1为Stage2，6为StageEx
		bool GetPracticeEnabled(int stageNumber) const;

		//stageName
		void EnablePractice(const std::string& stageName);

		//是否GameStart里可以使用Extra关卡
		bool GameStartExtraEnabled() const;

		//启动GameStart里的Extra关卡
		void EnableGameStartExtra();

		//获取符卡收率
		std::tuple<uint32_t, uint32_t> GetSpellGotNumber(int spellHash);

		//收率修改
		void SpellFinished(int spellHash,bool gotBonus);

		static PlayerProfile& Get();
	};
}