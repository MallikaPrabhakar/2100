#include "theme.hpp"

SDL_Color theme::textColor, theme::backgroundColor;

void theme::setTheme(int themeNumber, SDL_Renderer *renderer)
{
	switch (themeNumber)
	{
	case 1:
		textColor = THEME1_FONT;
		backgroundColor = THEME1_BACKGROUND;
		break;
	
	case 2:
		textColor = THEME2_FONT;
		backgroundColor = THEME2_BACKGROUND;
		break;

	default:
		return;
	}
	SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
}
