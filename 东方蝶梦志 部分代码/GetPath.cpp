#include "Precompiled.h"
#undef GetString
#include "GetPath.h"
#include <Shlobj.h>

using namespace SaveSystem;

std::filesystem::path SaveSystem::GetPath()
{
	wchar_t* appDataPath = nullptr;
	const auto result = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &appDataPath);

	if (!SUCCEEDED(result))
		Crash(L"未能获取存档目录。");
		

	//const auto appDataPath = std::filesystem::current_path().wstring();
	const std::filesystem::path strPath = std::wstring(appDataPath) + L"\\Strrationalism";
	
	if (!std::filesystem::exists(strPath))
		std::filesystem::create_directory(strPath);

	const std::filesystem::path dmzPath = std::wstring(appDataPath) + L"\\Strrationalism\\Thdmz";
	if (!std::filesystem::exists(dmzPath))
		std::filesystem::create_directory(dmzPath);

	CoTaskMemFree(appDataPath);

	return dmzPath;
}

std::filesystem::path SaveSystem::GetReplayPath()
{
	const auto ret = std::filesystem::path{ GetPath().wstring() + L"\\Replay" };
	if (!std::filesystem::exists(ret))
		std::filesystem::create_directory(ret);
	return ret;
}


