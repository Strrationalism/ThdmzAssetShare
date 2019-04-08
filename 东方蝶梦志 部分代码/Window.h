#pragma once
#include <SDL.h>
namespace Basic {
	namespace Video {
		class Window final
		{
		private:
			SDL_Window* m_wnd = nullptr;
			SDL_GLContext m_rnd = nullptr;
		public:
			~Window();
			void InitWindow(int w, int h, uint32_t flags, bool vsync,int msaa);
			void CloseWindow();
			inline void Raise() {
				SDL_RaiseWindow(m_wnd);
			}
			inline operator SDL_Window* () { return m_wnd; }
			static Window& GetWindow();
		};
	}
}
