#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include "map.hpp"
#include "theme.hpp"
#include "networking.hpp"

#define WINDOW_WIDTH 950
#define WINDOW_HEIGHT 1010
#define OFFSET 50
#define GAP 10
#define TILE_SIZE 38
#define MSG_SIZE 100

struct Game
{
	static SDL_Texture *mapTexture, *tile, *me, *opponent, *wall, *myBase, *oppBase;
	static SDL_Renderer *renderer;
	static SDL_Rect mapRect, myPos, oppPos;
	static sockaddr_in *from;
	static bool isServer;

	static void renderInit(SDL_Renderer *sourceRenderer);
	static void initTextures();
	static void loopGame();
	static void handleKeyEvents(SDL_Keycode key);
	static void handleBullets();
};

#endif
