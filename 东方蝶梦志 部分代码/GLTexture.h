#pragma once
#include "glew.h"
#include <string>
#include <cstdint>
#include <vector>
#include <SDL.h>
namespace Basic {
	namespace Video {
		namespace LowerRenderer {
			class GLTexture final
			{
			public:
				struct UVRect {
					double x, y, w, h;
				};
			private:

				GLuint m_texture = 0;
				std::vector<UVRect> m_rects;
				uint16_t m_w = 0, m_h = 0;
				std::string m_texName;

			public:
				GLTexture();
				GLTexture(const GLTexture&) = delete;
				GLTexture(GLTexture&&) = delete;
				const GLTexture& operator = (GLTexture&&) = delete;
				const GLTexture& operator = (const GLTexture&) = delete;
				GLTexture(const std::string& name);;
				~GLTexture();

				void EarlyDelete();

				void FillEmptyData(int w,int h);
				inline operator GLuint () const { return m_texture; }
				void LoadImage(const std::string& name);
				void LoadSurface(SDL_Surface*);
				void LoadSurfaceSDLRGBA8888(SDL_Surface*);
				inline UVRect GetUVRect(uint32_t num) const { return m_rects.at(num); }
				inline void GetSize(int& w, int& h) { w = m_w; h = m_h; }
				inline void GetSize(int num, int& w, int& h) {
					h = int(m_h*m_rects[num].h);
					w = int(m_w*m_rects[num].w);
				}
				inline uint32_t RectNum() {
					return m_rects.size();
				}
				void Clear();
				inline bool Empty() { return m_texture == 0; }

				inline void Bind() {
					glBindTexture(GL_TEXTURE_2D, m_texture);
				}
				inline void Bind(int i) {
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, m_texture);
				}
				inline static void Unbind(int i) {
					glActiveTexture(GL_TEXTURE0 + i);
					glBindTexture(GL_TEXTURE_2D, 0);
				}
				static void SetFliter(GLenum);
			};
		}
	}
}
