#include "NuzPrecompiled.h"
#include "Window.h"
#include "Globals.h"
#include "Asset.h"
#include "glew.h"
using namespace Basic::Video;
using namespace std;

Window::~Window()
{
	CloseWindow();
}

void Basic::Video::Window::InitWindow(int w, int h, uint32_t flags,bool vsync,int msaa)
{
	const int msaaMode[] = { 16,8,4,2,0 };

	SDL_assert(!m_wnd);	//如果已经创建窗口，则报错

	SDL_VideoInit(nullptr);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GLprofile::SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, msaaMode[msaa] > 0);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, msaaMode[msaa]);

	m_wnd = SDL_CreateWindow("THDMZ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL | flags);


	//加载Icon
	Basic::Asset iconAss("Icon.bmp");
	auto iconRW = SDL_RWFromConstMem(iconAss, iconAss.Size());
	auto iconSur = SDL_LoadBMP_RW(iconRW, iconAss.Size());
	SDL_assert(iconSur);
	SDL_RWclose(iconRW);

	m_rnd = SDL_GL_CreateContext(m_wnd);

	if (!m_wnd) Crash(wstring(L"创建窗口失败：") + Str2WStr(SDL_GetError()));
	if (!m_rnd) Crash(wstring(L"创建渲染环境失败：") + Str2WStr(SDL_GetError()) + L"\n您可能需要更换一个支持OpenGL 2.0的显卡，您的显卡很可能不支持OpenGL 2.0。");
	SDL_GL_MakeCurrent(m_wnd, m_rnd);


	glClearColor(0,0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	SDL_GL_SwapWindow(m_wnd);
	if (GLEW_OK != glewInit()) Crash(L"无法启动GLEW。");	//初始化GLEW
	if(vsync)
		SDL_GL_SetSwapInterval(1);
	else
		SDL_GL_SetSwapInterval(0);

	SDL_SetWindowIcon(m_wnd, iconSur);
	SDL_FreeSurface(iconSur);

}

void Basic::Video::Window::CloseWindow()
{
	if (m_wnd) {
		SDL_GL_DeleteContext(m_rnd);
		SDL_DestroyWindow(m_wnd);
		SDL_VideoQuit();
	}
}

Window & Basic::Video::Window::GetWindow()
{
	static Window wnd;
	return wnd;
}
