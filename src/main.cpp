#include <SDL.h>
#include <SDL_ttf.h>
#include "constants.hpp"
#include "font.hpp"
#include "theme.hpp"

int main()
{
	SDL_Window *window = SDL_CreateWindow("Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
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

	TTF_Init();
	TTF_Font *font = TTF_OpenFont(FONT_MEDIUM, 24);
	if (font == NULL)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		printf("Could not load font, error encountered: %s\n", SDL_GetError());
		return 1;
	}
	theme::setTheme(1);
	char str[] = "hello world!";
	SDL_Event e;
	bool quit = false;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();
				quit = true;
			}
		}

		SDL_RenderClear(renderer);
		displayText(renderer, str, font, 240, 200);
		SDL_RenderPresent(renderer);
	}
}
