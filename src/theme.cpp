#include "theme.hpp"

SDL_Color Theme::textColor;
std::string Theme::themeSource;
SDL_Texture *Theme::backgroundTexture;

void Theme::setTheme(int themeNumber, SDL_Renderer *renderer)
{
	SDL_Surface *surface;

	switch (themeNumber)
	{
	case 1:
		textColor = THEME1_FONT;
		surface = IMG_Load(THEME1_BACKGROUND);
		themeSource = THEME1;
		break;

	case 2:
		textColor = THEME2_FONT;
		surface = IMG_Load(THEME2_BACKGROUND);
		themeSource = THEME2;
		break;

	case 3:
		textColor = THEME3_FONT;
		surface = IMG_Load(THEME3_BACKGROUND);
		themeSource = THEME3;
		break;

	default:
		return;
	}

	backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
}
