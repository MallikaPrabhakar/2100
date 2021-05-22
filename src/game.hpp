#ifndef GAME_H
#define GAME_H

#include <unordered_set>
#include <algorithm>
#include <SDL.h>
#include <SDL_image.h>
#include "map.hpp"
#include "theme.hpp"
#include "networking.hpp"
#include "menu.hpp"
#include <iostream>

#define WINDOW_WIDTH 950
#define WINDOW_HEIGHT 1010
#define OFFSET 50
#define GAP 10
#define TILE_SIZE 38
#define MSG_SIZE 100
#define DELAY 20
#define RELOAD 10
#define MAX_HEALTH 5

struct Game
{
	struct Object
	{
		SDL_Texture *texture;
		SDL_Rect pos;
		int dir;

		Object(int dir, SDL_Texture *texture);

		void updatePos();
		void renderObject();
	};

	struct Bullet : Object
	{
		Bullet(int x, int y, int dir, SDL_Texture *texture);
	};

	struct Player : Object
	{
		int health, flags;

		Player(int dir);
	};

	struct Spawn : Object
	{
		int healthDelta;

		Spawn(int x, int y, int affects, SDL_Texture *texture);
	};

	static SDL_Texture *mapTexture, *tile, *wall, *bullet, *bomb, *health, *flag, *home1, *home2;
	static SDL_Renderer *renderer;
	static SDL_Rect mapRect;
	static bool isServer;
	static Player me, opponent;
	static int flagsOnMap, bombsOnMap, healthsOnMap, reloadTime;
	static unordered_set<Bullet *> bullets;

	static int renderInit(SDL_Renderer *sourceRenderer);
	static void initTextures();
	static void loopGame();
	static void handleKeyEvents(SDL_Keycode key);
	static int recvPlayerInfo();
	static int sendPlayerInfo();
	static void displayBullets();
	static void updateSpawnables();
	static void handleCollisions();
	static bool checkEnded();
};

#endif
