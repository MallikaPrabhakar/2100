#ifndef FONT_H
#define FONT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "theme.hpp"

#define FONTSCOUNT 3
#define FONT_LIGHT "../assets/fonts/Roboto-Light.ttf"
#define FONT_MEDIUM "../assets/fonts/Roboto-Medium.ttf"
#define FONT_BOLD "../assets/fonts/Roboto-Bold.ttf"

struct Fonts
{
	static TTF_Font *fonts[];

	static int initFonts();

	static void displayText(SDL_Renderer *renderer, const char *text, int fontNum, int x, int y, SDL_Color color = Theme::textColor);
};

#endif
