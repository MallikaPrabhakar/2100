#include "font.hpp"

TTF_Font *Fonts::fonts[FONTSCOUNT];

int Fonts::initFonts()
{
	TTF_Init();
	fonts[0] = TTF_OpenFont(FONT_LIGHT, 24);
	if (fonts[0] == NULL)
		return 1;
	fonts[1] = TTF_OpenFont(FONT_MEDIUM, 24);
	if (fonts[1] == NULL)
		return 2;
	fonts[2] = TTF_OpenFont(FONT_BOLD, 24);
	if (fonts[2] == NULL)
		return 3;
	return 0;
}

void Fonts::displayText(SDL_Renderer *renderer, char *text, int fontNum, int x, int y, SDL_Color color)
{
	SDL_Surface *surface = TTF_RenderText_Solid(fonts[fontNum], text, color);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	SDL_Rect pos;
	pos.w = surface->w;
	pos.h = surface->h;
	pos.x = x - pos.w / 2;
	pos.y = y - pos.h / 2;
	SDL_RenderCopy(renderer, texture, NULL, &pos);
}
