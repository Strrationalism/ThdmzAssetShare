#include "Precompiled.h"
#define SDL_MAIN_HANDLED

#pragma warning (disable : 4091)
#pragma warning (disable : 4305)
#pragma warning (disable : 4307)
#pragma warning (disable : 4309)

#include "Log.h"
#include "Window.h"
#include "Globals.h"
#include "ActivityEngine.h"
#include "Init.h"
#include "SSLogo.h" 
#include "Game.h"
#include <DbgHelp.h>
#include "LockContryActivity.h"
#include "GameActivity.h"
#include "GameMainActForDesign.h"
#include "TitleMainMenu.h"
#include "SpellCardDatabase.h"
#include "CharMarisa.h"
#include "CharReimu.h"
#include "ReplayerActivity.h"
#include "WrappedSaveRepActivity.h"
#include "GameRebuilder.h"
#include "StaffActivity.h"
#include "ImageHintActivity.h"
#include <shellapi.h>
/****************************************大
*****************************************妖
*****************************************精
******************************************！*/

using namespace std;

//锁区检测
static bool IsLockedContry()
{
	return false;
	LANGID langIDs[] = 
	{
		GetSystemDefaultLangID(),
		GetSystemDefaultLCID(),
		GetSystemDefaultUILanguage(),
		GetUserDefaultLangID(),
		GetUserDefaultLCID(),
		GetUserDefaultUILanguage()
	};

	set<LANGID> supported = 
	{
		0x0804,		//中华人民共和国
		0x0c04		//中华人民共和国 香港地区
	};

	for (auto i : langIDs)
		if (supported.count(i) == 0)
			return true;

	return false;
}

//Error Handler
//生产DUMP文件
int GenerateMiniDump(HANDLE hFile, PEXCEPTION_POINTERS pExceptionPointers, PCHAR pwAppName)
{
	BOOL bOwnDumpFile = FALSE;
	HANDLE hDumpFile = hFile;
	MINIDUMP_EXCEPTION_INFORMATION ExpParam;

	typedef BOOL(WINAPI * MiniDumpWriteDumpT)(
		HANDLE,
		DWORD,
		HANDLE,
		MINIDUMP_TYPE,
		PMINIDUMP_EXCEPTION_INFORMATION,
		PMINIDUMP_USER_STREAM_INFORMATION,
		PMINIDUMP_CALLBACK_INFORMATION
		);

	MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
	HMODULE hDbgHelp = LoadLibrary("DbgHelp.dll");
	if (hDbgHelp)
		pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

	if (pfnMiniDumpWriteDump)
	{
		if (hDumpFile == NULL || hDumpFile == INVALID_HANDLE_VALUE)
		{
			TCHAR szPath[MAX_PATH] = { 0 };
			TCHAR szFileName[MAX_PATH] = { 0 };
			TCHAR* szAppName = pwAppName;
			TCHAR* szVersion = __DATE__;
			TCHAR dwBufferSize = MAX_PATH;
			SYSTEMTIME stLocalTime;

			GetLocalTime(&stLocalTime);
			GetTempPath(dwBufferSize, szPath);

			sprintf_s(szFileName, "%s%s", szPath, szAppName);
			CreateDirectory(szFileName, NULL);

			sprintf_s(szFileName, "dump.dmp");
			hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

			bOwnDumpFile = TRUE;
			OutputDebugString(szFileName);
		}

		if (hDumpFile != INVALID_HANDLE_VALUE)
		{
			ExpParam.ThreadId = GetCurrentThreadId();
			ExpParam.ExceptionPointers = pExceptionPointers;
			ExpParam.ClientPointers = FALSE;

			pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
				hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &ExpParam : NULL), NULL, NULL);

			if (bOwnDumpFile)
				CloseHandle(hDumpFile);
		}
	}

	if (hDbgHelp != NULL)
		FreeLibrary(hDbgHelp);

	return EXCEPTION_EXECUTE_HANDLER;
}

LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
	if (IsDebuggerPresent())
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}

	return GenerateMiniDump(NULL, lpExceptionInfo, "test");
}

class SDL final {	//SDL生命周期管理类
public:
	SDL() {
		if (SDL_Init(		//初始化SDL
			SDL_INIT_AUDIO |
			SDL_INIT_EVENTS |
			SDL_INIT_JOYSTICK |
			SDL_INIT_HAPTIC//||
			//SDL_INIT_VIDEO
		)) Crash(wstring(L"SDL初始化失败：") + Str2WStr(SDL_GetError()));


	}
	~SDL() {
		
		SDL_Quit();	//关闭SDL
	}
};

static void CheckGPU() {	//兼容检查
	GLint major = 0;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	if (major < 2)
		MessageBoxW(nullptr, L"警告：OpenGL版本可能过低。", L"警告", 0);

	Log("OpenGL Version:%d", major);
	Log("OpenGL Vendor:%s", glGetString(GL_VENDOR));
	
#ifdef _DEBUG
	Log("Debug Mode");
#endif

	set<string> allExtensions;
	string exs((char*)glGetString(GL_EXTENSIONS));
	while (!exs.empty()) {
		auto pos = exs.find(' ');
		if (pos == string::npos) {
			allExtensions.insert(exs);
			exs.clear();
		}
		else {
			allExtensions.insert(exs.substr(0, pos));
			exs = exs.substr(pos + 1, exs.size() - pos - 1);
		}
	}

	set<string> extensionNeedSupposed;

	//要求DXT纹理支持
	extensionNeedSupposed.insert("GL_EXT_texture_compression_s3tc");

	//检查扩展是否支持
	for (auto& p : extensionNeedSupposed) {
		if (allExtensions.count(p) == 0)
			Crash(L"未找到" + Str2WStr(p) + L"扩展。");
	}
}


#include "GetPath.h"
#include "BaseBackground.h"
static bool __dbg__replayMode = false;

//如果找不到Config.sav则打开Config.exe
static void IfConfigNotExistOpenConfig()
{
	const auto exist = std::filesystem::exists(std::filesystem::path(SaveSystem::GetPath().string() + "\\config.sav"));
	if (!exist)
	{
		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		ShExecInfo.lpFile = "Config.exe";
		ShExecInfo.lpParameters = "";
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_SHOW;
		ShExecInfo.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
	}
}

static bool preloadBGM = true;

static void LoadConfigAndInitWindow() {
	IfConfigNotExistOpenConfig();
	//读取基础设置并创建窗口
	ifstream cfg(SaveSystem::GetPath().string() + "\\config.sav", ios::binary);
	uint8_t videoMode = 4;
	uint8_t skipFrameMode = 2, textureFliter = 0;
	uint8_t msaa = 2;
	//bool particle2DEnabled = true;
	bool fullScreen = false;
	bool vsync = true;

	bool usingBeautifulBG = true;
	if (cfg.good()) {
		cfg.read((char*)&videoMode, 1);
		cfg.read((char*)&fullScreen, 1);
		cfg.read((char*)&skipFrameMode, 1);
		cfg.read((char*)&textureFliter, 1);
		cfg.read((char*)&vsync, 1);
		cfg.read((char*)&msaa, 1);

		cfg.read((char*)&usingBeautifulBG, 1);
		cfg.read((char*)&preloadBGM, 1);
		Background::BaseBackground::SetUsingBeautifulBG(usingBeautifulBG);


		cfg.close();
	}

	const int video[][2] = {
		{ 2560,1440 },
		{ 1920,1080 },
		{ 1820,1024 },
		{ 1440,810 },
		{ 1280,720 },
		{ 1024,576 },
		{ 800,450 }
	};

	const int frameMode[] = {
		0,
		120,
		60,
		30,
		20
	};

	uint32_t flags = 0;
	uint32_t width = video[videoMode][0];
	uint32_t height = video[videoMode][1];

	if (fullScreen) {
		flags = SDL_WINDOW_FULLSCREEN;
		width = 1280;
		height = 720;
		if (0 == videoMode) {
			flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
			width = -1;
			height = -1;
		}
	}
	Basic::Video::Window::GetWindow().InitWindow(width, height, flags, vsync, msaa);
	SDL_ShowCursor(!fullScreen);

	if (!vsync)
		Basic::ActivityEngine::ActivityEngine::GetActivityEngine().SetFPS(frameMode[skipFrameMode]);

	CheckGPU();	//检查显卡是否兼容

	GLint max, need = usingBeautifulBG ? 4096 : 2048;	//最大纹理大小不小于need
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
	if (max < need) Crash(L"需要尝试关掉设置中的“使用更华丽的游戏效果”？你的显卡只能支持：" + to_wstring(max) + L"长宽的纹理，但是目前的设置需要" + to_wstring(need) + L"以上的。");

	if (textureFliter) {
		Basic::Video::LowerRenderer::GLTexture::SetFliter(GL_NEAREST);
		glHint(GL_FOG_HINT, GL_FASTEST);
	}
	else {
		Basic::Video::LowerRenderer::GLTexture::SetFliter(GL_LINEAR);
		glEnable(GL_POLYGON_SMOOTH);
		glEnable(GL_POINT_SMOOTH);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
		glHint(GL_FOG_HINT, GL_NICEST);
	}
}

static void FindUpdate()
{
	const auto newerVer = std::filesystem::path(SaveSystem::GetPath().string() + "/Update/Thdmz.exe").lexically_normal();
	if (std::filesystem::exists(newerVer))
	{
		const auto cd = std::filesystem::current_path().string();
		const auto wrappedCd = '\"' + cd + '\"';
		
		const auto instance = ShellExecute(
			nullptr,
			"open",
			newerVer.string().c_str(),
			wrappedCd.c_str(),
			cd.c_str(),
			SW_SHOWNORMAL);

		if(instance)
			exit(0);
	}
}

void RunGame()
{
	FindUpdate();

	SpellCardDatabase::Get();

	SetUnhandledExceptionFilter(ExceptionFilter);
	SDL_SetMainReady();
	SDL sdl;
	SDL_DisableScreenSaver();

	LoadConfigAndInitWindow();
	SDL_RaiseWindow(Basic::Video::Window::GetWindow());

	Basic::Input::LoadJoystickButtonSetting();

	Basic::Sound::LowerSound::InitSound();
	Basic::Sound::BGMData::Init();

	std::future<void> bgmLoaded;
	std::function<bool()> waitForBGMCache = [] { return true; };
	if (preloadBGM)
	{
		bgmLoaded = Basic::Sound::LowerSound::CacheBGM();
		waitForBGMCache = [&bgmLoaded] {
			return bgmLoaded.valid();
		};
	}

	{
		extern void __LogRenderer__Init();
		__LogRenderer__Init();
		Log("Build:%s %s", (__DATE__), (__TIME__));




		Basic::Init::InitOpenGL();


		std::any textureCache;

		auto logoAct = std::make_shared<Activities::Title::SSLogo>(textureCache, [&bgmLoaded,&waitForBGMCache] {
//#define SUPER_TEST
#ifndef SUPER_TEST


			Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Goto(make_shared<Activities::ImageHintActivity>("Staff1", 1,0.7f,&waitForBGMCache));
			/*std::ifstream(SaveSystem::GetPath().string() + "/Replay/dev.rep", std::ios::binary);
			auto p = make_unique<MovieData::MovieData>();
			Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Goto(
				make_shared<Activities::WrappedSaveRepActivity>(move(p))
			);*/
#else
			//Super Test!

			for (int i = 0;i < 2;++i)
			{
				Char::BasicChar* chl[] = {
					(Char::BasicChar*)&Char::Reimu::CharReimu::Get(),
					(Char::BasicChar*)&Char::Marisa::CharMarisa::Get()
				};

				auto& ch = *chl[i];

				using que = std::queue<std::string>;
				std::queue<std::string> stages;
				stages.push("Stage1");
				stages.push("Stage2");
				stages.push("Stage3");
				stages.push("Stage4");
				stages.push("Stage5");
				stages.push("Stage6");
				stages.push("StageEx");
				stages.push("End");

				Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Run(make_shared<Activities::GameActivity>(que(stages),StageLevel::E,ch));
				Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Run(make_shared<Activities::GameActivity>(que(stages),StageLevel::N,ch));
				Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Run(make_shared<Activities::GameActivity>(que(stages),StageLevel::H,ch));
				Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Run(make_shared<Activities::GameActivity>(que(stages),StageLevel::L,ch));
			}
			Crash(L"Test Successed!");
#endif
		});


		Basic::ActivityEngine::ActivityEngine::GetActivityEngine().
			Run(logoAct);

		ClearGame();

		extern void __LogRenderer__Quit();
		__LogRenderer__Quit();

		Basic::Sound::LowerSound::QuitSound();
		Basic::Video::LowerRenderer::ImageCacheManager::EarlyDelete();

		Basic::Sound::SeManager::Quit();

		SDL_EnableScreenSaver();
	}
}

#pragma warning(suppress: 28251)
int WINAPI WinMain(
	HINSTANCE hInstance,       //程序当前实例的句柄，以后随时可以用GetModuleHandle(0)来获得  
	HINSTANCE hPrevInstance,   //这个参数在Win32环境下总是0，已经废弃不用了  
	char * lpCmdLine,          //指向以/0结尾的命令行，不包括EXE本身的文件名，  
							   //以后随时可以用GetCommandLine()来获取完整的命令行  
	int nCmdShow               //指明应该以什么方式显示主窗口  
)
{
	SDL_version version;
	SDL_GetVersion(&version);
	Log("SDL Version:%d,%d,%d", static_cast<int>(version.major), static_cast<int>(version.minor), static_cast<int>(version.patch));

	
#ifdef _DEBUG
	RunGame();
#else
	try 
	{
		RunGame();
	}
	catch (std::exception e)
	{
		Crash(Str2WStr(e.what()));
	}
	catch (...)
	{
		Crash(L"未知错误");
	}
#endif
	

	return 0;
}

#ifdef _DEBUG
int main()
{
	return WinMain(NULL, nullptr, nullptr, 0);
}
#endif