#ifndef GAME_H
#define GAME_H

#include <unordered_map>
#include <SDL.h>
#include <SDL_image.h>
#include "map.hpp"
#include "theme.hpp"
#include "networking.hpp"
#include <iostream>

#define WINDOW_WIDTH 950
#define WINDOW_HEIGHT 1010
#define OFFSET 50
#define GAP 10
#define TILE_SIZE 38
#define MSG_SIZE 100
#define DELAY 20

struct Game
{
	struct Player
	{
		SDL_Texture *texture, *base;
		SDL_Rect pos;
		int dir;
	};

	struct Bullet
	{
		SDL_Rect pos;
		int dir;

		Bullet(int x, int y, int dir)
		{
			pos.h = pos.w = TILE_SIZE;
			pos.x = x;
			pos.y = y;
			this->dir = dir;
		}

		void updatePos()
		{
			if (dir == 0)
				pos.y -= TILE_SIZE;
			else if (dir == 1)
				pos.x += TILE_SIZE;
			else if (dir == 2)
				pos.y += TILE_SIZE;
			else if (dir == 3)
				pos.x -= TILE_SIZE;
		}
	};

	static SDL_Texture *mapTexture, *tile, *wall, *bullet;
	static SDL_Renderer *renderer;
	static SDL_Rect mapRect;
	static bool isServer, shoot;
	static Player me, opponent;
	static int newBulletID;
	static unordered_map<int, Bullet*> bullets;

	static void renderInit(SDL_Renderer *sourceRenderer);
	static void initTextures();
	static void loopGame();
	static void handleKeyEvents(SDL_Keycode key);
	static int recvInfo();
	static int sendInfo();
	static void displayBullets();
};

#endif
