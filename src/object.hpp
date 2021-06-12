#ifndef OBJECT_H
#define OBJECT_H

#include <unordered_set>
#include <unordered_map>
#include <boost/container_hash/hash.hpp>
#include <SDL.h>
#include "sound.hpp"

#define TILE_SIZE 36
#define MAX_HEALTH 10
#define FLAG_LIMIT 10

using namespace std;

template <typename A, typename B>
struct hash<typename std::pair<A, B>>
{
	size_t operator()(const typename std::pair<A, B> &p) const { return boost::hash_value(p); }
};

struct Object
{
	static SDL_Renderer *renderer;

	SDL_Texture *texture;
	SDL_Rect pos;
	int dir;

	Object(int dir, SDL_Texture *texture);

	void updatePos();
	void renderObject();
};

struct Bullet : Object
{
	static unordered_set<Bullet *> bullets;

	Bullet(int x, int y, int dir, SDL_Texture *texture);

	static void displayBullets();
};

struct Player : Object
{
	int health, flags;

	Player(int dir);

	bool updateHealthAndFlags();
};

struct Spawnable : Object
{
	static int flagsOnMap, healthsOnMap;
	static unordered_map<pair<int, int>, Spawnable *> spawnables;

	int healthDelta;

	Spawnable(int x, int y, int effect, SDL_Texture *texture);

	static void displaySpawnables();
};

#endif
