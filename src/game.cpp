#include "game.hpp"

SDL_Texture *Game::mapTexture, *Game::tile, *Game::wall, *Game::bullet;
SDL_Renderer *Game::renderer;
SDL_Rect Game::mapRect;
bool Game::isServer, Game::shoot;
Game::Player Game::me, Game::opponent;
int Game::newBulletID;
unordered_map<int, Game::Bullet *> Game::bullets;

void Game::renderInit(SDL_Renderer *sourceRenderer)
{
	renderer = sourceRenderer;
	if ((isServer ? Map::sendMap() : Map::recvMap()) != 0)
		return;
	mapRect.x = 0;
	mapRect.y = 0;
	mapRect.w = WINDOW_WIDTH;
	mapRect.h = WINDOW_HEIGHT - (OFFSET + GAP);
	initTextures();
	mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TILE_SIZE * MAP_SIZE, TILE_SIZE * MAP_SIZE);
	SDL_SetRenderTarget(renderer, mapTexture);
	SDL_Rect rect;
	rect.h = rect.w = TILE_SIZE;

	for (int i = 0; i < MAP_SIZE; ++i)
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			rect.x = TILE_SIZE * i;
			rect.y = TILE_SIZE * j;
			if (Map::map[i][j] == 0)
				SDL_RenderCopy(renderer, tile, NULL, &rect);
			else if (Map::map[i][j] == 1)
				SDL_RenderCopy(renderer, wall, NULL, &rect);
			else if (Map::map[i][j] == 2)
				SDL_RenderCopy(renderer, me.base, NULL, &rect);
			else if (Map::map[i][j] == 3)
				SDL_RenderCopy(renderer, opponent.base, NULL, &rect);
		}
	SDL_SetRenderTarget(renderer, NULL);

	me.dir = opponent.dir = 0;
	me.pos.w = me.pos.h = me.pos.x = me.pos.y = TILE_SIZE;
	opponent.pos.w = opponent.pos.h = TILE_SIZE;
	opponent.pos.x = opponent.pos.y = TILE_SIZE * (MAP_SIZE - 2);
	if (!isServer)
		swap(me, opponent);
}

// @TODO: home bases need to be changed to look contrasting
void Game::initTextures()
{
	SDL_Surface *surface;

	surface = IMG_Load((Theme::themeSource + "tile.jpg").c_str());
	tile = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((Theme::themeSource + "player1.jpg").c_str());
	me.texture = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((Theme::themeSource + "player2.jpg").c_str());
	opponent.texture = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((Theme::themeSource + "wall.jpg").c_str());
	wall = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((Theme::themeSource + "home1.jpg").c_str());
	me.base = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((Theme::themeSource + "home2.jpg").c_str());
	opponent.base = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((Theme::themeSource + "bullet.jpg").c_str());
	bullet = SDL_CreateTextureFromSurface(renderer, surface);
}

void Game::loopGame()
{
	SDL_Event e;
	while (true)
	{
		shoot = false;
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				return;
			else if (e.type == SDL_KEYDOWN)
				handleKeyEvents(e.key.keysym.sym);
		if (sendInfo() != 0)
			return;
		if (recvInfo() != 0)
			return;
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, mapTexture, NULL, &mapRect);
		SDL_RenderCopyEx(renderer, opponent.texture, NULL, &opponent.pos, opponent.dir * 90, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(renderer, me.texture, NULL, &me.pos, me.dir * 90, NULL, SDL_FLIP_NONE);
		displayBullets();
		SDL_RenderPresent(renderer);
		SDL_Delay(DELAY);
	}
}

void Game::handleKeyEvents(SDL_Keycode key)
{
	if (key == SDLK_SPACE)
	{
		bullets[newBulletID] = new Bullet(me.pos.x, me.pos.y, me.dir);
		shoot = true;
	}
	else if (key == SDLK_UP || key == SDLK_w)
	{
		me.dir = 0;
		if (Map::map[me.pos.x / TILE_SIZE][me.pos.y / TILE_SIZE - 1] != 1)
			me.pos.y -= TILE_SIZE;
	}
	else if (key == SDLK_RIGHT || key == SDLK_d)
	{
		me.dir = 1;
		if (Map::map[me.pos.x / TILE_SIZE + 1][me.pos.y / TILE_SIZE] != 1)
			me.pos.x += TILE_SIZE;
	}
	else if (key == SDLK_DOWN || key == SDLK_s)
	{
		me.dir = 2;
		if (Map::map[me.pos.x / TILE_SIZE][me.pos.y / TILE_SIZE + 1] != 1)
			me.pos.y += TILE_SIZE;
	}
	else if (key == SDLK_LEFT || key == SDLK_a)
	{
		me.dir = 3;
		if (Map::map[me.pos.x / TILE_SIZE - 1][me.pos.y / TILE_SIZE] != 1)
			me.pos.x -= TILE_SIZE;
	}
}

int Game::recvInfo()
{
	char info[MSG_SIZE];
	if (Network::recvRequest(info, MSG_SIZE) == 4)
		return 4;
	opponent.pos.x = info[0] * TILE_SIZE;
	opponent.pos.y = info[1] * TILE_SIZE;
	opponent.dir = info[2];
	if (info[3] == 1)
		bullets[newBulletID++] = new Bullet(info[4] * TILE_SIZE, info[5] * TILE_SIZE, info[6]);
	return 0;
}

int Game::sendInfo()
{
	char info[MSG_SIZE] = {0};
	info[0] = me.pos.x / TILE_SIZE;
	info[1] = me.pos.y / TILE_SIZE;
	info[2] = me.dir;
	if (shoot)
	{
		info[3] = 1;
		info[4] = bullets[newBulletID]->pos.x / TILE_SIZE;
		info[5] = bullets[newBulletID]->pos.y / TILE_SIZE;
		info[6] = bullets[newBulletID++]->dir;
	}
	return Network::sendRequest(info, MSG_SIZE);
}

void Game::displayBullets()
{
	for (auto it = bullets.begin(); it != bullets.end();)
	{
		it->second->updatePos();
		if (Map::map[it->second->pos.x / TILE_SIZE][it->second->pos.y / TILE_SIZE] == 1)
			bullets.erase(it++);
		else
			SDL_RenderCopyEx(renderer, bullet, NULL, &it->second->pos, it->second->dir * 90, NULL, SDL_FLIP_NONE), it++;
	}
}
