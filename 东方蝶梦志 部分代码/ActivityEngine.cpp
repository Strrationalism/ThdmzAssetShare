#include "NuzPrecompiled.h"
#include "ActivityEngine.h"
#include "Activity.h"
#include "Window.h"
#include "glew.h"
#include "BGMPlayer.h"
#include "Globals.h"
#include "SeManager.h"
#include "Haptic.h"


using namespace Basic::ActivityEngine;
using namespace std;


static double waitFps(int FPS) {
	static int fps_count = 0, count0t;
	static int f[240];
	static double ave;

	{
		int term, i, gnt;
		static int t = 0;
		if (fps_count == 0) {
			if (t == 0)
				term = 0;
			else
				term = count0t + 1000 - SDL_GetTicks();
		}
		else
			term = (int)(count0t + fps_count*(1000.0 / FPS)) - SDL_GetTicks();

		if (term>0)
			SDL_Delay(term);

		gnt = SDL_GetTicks();

		if (fps_count == 0)
			count0t = gnt;
		f[fps_count] = gnt - t;
		t = gnt;
		if (fps_count == FPS - 1) {
			ave = 0;
			for (i = 0; i<FPS; i++)
				ave += f[i];
			ave /= FPS;
		}
		++fps_count;
		fps_count = fps_count%FPS;
	}
	return 1000.0f / ave;
}



void Basic::ActivityEngine::ActivityEngine::SetFPS(int fps)
{
	m_frameMode = fps;
}

void Basic::ActivityEngine::ActivityEngine::Goto(std::shared_ptr<Activity> a)
{
	m_taskList.AddTask([this,a]() {
		m_now.top()->OnLeave();
		m_now.pop();

		m_now.push(a);
		m_now.top()->OnEntry();
	}, 0);
}

void Basic::ActivityEngine::ActivityEngine::Call(std::shared_ptr<Activity> a)
{
	m_now.push(a);
	m_now.top()->OnEntry();
}

void Basic::ActivityEngine::ActivityEngine::Return()
{
	m_taskList.AddTask([this]
	{
		m_now.top()->OnLeave();
		m_now.pop();
	},0);
}

static SDL_Joystick* joystick = nullptr;

bool JoystickConnected()
{
	return joystick != nullptr;
}

void Basic::ActivityEngine::ActivityEngine::Run(std::shared_ptr<Activity> a)
{
	m_cnt = 0;
	m_run = true;
	//m_now.clear();
	m_now.push(a);
	m_now.top()->OnEntry();
	auto window = (SDL_Window*)Basic::Video::Window::GetWindow();

	
	Sint32 joystickIndex = -1;
	
	static uint64_t begin;

	bool logShow = false;

	SDL_Event e;
	Basic::Input::Message msg;
	auto nextTick = SDL_GetTicks();
	begin = SDL_GetTicks();
	int frames = 0;

	//计算逻辑帧率
	int logic_frames = 0;
	uint64_t logic_time = SDL_GetTicks();
	int last_logicFrame = 0;

	//当前Draw是否有意义
	bool redraw = false;

	//deltaTimer
	Uint64 clockNow = SDL_GetTicks();

	//绘制循环
	while (m_run) {
		bool drawTask = false;

		uint32_t loops = 0;

		//逻辑循环
		while(SDL_GetTicks() > nextTick && loops < 3){
			redraw = true;
			if (SDL_GetTicks() - nextTick >= (1000 / LPS) * 20)
				nextTick = SDL_GetTicks();

			++loops;
			++m_cnt;
			
			Basic::Sound::BgmPlayer::Update();
			Basic::Sound::SeManager::Update();

			m_taskList.Update();

			//收集和分发输入消息
			SDL_PumpEvents();
			while (Basic::Input::GetKeyboardMessage(msg)) {
				//键盘事件分发
				m_now.top()->OnEvent(msg);
			}
			while (SDL_PollEvent(&e)) {
				//SDL事件分发

				//LOG控制器
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_F5)
						logShow = !logShow;
					else if (e.key.keysym.sym == SDLK_F6) {
						extern void __LogRenderer__WriteToFile();
						__LogRenderer__WriteToFile();
						Log("Log has written to thdmz.log");
					}
				}

				if (e.type == SDL_QUIT)
					m_run = false;
				else if (e.type == SDL_JOYDEVICEADDED) {
					if (joystick == nullptr) {
						//链接手柄
						joystick = SDL_JoystickOpen(e.jdevice.which);
						joystickIndex = SDL_JoystickInstanceID(joystick);
						if (SDL_JoystickNumButtons(joystick) >= 256)
							Crash(L"你手柄按键太多了。");
						if (SDL_JoystickIsHaptic(joystick) == 1 && SDL_NumHaptics() >= 1)
							Basic::Haptic::SetHaptic(SDL_HapticOpenFromJoystick(joystick));

						Basic::Haptic::Play(400);
					}
				}
				else if (e.type == SDL_JOYDEVICEREMOVED) {
					if (e.jdevice.which == joystickIndex) {
						Basic::Haptic::SetHaptic(nullptr);
						SDL_JoystickClose(joystick);
						joystickIndex = -1;
						joystick = nullptr;
					}

				}
				else if (e.type == SDL_WINDOWEVENT) {
					if (e.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
						//窗口失去焦点时
						Input::EnableInput(false);
						m_now.top()->OnEvent({ Basic::Input::Message::Type::WinFocusLost });
					}
					else if (e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
						//获得焦点时
						Input::EnableInput(true);
					}
				}
				else if (e.type == SDL_RENDER_TARGETS_RESET || e.type == SDL_RENDER_DEVICE_RESET) {
					Crash(L"渲染设备丢失！");
				}
				else {
					if (Basic::Input::Translate(msg, e))
						m_now.top()->OnEvent(msg);

				}
			}
			

			const float deltaTime = (SDL_GetTicks() - clockNow) / 1000.0f;
			m_now.top()->OnUpdate(deltaTime);
			clockNow = SDL_GetTicks();


			nextTick += 1000 / LPS;
			//loops++;

			++logic_frames;
			if (SDL_GetTicks() - begin >= 1000) {
				begin = SDL_GetTicks();
				last_logicFrame = logic_frames;
				logic_frames = 0;
				if(m_frameMode <= 0)
					SDL_SetWindowTitle(window, ("FPS:" + to_string(frames) + "  LPS:" + to_string(last_logicFrame)).c_str());
				frames = 0;
			}
		}

		
		if (redraw) {	//如果本次刷新有意义则刷新
			glClear(GL_DEPTH_BUFFER_BIT);
			m_now.top()->OnDraw();

			if (logShow) {
				extern void __LogRenderer__Draw();
				__LogRenderer__Draw();
			}

			SDL_GL_SwapWindow(window);
		}
		//SDL_SetWindowTitle(window, ("FPS:" + to_string(waitFps(60 / m_frameSkip))).c_str());
		//计算帧率
		if (m_frameMode > 0) {
			SDL_SetWindowTitle(window, ("FPS:" + to_string(waitFps(m_frameMode)) + "  LPS:" + to_string(last_logicFrame)).c_str());
		}
		else {
			++frames;
		}
	}

	Basic::Haptic::SetHaptic(nullptr);
	if (joystick) SDL_JoystickClose(joystick);
}

ActivityEngine & Basic::ActivityEngine::ActivityEngine::GetActivityEngine()
{
	static ActivityEngine ace;
	return ace;
}
