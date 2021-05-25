#include "game.hpp"

SDL_Texture *Game::mapTexture, *Game::tile, *Game::wall, *Game::bullet, *Game::bomb, *Game::health, *Game::flag, *Game::home1, *Game::home2;
SDL_Renderer *Game::renderer;
SDL_Rect Game::mapRect;
bool Game::isServer;
Game::Player Game::me(2), Game::opponent(0);
int Game::flagsOnMap, Game::bombsOnMap, Game::healthsOnMap, Game::reloadTime;
unordered_set<Game::Bullet *> Game::bullets;

Game::Object::Object(int dir, SDL_Texture *texture)
{
	this->texture = texture;
	this->dir = dir;
}

Game::Player::Player(int dir) : Object(dir, NULL)
{
	pos.w = pos.h = TILE_SIZE;
	flags = 0;
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

int Game::renderInit(SDL_Renderer *sourceRenderer)
{
	renderer = sourceRenderer;
	if ((isServer ? Map::sendMap() : Map::recvMap()) != 0)
	{
		Menu::exitLines = {"UNABLE TO SEND/RECEIVE MAP"};
		return -1;
	}

	initTextures();
	mapRect.x = 0;
	mapRect.y = 0;
	mapRect.w = WINDOW_WIDTH;
	mapRect.h = WINDOW_HEIGHT - (OFFSET + GAP);
	mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TILE_SIZE * MAP_SIZE, TILE_SIZE * MAP_SIZE);
	SDL_SetRenderTarget(renderer, mapTexture);
	SDL_Rect rect;
	rect.h = rect.w = TILE_SIZE;
	for (int i = 0; i < MAP_SIZE; ++i)
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			rect.x = TILE_SIZE * i;
			rect.y = TILE_SIZE * j;

			if (i == 1 && j == 1)
				SDL_RenderCopy(renderer, home1, NULL, &rect);
			else if (i == 23 && j == 23)
				SDL_RenderCopy(renderer, home2, NULL, &rect);
			else if (Map::map[i][j] == 0)
				SDL_RenderCopy(renderer, tile, NULL, &rect);
			else if (Map::map[i][j] == 1)
				SDL_RenderCopy(renderer, wall, NULL, &rect);
		}
	SDL_SetRenderTarget(renderer, NULL);

	vector<SDL_Texture *> tempVector = {mapTexture, tile, wall, bullet, bomb, health, flag, home1, home2, me.texture, opponent.texture};
	if (any_of(tempVector.begin(), tempVector.end(), [](SDL_Texture *texture) { return texture == NULL; }))
	{
		Menu::exitLines = {"UNABLE TO LOAD MAP TEXTURES", "PLEASE CHECK THAT THE FOLDER STRUCTURE IS UNCHANGED"};
		return -1;
	}
	me.pos.x = me.pos.y = TILE_SIZE;
	opponent.pos.x = opponent.pos.y = TILE_SIZE * (MAP_SIZE - 2);
	if (!isServer)
		swap(me, opponent);
	return 0;
}

void Game::initTextures()
{
	SDL_Surface *surface;

	//normal tiles- movable space
	surface = IMG_Load((Theme::themeSource + "tile.tif").c_str());
	tile = SDL_CreateTextureFromSurface(renderer, surface);

	//player1 (me is player 1)
	surface = IMG_Load((Theme::themeSource + "player1.tif").c_str());
	me.texture = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((Theme::themeSource + "home1.tif").c_str());
	home1 = SDL_CreateTextureFromSurface(renderer, surface);

	//player 2 (opponent is player 2)
	surface = IMG_Load((Theme::themeSource + "player2.tif").c_str());
	opponent.texture = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load((Theme::themeSource + "home2.tif").c_str());
	home2 = SDL_CreateTextureFromSurface(renderer, surface);

	//wall- barrier
	surface = IMG_Load((Theme::themeSource + "wall.tif").c_str());
	wall = SDL_CreateTextureFromSurface(renderer, surface);

	//bullet- @TODO currently pi/2 radian rotated
	surface = IMG_Load((Theme::themeSource + "bullet.tif").c_str());
	bullet = SDL_CreateTextureFromSurface(renderer, surface);

	//bomb- drastic health decreaser
	surface = IMG_Load((Theme::themeSource + "bomb.tif").c_str());
	bomb = SDL_CreateTextureFromSurface(renderer, surface);

	//health- health increaser
	surface = IMG_Load((Theme::themeSource + "health.tif").c_str());
	health = SDL_CreateTextureFromSurface(renderer, surface);

	//flag- for point calculation and winning rules
	surface = IMG_Load((Theme::themeSource + "flag.tif").c_str());
	flag = SDL_CreateTextureFromSurface(renderer, surface);
}

void Game::loopGame()
{
	// initSpawnables();
	SDL_Event e;
	while (true)
	{
		if (reloadTime)
			--reloadTime;
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
			{
				Menu::exitLines = {"YOU LEFT THE GAME"};
				return;
			}
			else if (e.type == SDL_KEYDOWN)
				handleKeyEvents(e.key.keysym.sym);
		}

		if (sendPlayerInfo() != 0)
		{
			Menu::exitLines = {string("ERROR IN SENDING INFORMATION TO ") + (isServer ? "CLIENT" : "SERVER")};
			return;
		}
		if (recvPlayerInfo() != 0)
		{
			Menu::exitLines = {string(isServer ? "CLIENT " : "SERVER ") + "DISCONNECTED FROM THE GAME"};
			return;
		}
		bool endGame = me.updateFlag() || me.updateHealth() || opponent.updateFlag() || opponent.updateHealth() || updateBulletPos();
		// if (isServer)
		// 	updateSpawnables();
		// if (sendSpawnInfo() != 0)
		// 	return;
		// if (recvSpawnInfo() != 0)
		// 	return;

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, mapTexture, NULL, &mapRect);
		opponent.renderObject();
		me.renderObject();
		displayBullets();
		SDL_RenderPresent(renderer);
		SDL_Delay(DELAY);

		if (endGame)
		{
			finish();
			return;
		}
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
		else if (Map::map[me.pos.x / TILE_SIZE][me.pos.y / TILE_SIZE - 1] <= 0)
			me.pos.y -= TILE_SIZE;
	}
	else if (key == SDLK_RIGHT || key == SDLK_d)
	{
		if (me.dir != 1)
			me.dir = 1;
		else if (Map::map[me.pos.x / TILE_SIZE + 1][me.pos.y / TILE_SIZE] <= 0)
			me.pos.x += TILE_SIZE;
	}
	else if (key == SDLK_DOWN || key == SDLK_s)
	{
		if (me.dir != 2)
			me.dir = 2;
		else if (Map::map[me.pos.x / TILE_SIZE][me.pos.y / TILE_SIZE + 1] <= 0)
			me.pos.y += TILE_SIZE;
	}
	else if (key == SDLK_LEFT || key == SDLK_a)
	{
		if (me.dir != 3)
			me.dir = 3;
		else if (Map::map[me.pos.x / TILE_SIZE - 1][me.pos.y / TILE_SIZE] <= 0)
			me.pos.x -= TILE_SIZE;
	}
}

int Game::recvPlayerInfo()
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

int Game::sendPlayerInfo()
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
		if (Map::map[(*it).pos.x / TILE_SIZE][(*it).pos.y / TILE_SIZE] == 1)
			bullets.erase(it++);
		else
			(**it).renderObject(), it++;
	}
}

void Game::updateSpawnables()
{
}

bool Game::updateBulletPos()
{
	bool ret = false;
	for (auto it = bullets.begin(); it != bullets.end();)
	{
		(**it).updatePos();
		pair<int, int> p = {(*it).pos.x, (*it).pos.y};
		if (p == make_pair(me.pos.x / TILE_SIZE, me.pos.y / TILE_SIZE))
			ret = true, --me.health;
		if (p == make_pair(opponent.pos.x / TILE_SIZE, opponent.pos.y / TILE_SIZE))
			ret = true, --opponent.health;
	}
	return ret;
}

bool Game::Player::updateHealth()
{
	int n = Map::map[pos.x / TILE_SIZE][pos.y / TILE_SIZE];
	if (n == -1)
	{
		health += 2;
		if (health > MAX_HEALTH)
			health = MAX_HEALTH;
	}
	else
	{
		health -= 2;
		if (health <= 0)
		{
			health = 0;
			return true;
		}
	}
	return false;
}

bool Game::Player::updateFlag()
{
	flags++;
	if (flags == FLAG_LIMIT)
		return true;
	return false;
}

// @TODO: enhance the messages
void Game::finish()
{
	bool winner;

	if (me.health == 0)
	{
		Menu::exitLines = {"THE GAME ENDED!, YOU LOST"};
	}
	else
	{
		Menu::exitLines = {"THE GAME ENDED!, YOU WON"};
	}
}

/*
	@TODO:
	1. health -1, bomb -2, flags -3
	2. collisions - 1. + bullets
	3. opponent == wall
	4. display bars
	5. gameEnd function
	6. appropriate delays on capture/death/end
	7. check if possible for players to move to same pos at same time
*/