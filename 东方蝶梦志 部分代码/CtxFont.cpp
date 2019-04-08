#include "CtxFont.h"
#include "NuzPrecompiled.h"
#include "CtxFont.h"
#include "Asset.h"
#include "ImageCacheManager.h"
#include "Globals.h"

using namespace Basic::Video::Renderer2D;

Basic::Video::Renderer2D::CtxFont::CtxFont(const char * fontName, const char * ctxNameBase):
	m_ctxNameBase(ctxNameBase + std::string("_"))
{
	Basic::Asset font(fontName);
	const CharInfo* infos = (const CharInfo*)(const char*)font;
	auto infoSize = font.Size() / sizeof(CharInfo);

	for (size_t i = 0; i < infoSize; ++i)
	{
		m_charInfos[infos[i].ch] = infos[i];
	}
}

void Basic::Video::Renderer2D::CtxFont::SetSprite2DToChar(wchar_t ch, Sprite2D & sprite)
{
	try {
		const auto& charInfo = m_charInfos.at(ch);
		const std::string ctxName = m_ctxNameBase + std::to_string(charInfo.ctxPage);
		sprite.UseImage(ctxName, charInfo.ctxRect);
	}
	catch(std::exception e)
	{
		Crash(std::wstring(L"×Ö¿âÈ±×Ö - ") + ch);
	}

}

void Basic::Video::Renderer2D::CtxFont::SetSprite2DWStreamToString(const std::wstring & str, Sprite2DWStreamLayout & layout)
{
	for (auto p : str)
	{
		auto pSp = std::make_shared<Sprite2D>();
		SetSprite2DToChar(p, *pSp);
		layout.AddSprite(pSp);
	}
}

std::pair<int, int> Basic::Video::Renderer2D::CtxFont::GetSize(wchar_t ch)
{
	try {
		const auto& charInfo = m_charInfos.at(ch);
		const std::string ctxName = m_ctxNameBase + std::to_string(charInfo.ctxPage);


		auto pTex = LowerRenderer::ImageCacheManager::LoadImage(ctxName);

		int tw, th;
		pTex->GetSize(tw, th);

		auto& rect = pTex->GetUVRect(charInfo.ctxRect);
		int w = rect.w * tw;
		int h = rect.h * th;

		return std::make_pair(w, h);
	}
	catch (std::exception e)
	{
		Crash(std::wstring(L"×Ö¿âÈ±×Ö - ") + ch);
	}
	return std::make_pair(-1, -1);
}
