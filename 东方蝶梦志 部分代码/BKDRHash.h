#pragma once
#include <string>

namespace Basic
{
	template<typename CharType>
	uintptr_t BKDRHash(const std::basic_string<CharType>& str)
	{
		constexpr unsigned int seed = 131;
		uint64_t hash = 0;
		for (size_t i = 0; i < str.length(); i++)
			hash = hash * seed + str[i];
		return hash % 0xFFFFFFFF;
	}

	template<size_t StrLen, typename CharType>
	constexpr uintptr_t BKDRHash(const CharType(&str)[StrLen])
	{
		constexpr unsigned int seed = 131;
		uint64_t hash = 0;
		for (size_t i = 0; i < StrLen - 1; i++)
			hash = hash * seed + str[i];
		return hash % 0xFFFFFFFF;
	}

}