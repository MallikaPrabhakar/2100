#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "font.hpp"
#include "sound.hpp"
#include "theme.hpp"
#include "modes.hpp"
#include "menu.hpp"
#include "networking.hpp"
#include "map.hpp"
#include "game.hpp"

#define ICON "../assets/res/icon.tif"

using namespace std;

void handleExit(SDL_Renderer *renderer, SDL_Window *window)
{
	close(Network::othersockfd);
	close(Network::sockfd);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
	exit(0);
}

int main()
{
	srand(time(NULL));

	// init window
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	SDL_Window *window = SDL_CreateWindow("2100", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (window == NULL)
	{
		printf("Could not create window, error encountered: %s\n", SDL_GetError());
		return 1;
	}
	SDL_Surface *iconSurface = IMG_Load(ICON);
	SDL_SetWindowIcon(window, iconSurface);

	// init renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		printf("Could not create renderer, error encountered: %s\n", SDL_GetError());
		return 1;
	}
	SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

	// init fonts
	int fonts = Fonts::initFonts();
	if (fonts != 0)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		printf("Could not load font %d, error encountered: %s\n", fonts, SDL_GetError());
		return 1;
	}

	// init sounds
	if (Sound::initSounds() != 0)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		printf("Could not load sounds, error encountered: %s\n", SDL_GetError());
		return 1;
	}
	Sound::startMusic();

	// init theme
	Theme::setTheme(1, renderer);

	// init intro
	if (Modes::initModes(renderer) != 0)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		printf("Could not initialise loading screen, error encountered: %s\n", SDL_GetError());
		return 1;
	}

	// @TODO: delete menu and move everything to modes
	Menu::renderer = renderer;

	// init game textures
	if (Game::initTextures(renderer) != 0)
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		printf("Could not initialise game textures, error encountered: %s\n", SDL_GetError());
		return 1;
	}

	// display intro page
	while (true)
	{
		if (Modes::displayFrontPage() == 0)
			break;
		if (Menu::exitMenu("START SCREEN") == -1)
			handleExit(renderer, window);
	}

	// display story
	while (true)
	{
		if (Modes::displayStory() == 0)
			break;
		if (Menu::exitMenu("STORY") == -1)
			handleExit(renderer, window);
	}

	// display rules
	while (true)
	{
		if (Modes::displayRules() == 0)
			break;
		if (Menu::exitMenu("RULES") == -1)
			handleExit(renderer, window);
	}

	// choose player (1 or 2)
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
			Map::setMap();
		}
		else
			Game::isServer = false;
		break;
	}

	// menu + game + exit loop
	while (true)
	{
		if (Menu::menuLoop() == 0)
		{
			if (Game::renderInit() == 0)
				if (Menu::exitMenu("MAIN MENU", Game::loopGame()) == -1)
					handleExit(renderer, window);
				else
					continue;
			else if (Menu::exitMenu("MAIN MENU", "UNABLE TO SEND/RECEIVE MAP") == -1)
				handleExit(renderer, window);
		}
		else if (Menu::exitMenu("MAIN MENU") == -1)
			handleExit(renderer, window);
	}
}
