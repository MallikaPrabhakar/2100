#include "game.hpp"

SDL_Texture *Game::mapTexture, *Game::tile, *Game::wall, *Game::bullet, *Game::bomb, *Game::health, *Game::flag;
SDL_Renderer *Game::renderer;
SDL_Rect Game::mapRect;
bool Game::isServer;
Game::Player Game::me, Game::opponent;
int Game::flagsOnMap, Game::bombsOnMap, Game::healthsOnMap, Game::reloadTime;
unordered_set<Game::Bullet *> Game::bullets;

Game::Object::Object(int dir, SDL_Texture *texture)
{
	this->texture = texture;
	this->dir = dir;
}

Game::Player::Player() : Object(0, NULL)
{
	pos.w = pos.h = TILE_SIZE;
	dir = flags = 0;
	health = MAX_HEALTH;
}

Game::Bullet::Bullet(int x, int y, int dir, SDL_Texture *texture) : Object(dir, texture)
{
	pos.h = pos.w = TILE_SIZE;
	pos.x = x;
	pos.y = y;
}

Game::Spawn::Spawn(int x, int y, int affects, SDL_Texture *texture) : Object(0, texture)
{
	pos.h = pos.w = TILE_SIZE;
	pos.x = x;
	pos.y = y;
	healthDelta = affects;
}

void Game::Object::updatePos()
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

void Game::Object::renderObject()
{
	SDL_RenderCopyEx(renderer, texture, NULL, &pos, dir * 90, NULL, SDL_FLIP_NONE);
}

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
		}
	SDL_SetRenderTarget(renderer, NULL);

	me.pos.x = me.pos.y = TILE_SIZE;
	opponent.pos.x = opponent.pos.y = TILE_SIZE * (MAP_SIZE - 2);
	if (!isServer)
		swap(me, opponent);
}

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

	surface = IMG_Load((Theme::themeSource + "bullet.jpg").c_str());
	bullet = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((Theme::themeSource + "bomb.jpg").c_str());
	bomb = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((Theme::themeSource + "health.jpg").c_str());
	health = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((Theme::themeSource + "flag.jpg").c_str());
	flag = SDL_CreateTextureFromSurface(renderer, surface);
}

void Game::loopGame()
{
	SDL_Event e;
	while (true)
	{
		if (reloadTime)
			--reloadTime;
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				return;
			else if (e.type == SDL_KEYDOWN)
				handleKeyEvents(e.key.keysym.sym);
		}
		if (sendInfo() != 0)
			return;
		if (recvInfo() != 0)
			return;
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, mapTexture, NULL, &mapRect);
		opponent.renderObject();
		me.renderObject();
		displayBullets();
		SDL_RenderPresent(renderer);
		SDL_Delay(DELAY);
	}
}

void Game::handleKeyEvents(SDL_Keycode key)
{
	if (key == SDLK_SPACE && !reloadTime)
	{
		bullets.insert(new Bullet(me.pos.x, me.pos.y, me.dir, bullet));
		reloadTime = RELOAD;
	}
	else if (key == SDLK_UP || key == SDLK_w)
	{
		if (me.dir != 0)
			me.dir = 0;
		else if (Map::map[me.pos.x / TILE_SIZE][me.pos.y / TILE_SIZE - 1] != 1)
			me.pos.y -= TILE_SIZE;
	}
	else if (key == SDLK_RIGHT || key == SDLK_d)
	{
		if (me.dir != 1)
			me.dir = 1;
		else if (Map::map[me.pos.x / TILE_SIZE + 1][me.pos.y / TILE_SIZE] != 1)
			me.pos.x += TILE_SIZE;
	}
	else if (key == SDLK_DOWN || key == SDLK_s)
	{
		if (me.dir != 2)
			me.dir = 2;
		else if (Map::map[me.pos.x / TILE_SIZE][me.pos.y / TILE_SIZE + 1] != 1)
			me.pos.y += TILE_SIZE;
	}
	else if (key == SDLK_LEFT || key == SDLK_a)
	{
		if (me.dir != 3)
			me.dir = 3;
		else if (Map::map[me.pos.x / TILE_SIZE - 1][me.pos.y / TILE_SIZE] != 1)
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
		bullets.insert(new Bullet(opponent.pos.x, opponent.pos.y, opponent.dir, bullet));
	return 0;
}

int Game::sendInfo()
{
	char info[MSG_SIZE] = {0};
	info[0] = me.pos.x / TILE_SIZE;
	info[1] = me.pos.y / TILE_SIZE;
	info[2] = me.dir;
	if (reloadTime == RELOAD)
		info[3] = 1;
	return Network::sendRequest(info, MSG_SIZE);
}

void Game::displayBullets()
{
	for (auto it = bullets.begin(); it != bullets.end();)
	{
		(**it).updatePos();
		if (Map::map[(**it).pos.x / TILE_SIZE][(**it).pos.y / TILE_SIZE] == 1)
			bullets.erase(it++);
		else
			(**it).renderObject(), it++;
	}
}

void Game::updateSpawnables()
{
}

/*
	@TODO:
	1. health++, health--, flags
	2. collisions - 1. + bullets
	3. opponent == wall
	4. game end -> death/first to max flags
	5. maps
*/
