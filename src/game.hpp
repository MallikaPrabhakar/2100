#ifndef GAME_H
#define GAME_H

#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <boost/container_hash/hash.hpp>
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
#define MSG_SIZE 5
#define RELOAD 50
#define MAX_HEALTH 10
#define FLAG_LIMIT 10
#define MAX_SPAWN 5
#define SPAWN_PROB 0.4

template <typename A, typename B>
struct hash<typename std::pair<A, B>>
{
	size_t operator()(const typename std::pair<A, B> &p) const { return boost::hash_value(p); }
};

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

		bool updateHealthAndFlags();
	};

	struct Spawnable : Object
	{
		int healthDelta;

		Spawnable(int x, int y, int effect, SDL_Texture *texture);
	};

	static SDL_Texture *mapTexture, *tile, *wall, *bullet, *bomb, *health, *flag, *home1, *home2;
	static SDL_Renderer *renderer;
	static SDL_Rect mapRect;
	static bool isServer;
	static Player me, opponent;
	static int flagsOnMap, healthsOnMap, reloadTime;
	static unordered_set<Bullet *> bullets;
	static unordered_map<pair<int, int>, Spawnable *> spawnables;

	static int renderInit(SDL_Renderer *sourceRenderer);
	static void initTextures();
	static void loopGame();
	static void handleKeyEvents(SDL_Keycode key);
	static int recvPlayerInfo();
	static int sendPlayerInfo();
	static void displayBullets();
	static bool updateBullets();
	static int updateSpawnables();
	static void displaySpawnables();
	static pair<int, int> spawnObject(int healthDelta);
	static int recvSpawnInfo();
	static bool handleCollisions();
	static void finish();
};

#endif
