#ifndef FONT_H
#define FONT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "theme.hpp"

using namespace std;

struct Fonts
{
	static TTF_Font *fonts[];

	static int initFonts();
};
void displayText(SDL_Renderer *renderer, char *text, TTF_Font *font, int x, int y);

#endif
