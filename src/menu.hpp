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
	static SDL_Renderer *renderer;
	static vector<string> serverMenuLines, clientMenuLines, lines;
	static int mode, key;

	static int whichPlayer();
	static int displayLines();
	static int serverMenu();
	static int clientMenu();
};

#endif
