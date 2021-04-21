#include "font.hpp"

void displayText(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y)
{
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, theme::textColor);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	SDL_Rect pos;
	pos.w = surface->w;
	pos.h = surface->h;
	pos.x = x;
	pos.y = y;
	SDL_RenderCopy(renderer, texture, NULL, &pos);
}
