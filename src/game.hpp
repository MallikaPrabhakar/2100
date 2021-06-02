#ifndef GAME_H
#define GAME_H

#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include "object.hpp"
#include "map.hpp"
#include "theme.hpp"
#include "networking.hpp"
#include "menu.hpp"

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 1010
#define OFFSET 25
#define MSG_SIZE 5
#define RELOAD 50
#define MAX_SPAWN 5
#define SPAWN_PROB 0.4

using namespace std;

struct Game
{
	static SDL_Texture *mapTexture, *tile, *wall, *bullet, *bomb, *health, *flag, *home1, *home2;
	static SDL_Renderer *renderer;
	static SDL_Rect mapRect;
	static bool isServer;
	static Player me, opponent;
	static int reloadTime;

	static int renderInit();
	static void initTextures(SDL_Renderer *sourceRenderer);
	static void genMapTexture(SDL_Texture *texture, int size = TILE_SIZE, int x = 0, int y = 0);
	static void loopGame();
	static void handleKeyEvents(SDL_Keycode key);
	static int recvPlayerInfo();
	static int sendPlayerInfo();
	static bool updateBullets();
	static int updateAndSendSpawnables();
	static pair<int, int> spawnObject(int healthDelta);
	static int recvSpawnInfo();
	static void displayHealthBars();
	static void displayFlagsCount();
	static void displayReloadTime();
	static void finish();
};

#endif
