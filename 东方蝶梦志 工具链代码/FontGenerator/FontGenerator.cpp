#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include "Pager.h"
#include "PageRender.h"
#include "Shadower.h"
#include "CtxEncoder.h"
using namespace std;

int main(int argc,char** argv)
{
	if (argc < 4)
	{
		cout << "FontGenerator txtFile ttfFile fntOut ctxOut" << endl;
		return -1;
	}

	const string fntOut(argv[3]);
	const string ctxOut(argv[4]);

	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	TTF_Font* const font = TTF_OpenFont(argv[2], 32);

	std::vector<Page> pages;
	{
		set<wchar_t> words;
		{
			auto p = System::IO::File::ReadAllText(gcnew System::String(argv[1]));
			for (int i = 0; i < p->Length; ++i)
			{
				const wchar_t ch = p[i];
				words.insert(ch);
			}
		}
		PageWords(words, pages,font);
	}

	std::vector<SDL_Surface*> pageSurs,shadowSurs;
	for (auto& page : pages)
		pageSurs.push_back(RenderPage(page, font));
	
	for (auto pageSur : pageSurs)
	{
		shadowSurs.push_back(GetShadow(pageSur));
	}

	std::vector<CharInfo> infos;
	for (int i = 0; i < pageSurs.size(); ++i)
	{
		auto sur = pageSurs[i];
		auto shadow = shadowSurs[i];
		SDL_SetSurfaceBlendMode(sur, SDL_BLENDMODE_BLEND);
		SDL_SetSurfaceBlendMode(shadow, SDL_BLENDMODE_BLEND);

		SDL_Rect srcdst = { 0,0,sur->w,sur->h };

		//SDL_SaveBMP(shadow, "DEBUG.BMP");
		SDL_BlitSurface(sur, nullptr, shadow, &srcdst);
		
		SaveToCtx(shadow, pages[i], i, ctxOut + "_" + std::to_string(i) + ".ctx", infos);

		SDL_FreeSurface(sur);
		SDL_FreeSurface(shadow);

		
	}
	pageSurs.clear();

	ofstream fnt(fntOut.c_str(), ios::binary);
	fnt.write((char*)infos.data(), sizeof(CharInfo)*infos.size());
	fnt.close();
	
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();
    return 0;
}

