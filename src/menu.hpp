#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "theme.hpp"
#include "font.hpp"
#include "map.hpp"
#include "networking.hpp"
#include "game.hpp"

using namespace std;

struct Menu
{
	enum modes
	{
		MAIN_MENU, THEME, MAP, CONNECT, LOOK, PLAY, QUIT = -1
	};

	static SDL_Renderer *renderer;
	static vector<string> serverMenuLines, clientMenuLines, exitLines, lines;

	static void displayLines();
	static int whichPlayer();
	static void handleMenuKeyEvents(int &mode, int key = -1);
	static int menuLoop();
	static int exitMenu();
};

#endif
