#include "NuzPrecompiled.h"
#include "UnicodeSite.h"
#include "Asset.h"


std::map<int, std::wstring> InitUnicodeText()
{
	std::map<int, std::wstring> texts;
	std::wstring lst((const wchar_t*)(const void*)Basic::Asset("UnicodeText.txt"));
	while (1) {
		auto pos = lst.find(L'\n');
		std::wstring lineFull = lst.substr(0, pos),line;
		lst = lst.substr(pos + 1, lst.length() - pos - 1);
		for (auto p : lineFull) {
			if (iswgraph(p)) line += p;
		}
		if (line.empty()) continue;
		else if (line == L"END") break;

		auto spos = line.find(L'|');
		int first = _wtoi(line.substr(0, spos).c_str());
		texts[first] = line.substr(spos + 1, line.length() - spos - 1);
	}

	return texts;
}

const std::wstring& Basic::UnicodeText(int i)
{
	static std::map<int, std::wstring> texts = InitUnicodeText();
	return texts.at(i);
}
