#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include "networking.hpp"
#include "theme.hpp"
#include "font.hpp"
#include "game.hpp"
#include "intro.hpp"
#include "sound.hpp"

#define PREVIEW_SIZE 16

using namespace std;

struct Menu
{
	enum modes
	{
		MAIN_MENU,
		THEME,
		MAP,
		CONNECT,
		LOOK,
		STORY,
		RULES,
		PLAY,
		QUIT = -1
	};

	static modes mode;
	static SDL_Renderer *renderer;
	static vector<string> serverMenuLines, clientMenuLines, lines;
	static SDL_Texture *mapTexture, *backgroundTexture;
	static SDL_Rect mapRect;

	static void displayLines();
	static int whichPlayer();
	static void handleMenuKeyEvents(int key = -1);
	static int menuLoop();
	static int exitMenu(string target, string exitMessage = "");
};

#endif
