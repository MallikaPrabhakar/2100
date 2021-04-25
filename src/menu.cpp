#include "menu.hpp"

int whichPlayer(SDL_Renderer *renderer)
{
	SDL_Event e;
	while (true)
	{
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				return 0;
			else if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_ESCAPE)
					return 0;
				if (e.key.keysym.sym == SDLK_1)
					return 1;
				if (e.key.keysym.sym == SDLK_2)
					return 2;
			}
		SDL_RenderClear(renderer);
		char text[] = "PLAYER 1 OR PLAYER 2?";
		displayText(renderer, text, Fonts::fonts[2], 500, 440);
		SDL_RenderPresent(renderer);
	}
}

void serverMenu(SDL_Renderer *renderer)
{
}

void clientMenu(SDL_Renderer *renderer)
{
}
