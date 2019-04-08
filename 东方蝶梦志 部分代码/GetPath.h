#pragma once
#include <string>
#include <filesystem>

namespace SaveSystem
{
	std::filesystem::path GetPath();
	std::filesystem::path GetReplayPath();
}