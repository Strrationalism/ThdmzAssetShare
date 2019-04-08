
#include "Pager.h"

void PageWords(const std::set<wchar_t>& s, std::vector<Page>& output, TTF_Font* fnt)
{
	output.emplace_back(Page());
	int x = 0, y = 4;
	for (auto ch : s)
	{
		Word word;
		word.ch = ch;
		wchar_t str[2] = { ch,L'0' };
		TTF_SizeUNICODE(fnt, reinterpret_cast<Uint16*>(str), &word.w, &word.h);

		word.w /= 2;

		word.w += 4;
		word.h += 3;

		

		//»»ÐÐÅÐ¶¨
		if (word.w + x >= 2048) 
		{
			x = 0;

			//»»Ò³ÅÐ¶¨
			if (word.h + y >= 2048)
			{
				y = 0;
				output.emplace_back(Page());
			}
			else {
				y += word.h;
				y += 4;
			}
		}

		word.x = x;
		x += word.w;
		word.y = y;
		output.back().words.push_back(word);
	}

	for (auto& page : output)
	{
		page.w = 0, page.h = 0;
		for (const auto& word : page.words)
		{
			if (word.x + word.w > page.w) page.w = word.x + word.w;
			if (word.y + word.h > page.h) page.h = word.y + word.h;
		}
	}
}
