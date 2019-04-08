#include "NuzPrecompiled.h"
#include "Animation2D.h"
#include "ImageCacheManager.h"
void Basic::Video::Renderer2D::Animation2D::setSrc(int num)
{
	m_imageRect = m_tex->GetUVRect(num);

	m_tex->GetSize(m_w, m_h);
	m_w = int(m_imageRect.w * m_w) / 2;
	m_h = int(m_imageRect.h * m_h) / 2;

	m_drawTexCoords[0] = m_imageRect.x;
	m_drawTexCoords[1] = m_imageRect.y;
	m_drawTexCoords[2] = m_imageRect.x + m_imageRect.w;
	m_drawTexCoords[3] = m_imageRect.y;
	m_drawTexCoords[4] = m_imageRect.x + m_imageRect.w;
	m_drawTexCoords[5] = m_imageRect.y + m_imageRect.h;
	m_drawTexCoords[6] = m_imageRect.x;
	m_drawTexCoords[7] = m_imageRect.y + m_imageRect.h;
	//updateVertArray();
}

void Basic::Video::Renderer2D::Animation2D::UseImage(const std::string & ctx, int screenFrame_frame, int begin, int num)
{
	m_tex = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage(ctx);
	setSrc(begin);
	if (num == -1) m_allFrame = m_tex->RectNum();
	else m_allFrame = num;
	m_nowFrame = begin;
	m_begin = begin;
	m_timer = screenFrame_frame;
	m_screenFrame_frame = screenFrame_frame;
}

void Basic::Video::Renderer2D::Animation2D::Update()
{
	--m_timer;
	if (m_timer <= 0) {
		++m_nowFrame;
		m_nowFrame %= m_allFrame;
		m_timer = m_screenFrame_frame;
		setSrc(m_nowFrame + m_begin);
	}
}

