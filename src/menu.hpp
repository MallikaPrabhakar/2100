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
#include "modes.hpp"
#include "sound.hpp"

using namespace std;

struct Menu
{
	static SDL_Renderer *renderer;
	static vector<string> serverMenuLines, clientMenuLines, lines;

	static void displayLines();
	static int whichPlayer();
	static void handleMenuKeyEvents(int key = -1);
	static int menuLoop();
	static int exitMenu(string target, string exitMessage = "");
};

#endif
