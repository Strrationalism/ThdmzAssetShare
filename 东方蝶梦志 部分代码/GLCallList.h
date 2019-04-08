#pragma once
#include "glew.h"

namespace Basic {
	namespace Video {
		namespace LowerRenderer {
			class GLCallList final
			{
			private:
				GLuint m_callList;
			public:
				GLCallList();
				~GLCallList();

				GLCallList(const GLCallList&) = delete;
				GLCallList(GLCallList&&) = delete;
				const GLCallList& operator = (GLCallList&&) = delete;
				const GLCallList& operator = (const GLCallList&) = delete;

				inline void DefineBegin() { glNewList(m_callList, GL_COMPILE); }
				inline static void DefineEnd() { glEndList(); }
				inline void operator()() const { glCallList(m_callList); }
			};
		}
	}
}