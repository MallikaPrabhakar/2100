#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <SDL_image.h>
#include "map.hpp"
#include "theme.hpp"

#define TILE_SIZE 32
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960
#define HEALTH_HEIGHT 50
#define GAP 10

struct Game
{
	static SDL_Texture *mapTexture, *tile, *me, *opponent, *wall, *myBase, *oppBase;
	static SDL_Renderer *renderer;
	static SDL_Rect mapRect;
	static bool isServer;

	static void renderInit(SDL_Renderer *sourceRenderer, bool server);
	static void initTextures();
	static void loopGame();
};

#endif
