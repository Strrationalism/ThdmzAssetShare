#include "NuzPrecompiled.h"
#include "Globals.h"
#include <Windows.h>
using namespace std;


Basic::Video::Renderer2D::CtxFont & GetCtxFont1()
{
	static Basic::Video::Renderer2D::CtxFont fnt1("Font1.fnt", "Font1");
	return fnt1;
}

std::wstring Str2WStr(const std::string & s)
{
	//static const char* GBK_LOCALE_NAME = ".936";
	//static std::wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> cv1(new std::codecvt_byname<wchar_t, char, mbstate_t>(GBK_LOCALE_NAME));
	//return cv1.from_bytes(s);
	static wchar_t wch[4096];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s.c_str(), -1, wch, 4096);
	return wch;
}

