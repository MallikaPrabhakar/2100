#include <SDL.h>
#include <SDL_ttf.h>
#include "font.hpp"
#include "theme.hpp"
#include "networking.hpp"
#include "menu.hpp"
#include "map.hpp"
#include "game.hpp"

void handleExit(SDL_Renderer *renderer, SDL_Window *window)
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);
}

int main()
{
	SDL_Window *window = SDL_CreateWindow("Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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
	SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

	int fonts = Fonts::initFonts();
	if (fonts != 0)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		printf("Could not load font %d, error encountered: %s\n", fonts, SDL_GetError());
		return 1;
	}
	Theme::setTheme(1, renderer);
	Menu::renderer = renderer;
	// @TODO: insert while loop and exitMenu function in menu.?pp
	int player = Menu::whichPlayer();
	if (player == -1)
		handleExit(renderer, window);
	if (player == 1)
	{
		int ret = Network::makeServer();
		if (ret != 0)
		{
			printf(ret == 1 ? "Could not init socket\n" : "Could not bind socket\n");
			handleExit(renderer, window);
		}
		if (Menu::serverMenu() == -1)
			handleExit(renderer, window);
	}
	else if (player == 2)
		if (Menu::clientMenu() == -1)
			handleExit(renderer, window);
	Game::renderInit(renderer);
	Game::loopGame();
	handleExit(renderer, window);
}
