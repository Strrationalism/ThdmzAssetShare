#include "PageRender.h"

SDL_Surface * RenderPage(Page & page, TTF_Font* fnt)
{
	auto sur = SDL_CreateRGBSurface(0, page.w, page.h, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);

	SDL_FillRect(sur, NULL, 0xFFFFFF00);

	for (auto ch : page.words)
	{
		wchar_t str[2] = { ch.ch,L'\0' };
		auto chSur = TTF_RenderUNICODE_Blended(fnt, reinterpret_cast<Uint16*>(str), SDL_Color{ 255,255,255,255 });
		SDL_Rect rect = { ch.x,ch.y,ch.w,ch.h };

		
		SDL_BlitSurface(chSur, nullptr, sur, &rect);
		SDL_FreeSurface(chSur);
	}

	return sur;
}
