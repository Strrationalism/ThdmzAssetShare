#include "NuzPrecompiled.h"
#include "NumberDisplay.h"
#include "glew.h"

Basic::Video::Renderer2D::NumberDisplay::NumberDisplay(const std::string & styleTex) {
	m_tex = LowerRenderer::ImageCacheManager::LoadImage(styleTex);

	int w, h;
	m_tex->GetSize(w, h);

	for (int i = 0; i <= 9; ++i) {

		const auto& imageRect = m_tex->GetUVRect(i);

		double singleW = imageRect.w * w, singleH = imageRect.h * h;
		m_singleW += singleW;
		
		drawNumber[i].DefineBegin(); {
			glBegin(GL_QUADS); {
				glTexCoord2d(imageRect.x, imageRect.y); glVertex2d(0,0);	//左上
				glTexCoord2d(imageRect.x + imageRect.w, imageRect.y); glVertex2d(singleW, 0);	//右上
				glTexCoord2d(imageRect.x + imageRect.w, imageRect.y + imageRect.h); glVertex2d(singleW, singleH);	//右下
				glTexCoord2d(imageRect.x, imageRect.y + imageRect.h); glVertex2d(0, singleH);	//左下
			}
			glEnd();
			glTranslated(singleW, 0, 0);
		}
		LowerRenderer::GLCallList::DefineEnd();
	}

	m_singleW /= 10;
}

void Basic::Video::Renderer2D::NumberDisplay::Draw() const
{
	glColor4d(m_r, m_g, m_b, m_alpha);
	if (m_leftTopPos) {
		//左上坐标模式
		//右移位置
		glTranslated(- m_singleW * (m_displayFormat == -1 ? m_number.size() : m_displayFormat), 0, 0);
	}
	glTranslated(m_x, m_y,0);
	glScaled(m_zoom, m_zoom, 0);

	glBindTexture(GL_TEXTURE_2D, *m_tex);

	if (m_displayFormat != -1) {
		//先绘制几个0
		int zeroCnt = m_displayFormat - m_number.size();
		if (zeroCnt < 0) {
			//如果溢出
			//则全部绘制为9后中断
			for (int i = 0; i < m_displayFormat; ++i) {
				drawNumber[9]();
				
			}
			return;
		}
		else {
			for (int i = 0; i < zeroCnt; ++i) {
				drawNumber[0]();
			}
		}
	}
	//无格式直接绘制
	for (auto p : m_number) {
		drawNumber[p]();
	}
	glLoadIdentity();
}
