#include "NuzPrecompiled.h"
#include "GLCallList.h"
#include "Globals.h"
using namespace Basic::Video::LowerRenderer;

GLCallList::GLCallList()
{
	m_callList = glGenLists(1);
	if (m_callList == 0) Crash(L"无法创建显示列表。");
}


GLCallList::~GLCallList()
{
	glDeleteLists(m_callList, 1);
}
