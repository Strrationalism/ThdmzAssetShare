#pragma once
#include <string>
#include <sstream>

void Log(const char* format,...);

namespace Log2Impls
{
	template<typename T, typename...TArgs>
	inline void Log2Impl(std::stringstream& s, T arg, TArgs...args);

	inline void Log2Impl(std::stringstream& s)
	{}

	template<typename T,typename ...TArgs>
	inline void Log2Impl(std::stringstream& s, T arg, TArgs...args)
	{
		s << arg << ' ';
		Log2Impl(s,args...);
	}


}

template<typename... TArgs>
void Log2(TArgs... t)
{
	std::stringstream str;
	Log2Impls::Log2Impl(str, t...);
	Log(str.str().c_str());
}