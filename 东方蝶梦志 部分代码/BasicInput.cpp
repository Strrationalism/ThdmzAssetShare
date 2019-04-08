#include "NuzPrecompiled.h"
#include "BasicInput.h"
#include "SnowRVReader.h"
#include "Globals.h"
#include "Window.h"
#include <cassert>

#define DIRECTINPUT_VERSION 0x0800
//DInput
#include <dinput.h>
#include <dinputd.h>

#include <wrl/client.h>
#include "GetPath.h"

using namespace Basic::Input;

static std::array<Basic::Input::Key,256> config;
static bool inputEnabled = true;

using Microsoft::WRL::ComPtr;

static ComPtr<IDirectInputA> lpDirectInput = nullptr;
static ComPtr<IDirectInputDeviceA> lpKeyBoard = nullptr;

void __JoystickClearKeyConfig()
{
	std::fill(config.begin(), config.end(), Basic::Input::Key::Error);
}

void __JoystickSetKey(int button, Basic::Input::Key key)
{
	config.at(button) = key;
}

/*
static int eventFliter(void*,SDL_Event* e) {
	if (e->type == SDL_QUIT ||
		e->type == SDL_KEYDOWN ||
		e->type == SDL_KEYUP ||
		e->type == SDL_JOYAXISMOTION ||
		e->type == SDL_JOYBUTTONDOWN ||
		e->type == SDL_JOYBUTTONUP ||
		e->type == SDL_JOYDEVICEADDED ||
		e->type == SDL_JOYDEVICEREMOVED ||
		e->type == SDL_WINDOWEVENT
		)
		return 1;
	else
		return 0;
}
*/

static char keyboardBuf[256];
void Basic::Input::Init()
{
	memset(keyboardBuf, 0, sizeof(keyboardBuf));
	SDL_SysWMinfo info;
	SDL_VERSION(&info.version);
	//SDL_SetEventFilter(eventFliter, nullptr);
	auto pWnd = static_cast<SDL_Window*>(Basic::Video::Window::GetWindow());
	auto flag = SDL_GetWindowWMInfo(pWnd,&info);
	if (!flag) Crash(L"输入系统：无法取得pWnd，故障原因:" + Str2WStr(SDL_GetError()));
	if (info.subsystem != SDL_SYSWM_WINDOWS) Crash(L"未能取得正确的操作系统代码，故障原因：" + Str2WStr(SDL_GetError()));
	auto hWnd = info.info.win.window;

	HRESULT hr = DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(lpDirectInput.GetAddressOf()), nullptr);
	if (FAILED(hr)) Crash(L"输入系统：无法打开DInput8");
	auto hr2 = lpDirectInput->CreateDevice(GUID_SysKeyboard, lpKeyBoard.GetAddressOf(), nullptr);
	if (!FAILED(hr2)) {
		lpKeyBoard->SetDataFormat(&c_dfDIKeyboard);
		lpKeyBoard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND | DISCL_NOWINKEY);
		lpKeyBoard->Acquire();
	}
	else
	{
		if (FAILED(hr)) Crash(L"输入系统：无法加载键盘");
	}
}

extern float __GetHapticStregth();
void Basic::Input::WriteJoystickButtonSetting()
{
	std::ofstream sav((SaveSystem::GetPath().string() + "\\joysticksetting.sav").c_str(), std::ios::binary);
	if (sav.bad())
		Crash(L"无法创建手柄配置文件！");
	sav.write((char*)config.data(), config.size() * sizeof(config[0]));
	
	const auto haptic = __GetHapticStregth();
	sav.write((char*)&haptic, sizeof(haptic));
	sav.close();

	static_assert(sizeof(float) == 4, "float必须为4");
}

//配置文件格式：
//config的POD形式
//一个float，存储震动强度

extern void __SetHapticStrength(float);
void Basic::Input::LoadJoystickButtonSetting()
{
	std::fill(config.begin(), config.end(), Key::Error);

	try {
		std::ifstream in((SaveSystem::GetPath().string() + "\\joysticksetting.sav").c_str(), std::ios::binary);
		if (!in.good())
			throw Basic::Asset::FileNotFound("Gamepad savedata");

		in.read((char*)config.data(), config.size() * sizeof(config[0]));

		float haptic;
		in.read((char*)&haptic, sizeof(haptic));
		in.close();

		__SetHapticStrength(haptic);
	}
	catch (Basic::Asset::FileNotFound)
	{
		config[0] = Key::Shoot;
		config[1] = Key::Bomb;
		config[2] = Key::Slow;

		__SetHapticStrength(1.0f);
	}
	
}

inline static Basic::Input::Key translateKeyboard(int sdl_k) {
	using Basic::Input::Key;
	switch (sdl_k) {
	case SDLK_z:
		return Key::Shoot;
	case SDLK_x:
		return Key::Bomb;
	case SDLK_LSHIFT:
		return Key::Slow;
	case SDLK_ESCAPE:
		return Key::Esc;
	case 13:
		return Key::Enter;
	case SDLK_F1:
		return Key::F1;
	case SDLK_F2:
		return Key::F2;
	case SDLK_F3:
		return Key::F3;
	case SDLK_F4:
		return Key::F4;
	default:
		return Key::Error;
	}
}


inline void getVecFormKeyBoard(int& vecX, int& vecY) {
	//auto keyMap = GetKeyboardState(nullptr);
	
	
	vecX = vecY = 0;
	if (keyboardBuf[DIK_UP] & 0x8000) vecY -= 32768;
	if (keyboardBuf[DIK_DOWN] & 0x8000) vecY += 32767;
	if (keyboardBuf[DIK_LEFT] & 0x8000) vecX -= 32768;
	if (keyboardBuf[DIK_RIGHT] & 0x8000) vecX += 32767;
}

static std::array<int, 2> axis;

static std::optional<std::function<void(int)>> joystickSetter;

bool Basic::Input::Translate(Message & out, const SDL_Event & in)
{
	if (in.type == SDL_JOYBUTTONDOWN) {
		if (config[in.jbutton.button] != Key::Error) {
			out.type = Message::Type::KeyDown;
			out.key = config[in.jbutton.button];
			return true;
		}
	}
	else if (in.type == SDL_JOYBUTTONUP) {

		if (joystickSetter.has_value())
		{
			(*joystickSetter)(in.jbutton.button);
			return false;
		}

		else if (config[in.jbutton.button] != Key::Error) {
			out.type = Message::Type::KeyUp;
			out.key = config[in.jbutton.button];
			return true;
		}
	}
	else if (in.type == SDL_JOYAXISMOTION) {
		out.type = Basic::Input::Message::Type::VecMotion;
		try {
			axis.at(in.jaxis.axis) = in.jaxis.value;
		}
		catch (...) {}
		out.vec.x = axis[0];
		out.vec.y = axis[1];

		if (out.vec.x <= 3276 && out.vec.x >= -3276)
			out.vec.x = 0;
		if (out.vec.y <= 3276 && out.vec.y >= -3276)
			out.vec.y = 0;

		return true;
	}
	else if (in.type == SDL_JOYHATMOTION) {
		out.type = Basic::Input::Message::Type::VecMotion;
		switch (in.jhat.value) {
		case SDL_HAT_LEFTUP:
			out.vec.x = -32768;
			out.vec.y = -32768;
			return true;
		case SDL_HAT_UP:
			out.vec.x = 0;
			out.vec.y = -32768;
			return true;
		case SDL_HAT_RIGHTUP:
			out.vec.x = 32767;
			out.vec.y = -32768;
			return true;
		case SDL_HAT_RIGHT:
			out.vec.x = 32767;
			out.vec.y = 0;
			return true;
		case SDL_HAT_RIGHTDOWN:
			out.vec.x = 32767;
			out.vec.y = 32767;
			return true;
		case SDL_HAT_DOWN:
			out.vec.x = 0;
			out.vec.y = 32767;
			return true;
		case SDL_HAT_LEFTDOWN:
			out.vec.x = -32768;
			out.vec.y = 32767;
			return true;
		case SDL_HAT_LEFT:
			out.vec.x = -32768;
			out.vec.y = 0;
			return true;
		case SDL_HAT_CENTERED:
			out.vec.x = 0;
			out.vec.y = 0;
			return true;
		}
	}
	return false;
}


static int lastVecX = 0, lastVecY = 0;
static short lastKeyState[6] = { 0 };
static short lastDebugKeyState[4] = { 0 };
bool Basic::Input::GetKeyboardMessage(Message & msg)
{

	//寻找与上一帧不同的键盘状态
	if (lpKeyBoard) lpKeyBoard->GetDeviceState(sizeof(keyboardBuf), &keyboardBuf);

	//寻找方向键
	int vecX = 0, vecY = 0;
	if(inputEnabled) getVecFormKeyBoard(vecX,vecY);
	if (vecX != lastVecX || lastVecY != vecY) {
		lastVecX = vecX;
		lastVecY = vecY;
		//返回VecMotion消息
		msg.type = Message::Type::VecMotion;
		msg.vec.x = vecX;
		msg.vec.y = vecY;
		return true;
	}

	//寻找操作键
	const static int keyMap[] = {
		DIK_Z,
		DIK_X,
		DIK_LSHIFT,
		DIK_RETURN,
		DIK_ESCAPE,
		DIK_SPACE
	};
	for (int i = 0; i < 6; ++i) {
		short pressed = inputEnabled ? keyboardBuf[keyMap[i]] & 0x8000 : 0;
		if (lastKeyState[i] != pressed) {
			lastKeyState[i] = pressed;
			//创建和返回消息
			msg.type = pressed ? Message::Type::KeyDown : Message::Type::KeyUp;
			msg.key = (Key)i;
			return true;
		}
	}

	//寻找调试键
	//寻找操作键
	const static int keyDebugMap[] = {
		DIK_F1,
		DIK_F2,
		DIK_F3,
		DIK_F4
	};
	for (int i = 0; i < 4; ++i) {
		//short pressed = inputEnabled ? GetAsyncKeyState(keyDebugMap[i]) & 0x8000 : 0;
		short pressed = keyboardBuf[keyDebugMap[i]] & 0x8000;
		if (lastDebugKeyState[i] != pressed) {
			lastDebugKeyState[i] = pressed;
			//创建和返回消息
			msg.type = pressed ? Message::Type::KeyDown : Message::Type::KeyUp;
			msg.key = (Key)(11+i);
			return true;
		}
	}
	return false;
}

void Basic::Input::ResetKeyboardState()
{
	memset(lastKeyState, 0, sizeof(lastKeyState));
	memset(lastDebugKeyState, 0, sizeof(lastDebugKeyState));
	lastVecX = lastVecY = 0;
}

void Basic::Input::EnableInput(bool b)
{
	inputEnabled = b;
}

Basic::Input::JoystickSetupper::JoystickSetupper(std::function<void(int)>&& f)
{
	assert(!joystickSetter.has_value());
	joystickSetter.emplace(std::move(f));
}

Basic::Input::JoystickSetupper::~JoystickSetupper()
{
	joystickSetter.reset();
}
