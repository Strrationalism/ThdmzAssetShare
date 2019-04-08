#include "NuzPrecompiled.h"
#include "Particle3D.h"
#include "ImageCacheManager.h"
#include "Globals.h"
#include "Asset.h"
#include "GLShader.h"
#include "Sprite2D.h"
#include "Globals.h"
using namespace Basic::Video::Renderer3D;
using namespace Basic::Video::LowerRenderer;
using namespace Basic;

using namespace std;

shared_ptr<GLTexture> Particle3D::m_texture;

static std::optional<Basic::Video::LowerRenderer::GLShader> particleShader;
static GLuint particleShader_attr;

struct ParticleStyleVertex {
	double texCoord[8], vert[8];
};
static std::vector<ParticleStyleVertex> styleVerts;

Particle3D::Particle3D() {
	m_dot.reserve(8192);
}


void Basic::Video::Renderer3D::Particle3D::InitSystem()
{
	m_texture = ImageCacheManager::LoadImageA("Particle3D");

	particleShader.emplace("NoEffect", "Particle3D");
	particleShader_attr = particleShader->AttributeLocation("attr");
	
	glUniform1i(particleShader->UniformLocation("texture"), 0);
	
	for (uint32_t i = 0; i < m_texture->RectNum(); ++i) {
		auto& iRect = m_texture->GetUVRect(i);

		double wd2 = iRect.w / 2;
		double hd2 = iRect.h / 2;

		styleVerts.push_back({
			{
				iRect.x, iRect.y,
				iRect.x + iRect.w, iRect.y,
				iRect.x + iRect.w, iRect.y + iRect.h,
				iRect.x, iRect.y + iRect.h
			},
			{
				-wd2, -hd2,
				wd2,-hd2,
				wd2, hd2,
				-wd2, hd2
			}
		});
	}


}

void Basic::Video::Renderer3D::Particle3D::AddDot(int style, std::function<void(Dot3D&)> initer, std::function<void(Dot3D&, Particle3D&)> program, double x , double y , int num )
{
	//std::lock_guard<std::mutex> g(m_lock);
	for (int i = 0; i < num; ++i) {
		if (m_dot.size() >= 1000) break;
		m_dot.push_back(Dot3D());
		m_dot.back().x = x;
		m_dot.back().y = y;
		m_dot.back().program = program;
		m_dot.back().style = style;
		initer(m_dot.back());
		program(m_dot.back(), *this);
	}
}

struct FastBuffer {
	std::vector<double> attr, vert, texCoord, color;
	FastBuffer() {
		attr.reserve(8192 * 4);
		vert.reserve(8192 * 2);
		texCoord.reserve(8192 * 2);
		color.reserve(8192 * 4);
	}
};

static FastBuffer noEffect, hdr;

void Basic::Video::Renderer3D::Particle3D::Draw() const
{
	//创建顶点缓存

	noEffect.attr.clear();
	noEffect.texCoord.clear();
	noEffect.vert.clear();
	noEffect.color.clear();
	hdr.attr.clear();
	hdr.texCoord.clear();
	hdr.vert.clear();
	hdr.color.clear();

	for (auto& dot : m_dot) {

		if (dot.blend_dst == GL_ONE) {
			for (auto p : styleVerts[dot.style].vert)
				hdr.vert.push_back(p);

			for (auto p : styleVerts[dot.style].texCoord)
				hdr.texCoord.push_back(p);

			for (int i = 0; i < 4; ++i) {
				hdr.attr.push_back(dot.x);
				hdr.attr.push_back(dot.y);
				hdr.attr.push_back(dot.self_angle);
				hdr.attr.push_back(dot.size);
				hdr.color.push_back(dot.r);
				hdr.color.push_back(dot.g);
				hdr.color.push_back(dot.b);
				hdr.color.push_back(dot.alpha);
			}
		}
		else if (dot.blend_dst == GL_ONE_MINUS_SRC_ALPHA) {
			for (auto p : styleVerts[dot.style].vert)
				noEffect.vert.push_back(p);

			for (auto p : styleVerts[dot.style].texCoord)
				noEffect.texCoord.push_back(p);

			for (int i = 0; i < 4; ++i) {
				noEffect.attr.push_back(dot.x);
				noEffect.attr.push_back(dot.y);
				noEffect.attr.push_back(dot.self_angle);
				noEffect.attr.push_back(dot.size);
				noEffect.color.push_back(dot.r);
				noEffect.color.push_back(dot.g);
				noEffect.color.push_back(dot.b);
				noEffect.color.push_back(dot.alpha);
			}
		}
		else {
			Crash(L"未知的粒子混合模式！");
		}
	}

	glUseProgram(*particleShader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *m_texture);

	glEnableVertexAttribArray(particleShader_attr);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(2, GL_DOUBLE, 0, noEffect.vert.data());
	glTexCoordPointer(2, GL_DOUBLE, 0, noEffect.texCoord.data());
	glColorPointer(4, GL_DOUBLE, 0, noEffect.color.data());
	glVertexAttribPointer(particleShader_attr, 4, GL_DOUBLE, false, 0, noEffect.attr.data());
	glDrawArrays(GL_QUADS, 0, noEffect.vert.size() / 2);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, *m_texture);
	glVertexPointer(2, GL_DOUBLE, 0, hdr.vert.data());
	glTexCoordPointer(2, GL_DOUBLE, 0, hdr.texCoord.data());
	glColorPointer(4, GL_DOUBLE, 0, hdr.color.data());
	glVertexAttribPointer(particleShader_attr, 4, GL_DOUBLE, false, 0, hdr.attr.data());
	glDrawArrays(GL_QUADS, 0, hdr.vert.size() / 2);
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glDisableClientState(GL_COLOR_ARRAY);
	glDisableVertexAttribArray(particleShader_attr);

	glUseProgram(0);
}

void Basic::Video::Renderer3D::Particle3D::Update()
{

	int size = m_dot.size();
	//#pragma omp parallel for
	for (int i = 0; i < size;++i) {
		auto& dot = m_dot[i];
		if(dot.program)
			dot.program(dot, *this);
		dot.x += sin(dot.angle) * dot.speed;
		dot.y += cos(dot.angle) * dot.speed;
	}
	auto nend = remove_if(m_dot.begin(), m_dot.end(), [](const Dot3D& i) {
		return i.alpha <= 0 || i.size <= 0;
	});
	m_dot.erase(nend, m_dot.end());
}
