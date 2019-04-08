#pragma once
#include <set>
#include <vector>
#include <SDL_ttf.h>

struct Word 
{
	int x, y, w, h;
	wchar_t ch;
};

struct Page
{
	int w, h;
	std::vector<Word> words;
};

void PageWords(const std::set<wchar_t>&, std::vector<Page>& output, TTF_Font* fnt);
