#include "theme.hpp"

SDL_Color Theme::textColor, Theme::backgroundColor;
string Theme::themeSource;

void Theme::setTheme(int themeNumber, SDL_Renderer *renderer)
{
	switch (themeNumber)
	{
	case 1:
		textColor = THEME1_FONT;
		backgroundColor = THEME1_BACKGROUND;
		themeSource = THEME1;
		break;

	case 2:
		textColor = THEME2_FONT;
		backgroundColor = THEME2_BACKGROUND;
		themeSource = THEME2;
		break;

	default:
		return;
	}
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	SDL_RenderPresent(renderer);
}
