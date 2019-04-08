#pragma once
#include <SDL_rect.h>
#include "Sprite2D.h"

namespace Basic {
	namespace Video {
		namespace Renderer2D {
			class Sprite2DWStreamLayout
			{
			public:
				enum class LayoutMode {
					LeftTop, CenterTop, RightTop
				};
			private:
				std::vector<std::shared_ptr<Sprite2D>> m_sprites;
				SDL_Rect m_rect;
				LayoutMode m_mode;
				int m_spriteSizeOffsetW = 0, m_spriteSizeOffsetH = 0;
			public:


				Sprite2DWStreamLayout(SDL_Rect rect, LayoutMode mode = LayoutMode::LeftTop);
				void SetRect(SDL_Rect rect);
				void SetMode(LayoutMode);
				std::pair<int, int> GetContentSize();
				void SetSpriteSizeOffset(int w, int h);
				void UpdateStream();
				void Draw() const;
				void DrawToFastBuffer(int fastBufferIndex = 0) const;

				void AddSprite(std::shared_ptr<Sprite2D>& sprite);

				auto begin() { return m_sprites.begin(); }
				auto end() { return m_sprites.end(); }


			};
		}
	}
}