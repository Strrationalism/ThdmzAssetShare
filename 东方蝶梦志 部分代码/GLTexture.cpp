#include "NuzPrecompiled.h"
#include "GLTexture.h"
#include "CtxReader.h"
#include "Globals.h"
#include "Asset.h"
using namespace Basic::Video::LowerRenderer;
static GLfloat gl_Fliter;

GLTexture::GLTexture()
{
	glGenTextures(1, &m_texture);
	if(!m_texture) Crash(L"无法创建纹理。");

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_Fliter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_Fliter);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Basic::Video::LowerRenderer::GLTexture::GLTexture(const std::string & name) {
	glGenTextures(1, &m_texture);
	if (!m_texture) Crash(L"无法创建纹理。");
	LoadImage(name);
}


GLTexture::~GLTexture()
{
	EarlyDelete();
}

void Basic::Video::LowerRenderer::GLTexture::EarlyDelete()
{
	if (m_texture)
		glDeleteTextures(1, &m_texture);
	m_texture = 0;
}

void Basic::Video::LowerRenderer::GLTexture::FillEmptyData(int w, int h)
{
	Clear();
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
}

void GLTexture::LoadImage(const std::string & name)
{
	Clear();
	Asset ass("Textures/"+name+".ctx");
	auto p = ass.Ptr();
	m_texName = name;
	CTXReader reader;
	reader.OpenBuffer((char*)&(*p)[0]);
	m_w = reader.GetLengthX();
	m_h = reader.GetLengthY();
	for (int i = 0; i < reader.GetRectNum(); ++i) {
		int x, y, w, h;
		reader.GetRect(i, x, y, w, h);
		//y = m_h - y;	//UV坐标转换
		m_rects.push_back({ double(x) / m_w, double(y) / m_h, double(w) / m_w, double(h) / m_h });
	}
	glBindTexture(GL_TEXTURE_2D, m_texture);

	if (reader.GetCompressFormat() == GL_RGBA)
		glTexImage2D(GL_TEXTURE_2D, 0, 4, m_w, m_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, reader.GetPixels());
	else
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, reader.GetCompressFormat(), reader.GetLengthX(), reader.GetLengthY(), 0, reader.GetPixelsSize(), reader.GetPixels());
}

void GLTexture::LoadSurface(SDL_Surface * sur)
{
	auto convert = SDL_ConvertSurfaceFormat(sur, SDL_PIXELFORMAT_ABGR8888,0 );
	LoadSurfaceSDLRGBA8888(convert);
	SDL_FreeSurface(convert);
}

void Basic::Video::LowerRenderer::GLTexture::LoadSurfaceSDLRGBA8888(SDL_Surface * convert)
{
	Clear();
	m_rects.push_back({ 0,0,1,1 });
	m_w = convert->w;
	m_h = convert->h;
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, convert->w, convert->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, convert->pixels);
}

void GLTexture::Clear()
{
	m_w = m_h = 0;
	m_rects.clear();
}

void Basic::Video::LowerRenderer::GLTexture::SetFliter(GLenum e)
{
	gl_Fliter = (GLfloat)e;
}
