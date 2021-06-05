#include <SDL.h>
#include <SDL_ttf.h>
#include "font.hpp"
#include "theme.hpp"
#include "networking.hpp"
#include "menu.hpp"
#include "map.hpp"
#include "game.hpp"
#include "intro.hpp"

void handleExit(SDL_Renderer *renderer, SDL_Window *window)
{
	close(Network::othersockfd);
	close(Network::sockfd);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(0);
}

int main()
{
	srand(time(NULL));
	SDL_Window *window = SDL_CreateWindow("Capture the Flag!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
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
	// @SOUND play some calming/apocalyptic mild sound for the entire game
	Intro::initIntro(renderer);
	//for starting page
	while (true)
	{
		if (Intro::displayStartingPage() == 0)
		{
			break;
		}
		if (Menu::exitMenu("START SCREEN") == -1)
			handleExit(renderer, window);
	}
	//for plot
	while (true)
	{
		if (Intro::displayPlot() == 0)
		{
			break;
		}
		if (Menu::exitMenu("STORY") == -1)
			handleExit(renderer, window);
	}

	//for rules
	while (true)
	{
		if (Intro::displayRules() == 0)
		{
			break;
		}
		if (Menu::exitMenu("RULES") == -1)
			handleExit(renderer, window);
	}

	//for choosing player
	while (true)
	{
		int player = Menu::whichPlayer();
		if (player == -1)
			if (Menu::exitMenu("PLAYER SELECTION SCREEN") == -1)
				handleExit(renderer, window);
			else
				continue;

		if (player == 1)
		{
			Game::isServer = true;
			int ret = Network::makeServer();
			if (ret != 0)
			{
				printf(ret == 1 ? "Could not init socket\n" : "Could not bind socket\n");
				handleExit(renderer, window);
			}
		}
		else
			Game::isServer = false;
		break;
	}

	Game::initTextures(renderer);
	while (true)
	{
		if (Menu::menuLoop() == 0)
			if (Game::renderInit() == 0)
				Game::loopGame();
		if (Menu::exitMenu("MAIN MENU") == -1)
			handleExit(renderer, window);
	}
}
