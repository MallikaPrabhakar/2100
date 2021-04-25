#ifndef THEME_H
#define THEME_H

#include <SDL.h>
#include "constants.hpp"

class theme
{
public:
	static SDL_Color textColor, backgroundColor;

	static void setTheme(int themeNumber, SDL_Renderer *renderer);
};

#endif
