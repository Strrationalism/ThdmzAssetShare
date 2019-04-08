#pragma once
#include <map>
#include <memory>
#include <SDL.h>
#include "GLTexture.h"
namespace Basic {
	namespace Video {
		namespace LowerRenderer {
			class ImageCacheManager final
			{
			private:
				static std::map<std::string, std::weak_ptr<GLTexture>> m_imageCache;
				inline ImageCacheManager() {};
			public:
				static std::shared_ptr<GLTexture> LoadImage(const std::string& path);
				static std::shared_ptr<GLTexture> RenderSurface(SDL_Surface*,bool needConvert = false);

				static void EarlyDelete();
			};
		}
	}
}
