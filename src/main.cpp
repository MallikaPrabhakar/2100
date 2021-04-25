#include <SDL.h>
#include <SDL_ttf.h>
#include "constants.hpp"
#include "font.hpp"
#include "theme.hpp"
#include "menu.hpp"

void handleExit(SDL_Renderer *renderer, SDL_Window *window)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);
}

int main()
{
	SDL_Window *window = SDL_CreateWindow("Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 960, 0);
	if (window == NULL)
	{
		printf("Could not create window, error encountered: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		printf("Could not create renderer, error encountered: %s\n", SDL_GetError());
		return 1;
	}

	int fonts = Fonts::initFonts();
	if (fonts != 0)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		printf("Could not load font %d, error encountered: %s\n", fonts, SDL_GetError());
		return 1;
	}
	Theme::setTheme(1, renderer);
	int player = whichPlayer(renderer);
	if (player == -1)
		handleExit(renderer, window);
	if (player == 1)
	{
		int ret = serverMenu(renderer);
		if (ret == -1)
			handleExit(renderer, window);
	}
	else if (player == 2)
		clientMenu(renderer);
}
