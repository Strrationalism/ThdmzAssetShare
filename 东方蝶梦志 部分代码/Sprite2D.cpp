#include "NuzPrecompiled.h"
#include "Sprite2D.h"
#include "ImageCacheManager.h"
#include "Globals.h"
#include "Matrix.h"
using namespace Basic::Video::Renderer2D;
using namespace Basic::Video::LowerRenderer;

std::map<std::shared_ptr<Basic::Video::LowerRenderer::GLTexture>, Sprite2D::SingleFastBuffer> Sprite2D::m_fastBuffer[2];

void Basic::Video::Renderer2D::Sprite2D::UpdateVertex()
{
	m_drawVerts[0] = -m_w;
	m_drawVerts[1] = -m_h;

	m_drawVerts[2] = m_w;
	m_drawVerts[3] = -m_h;

	m_drawVerts[4] = m_w;
	m_drawVerts[5] = m_h;

	m_drawVerts[6] = -m_w;
	m_drawVerts[7] = m_h;

	for (int i = 0; i < 4; ++i) {
		m_drawVerts[2 * i] -= m_centerX;
		m_drawVerts[2 * i + 1] -= m_centerY;
	}

	if (m_flipX) {
		m_drawVerts[1] = -m_drawVerts[1];
		m_drawVerts[3] = -m_drawVerts[3];
		m_drawVerts[5] = -m_drawVerts[5];
		m_drawVerts[7] = -m_drawVerts[7];
	}

	if (m_flipY) {
		m_drawVerts[0] = -m_drawVerts[0];
		m_drawVerts[2] = -m_drawVerts[2];
		m_drawVerts[4] = -m_drawVerts[4];
		m_drawVerts[6] = -m_drawVerts[6];
	}

	if (m_angle != 0) {
		for (int i = 0; i < 4; ++i)
			Basic::Math::Matrix::RotateVec2_2D(
				m_drawVerts[i * 2],
				m_drawVerts[i * 2 + 1],
				m_angle*M_PI / 180,
				m_drawVerts + i * 2,
				m_drawVerts + i * 2 + 1
			);
	}
	for (int i = 0; i < 4; ++i) {
		m_drawVerts[i * 2] += m_x;
		m_drawVerts[i * 2 + 1] += m_y;
	}
}



Sprite2D::Sprite2D()
{
}


Sprite2D::~Sprite2D()
{
}

void Basic::Video::Renderer2D::Sprite2D::UseImage(const std::string & ctx, int num)
{
	m_texNum = num;
	m_tex = Basic::Video::LowerRenderer::ImageCacheManager::LoadImage(ctx);
	auto & imageRect = m_tex->GetUVRect(num);

	m_drawTexCoords[0] = imageRect.x;
	m_drawTexCoords[1] = imageRect.y;
	m_drawTexCoords[2] = imageRect.x + imageRect.w;
	m_drawTexCoords[3] = imageRect.y;
	m_drawTexCoords[4] = imageRect.x + imageRect.w;
	m_drawTexCoords[5] = imageRect.y + imageRect.h;
	m_drawTexCoords[6] = imageRect.x;
	m_drawTexCoords[7] = imageRect.y + imageRect.h;

	m_tex->GetSize(m_w, m_h);
	m_w = int(imageRect.w * m_w) / 2;
	m_h = int(imageRect.h * m_h) / 2;

}

void Basic::Video::Renderer2D::Sprite2D::UseImage(SDL_Surface * sur,bool needConvert)
{
	m_tex = Basic::Video::LowerRenderer::ImageCacheManager::RenderSurface(sur, needConvert);

	m_drawTexCoords[0] = m_drawTexCoords[1] = m_drawTexCoords[3] = m_drawTexCoords[6] = 0;
	m_drawTexCoords[2] = m_drawTexCoords[4] = m_drawTexCoords[5] = m_drawTexCoords[7] = 1;

	m_w = sur->w / 2;
	m_h = sur->h / 2;
}


void Basic::Video::Renderer2D::Sprite2D::Clear()
{
	m_tex.reset();
}


void Basic::Video::Renderer2D::Sprite2D::Draw() const
{
	glColor4d(m_r, m_g, m_b, m_alpha);
	glBindTexture(GL_TEXTURE_2D, *m_tex);
	
	glVertexPointer(2, GL_DOUBLE, 0, m_drawVerts);
	glTexCoordPointer(2, GL_DOUBLE, 0, m_drawTexCoords);
	glDrawArrays(GL_QUADS, 0, 4);
}

void Basic::Video::Renderer2D::Sprite2D::OldDraw() const
{
	glColor4d(m_r, m_g, m_b, m_alpha);
	glBindTexture(GL_TEXTURE_2D, *m_tex);

	glBegin(GL_TRIANGLE_STRIP); {
		glTexCoord2d(m_drawTexCoords[0],m_drawTexCoords[1]); glVertex2d(m_drawVerts[0], m_drawVerts[1]);
		glTexCoord2d(m_drawTexCoords[2], m_drawTexCoords[3]); glVertex2d(m_drawVerts[2], m_drawVerts[3]);
		glTexCoord2d(m_drawTexCoords[6], m_drawTexCoords[7]); glVertex2d(m_drawVerts[6], m_drawVerts[7]);
		glTexCoord2d(m_drawTexCoords[4], m_drawTexCoords[5]); glVertex2d(m_drawVerts[4], m_drawVerts[5]);
	}
	glEnd();
}

void Basic::Video::Renderer2D::Sprite2D::DrawToFastBuffer(int fastBufferIndex) const
{
	if (m_fastBuffer[fastBufferIndex].count(m_tex) == 0) {
		m_fastBuffer[fastBufferIndex][m_tex].colors.reserve(8192 * 4);
		m_fastBuffer[fastBufferIndex][m_tex].drawVerts.reserve(8192 * 3);
		m_fastBuffer[fastBufferIndex][m_tex].texCoords.reserve(8192 * 2);
	}

	auto & fb = m_fastBuffer[fastBufferIndex][m_tex];

	for (int i = 0; i < 4; ++i) {
		//写出4个颜色顶点
		fb.colors.push_back(m_r);
		fb.colors.push_back(m_g);
		fb.colors.push_back(m_b);
		fb.colors.push_back(m_alpha);
	}

	for (auto p : m_drawVerts)
		fb.drawVerts.push_back(p);
	for (auto p : m_drawTexCoords)
		fb.texCoords.push_back(p);
}

void Basic::Video::Renderer2D::Sprite2D::DrawFastBuffer()
{
	glEnableClientState(GL_COLOR_ARRAY);

	//glLoadIdentity();
	glColor4d(1.0, 1.0, 1.0, 1.0);
	for (auto& p : m_fastBuffer[0]) {
		if (p.second.drawVerts.empty()) continue;
		glBindTexture(GL_TEXTURE_2D, *p.first);
		glVertexPointer(2, GL_DOUBLE, 0, p.second.drawVerts.data());
		glTexCoordPointer(2, GL_DOUBLE, 0, p.second.texCoords.data());
		glColorPointer(4, GL_DOUBLE, 0, p.second.colors.data());
		glDrawArrays(GL_QUADS, 0, p.second.drawVerts.size()/2);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	for (auto& p : m_fastBuffer[1]) {
		if (p.second.drawVerts.empty()) continue;
		glBindTexture(GL_TEXTURE_2D, *p.first);
		glVertexPointer(2, GL_DOUBLE, 0, p.second.drawVerts.data());
		glTexCoordPointer(2, GL_DOUBLE, 0, p.second.texCoords.data());
		glColorPointer(4, GL_DOUBLE, 0, p.second.colors.data());
		glDrawArrays(GL_QUADS, 0, p.second.drawVerts.size() / 2);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glDisableClientState(GL_COLOR_ARRAY);
}

void Basic::Video::Renderer2D::Sprite2D::ClearFastBuffer()
{
	for (auto& b : m_fastBuffer) {
		for (auto& p : b) {
			p.second.colors.clear();
			p.second.drawVerts.clear();
			p.second.texCoords.clear();
		}
	}
}
