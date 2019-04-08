#pragma once
#include <string>
#include "glew.h"
namespace Basic {
	namespace Video {
		namespace LowerRenderer {
			class GLShader final
			{
			private:
				GLuint m_program = 0;

				GLuint compileSingleShader(const std::string& file, GLenum type);
			public:
				GLShader(const GLShader&) = delete;
				GLShader(GLShader&&) = delete;
				const GLShader& operator = (GLShader&&) = delete;
				const GLShader& operator = (const GLShader&) = delete;

				GLShader();
				GLShader(const std::string& frag, std::string vert = "default");
				~GLShader();
				void Load(const std::string& frag, std::string vert = "default");
				inline operator GLuint() const { return m_program; }
				inline void Use() { glUseProgram(m_program); }
				inline static void Unuse() { glUseProgram(0); }
				inline GLuint UniformLocation(const char* var) {
					return glGetUniformLocation(m_program, var);
				}
				inline GLuint AttributeLocation(const char* var) {
					return glGetAttribLocation(m_program, var);
				}
				void Clear();
			};
		}
	}
}