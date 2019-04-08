#pragma once
#include "Sprite2D.h"
#include <string>
#include <map>
#include "Sprite2DWStreamLayout.h"
#include "../FontGenerator/FontMap.h"
namespace Basic {
	namespace Video {
		namespace Renderer2D {
			class CtxFont final
			{
			public:

				CtxFont(const char* fontName, const char* ctxNameBase);
				void SetSprite2DToChar(wchar_t ch, Sprite2D& sprite);
				void SetSprite2DWStreamToString(const std::wstring& str, Sprite2DWStreamLayout& layout);

				std::pair<int, int> GetSize(wchar_t ch);

			private:
				std::string m_ctxNameBase;
				std::map<wchar_t, CharInfo> m_charInfos;
			};
		}
	}
}
