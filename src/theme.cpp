#include "theme.hpp"

SDL_Color Theme::textColor, Theme::backgroundColor;
std::string Theme::themeSource;
SDL_Surface *Theme::surface;
SDL_Renderer *Theme::renderer;
SDL_Texture *Theme::backgroundTexture;

void Theme::setTheme(int themeNumber, SDL_Renderer *srcRenderer)
{
	renderer = srcRenderer;

	switch (themeNumber)
	{
	case 1:
		textColor = THEME1_FONT;
		backgroundColor = THEME1_ANTIFONT;
		surface = IMG_Load(THEME1_BACKGROUND);
		themeSource = THEME1;
		break;

	case 2:
		textColor = THEME2_FONT;
		backgroundColor = THEME2_ANTIFONT;
		surface = IMG_Load(THEME2_BACKGROUND);
		themeSource = THEME2;
		break;

	case 3:
		textColor = THEME3_FONT;
		backgroundColor = THEME3_ANTIFONT;
		surface = IMG_Load(THEME3_BACKGROUND);
		themeSource = THEME3;
		break;

	default:
		return;
	}

	backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);

	/*
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
	SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	/*SDL_RenderPresent(renderer);
	*/
}
