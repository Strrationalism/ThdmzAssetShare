#pragma once
#include <fstream>
#include <cstdint>
#include "StageData.h"
#include "Game.h"
#include "Movie.h"

namespace MovieData
{
	constexpr int32_t MOVIE_VERSION = 0;
	struct Head
	{
		enum Charater : int32_t
		{
			Reimu = 0,
			Marisa = 1
		};

		int32_t version = MOVIE_VERSION;	//录像版本号，调试版常为0，发布时为1
		Charater charater;	//角色名
		int32_t level;	//等级（从StageLevel强转而来）
		bool isExtra = false;	//是否为Extra模式
		wchar_t lastStage[8];	//玩的最后一面的关卡名称
		wchar_t playerName[10];	//机签
		bool mirrored = false;
		uint64_t score;		//总分数
	};

	struct MovieData
	{
		Head head;
		std::map<std::string, std::pair<GameInitData, Inputer::Movie>> stages;
	};

	void SaveMovie(const MovieData& movieData, std::ofstream& out);
	void ReadMovie(MovieData& movieData, std::ifstream& in);
};