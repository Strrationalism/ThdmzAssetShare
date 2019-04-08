#include "NuzPrecompiled.h"
#include "GLShader.h"
#include "Globals.h"
#include "Asset.h"
using namespace Basic::Video::LowerRenderer;


GLuint Basic::Video::LowerRenderer::GLShader::compileSingleShader(const std::string & file, GLenum type)
{
	//加载Shader代码
	Basic::Asset codeAss(file);
	auto needToDecode = codeAss.Ptr();

	std::transform(needToDecode->begin(), needToDecode->end(), needToDecode->begin(),
		[](uint8_t b) {return b ^ uint8_t(0x80u); });

	needToDecode->back() = uint8_t('\0');

	const char* code = codeAss;

	auto s = glCreateShader(type);
	if (!s) Crash(L"无法创建单个Shader程序。");
	glShaderSource(s, 1, &code, nullptr);
	glCompileShader(s);

	GLint compileResult;
	glGetShaderiv(s, GL_COMPILE_STATUS, &compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		glGetShaderiv(s, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			char *log = (char *)malloc(logLen);
			GLsizei written;
			glGetShaderInfoLog(s, logLen, &written, log);
			if (log == nullptr) abort();
			std::string slog(log);
			free(log);
			Crash(L"Shader编译失败：\n在" + Str2WStr(file) + L"里\n" + Str2WStr(slog));
		}
	}
	return s;
}

Basic::Video::LowerRenderer::GLShader::GLShader()
{
}

Basic::Video::LowerRenderer::GLShader::GLShader(const std::string& frag, std::string vert)
{
	Load(frag, vert);
}

GLShader::~GLShader()
{
	if(m_program)
		glDeleteProgram(m_program);
}

void Basic::Video::LowerRenderer::GLShader::Load(const std::string& frag, std::string vert)
{
	Clear();
	m_program = glCreateProgram();
	if (!m_program) Crash(L"无法创建着色器程序。");
	auto v = compileSingleShader("ShaderBinary/" + vert + ".vert", GL_VERTEX_SHADER);
	auto f = compileSingleShader("ShaderBinary/" + frag + ".frag", GL_FRAGMENT_SHADER);

	glAttachShader(m_program, v);
	glAttachShader(m_program, f);
	glLinkProgram(m_program);
	glDeleteShader(f);
	glDeleteShader(v);
}

void Basic::Video::LowerRenderer::GLShader::Clear()
{
	if (m_program) glDeleteProgram(m_program);
	m_program = 0;
}
