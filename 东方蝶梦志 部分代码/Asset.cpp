#include "NuzPrecompiled.h"
#include "Asset.h"
#include "Globals.h"
#include "GetPath.h"
using namespace std;
using namespace Basic;



void Crash(const std::wstring & s)
{
	MessageBoxW(nullptr, s.c_str(), L"Error", MB_OK | MB_ICONERROR);
	quick_exit(-1);
}

Asset::Asset(const std::string& path)
{
	string f = "../assets/" + path;
	string f2 = "assets/" + path;
	ifstream in(SaveSystem::GetPath().string() + "/Update/" + path, ios::binary);
	if (!in.good())
		in.open(f, ios::binary);
	if (!in.good())
		in.open(f2, ios::binary);
	if (!in.good())
		in.open(path, std::ios::binary);
	if(!in.good())
		throw FileNotFound(path.c_str());
	m_buf.reset();
	m_buf = std::make_shared<std::vector<uint8_t> >();
	
	//取得大小
	in.seekg(0, ios::end);
	auto size = in.tellg();
	m_buf->resize(uint32_t(size) + 1);
	
	//读取数据
	in.seekg(0, ios::beg);
	in.read((char*)&(*m_buf)[0],size);
	in.close();

	m_buf->back() = '\0';

#ifndef _RV_COMPILER_
	Log("Load File:%s", path.c_str());
#endif
}

