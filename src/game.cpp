#include "game.hpp"

SDL_Texture *Game::mapTexture, *Game::tile, *Game::wall, *Game::bullet, *Game::bomb, *Game::health, *Game::flag, *Game::home1, *Game::home2;
SDL_Renderer *Game::renderer;
SDL_Rect Game::mapRect;
bool Game::isServer;
Player Game::me(2), Game::opponent(0);
int Game::reloadTime;

int Game::renderInit(SDL_Renderer *sourceRenderer)
{
	renderer = Object::renderer = sourceRenderer;
	if ((isServer ? Map::sendMap() : Map::recvMap()) != 0)
	{
		Menu::exitLines = {"UNABLE TO SEND/RECEIVE MAP"};
		return -1;
	}

	initTextures();
	mapRect = SDL_Rect({0, 0, WINDOW_WIDTH, WINDOW_WIDTH});
	mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TILE_SIZE * MAP_SIZE, TILE_SIZE * MAP_SIZE);
	SDL_SetRenderTarget(renderer, mapTexture);
	SDL_Rect rect = {0, 0, TILE_SIZE, TILE_SIZE};
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

	me.health = opponent.health = MAX_HEALTH;
	me.flags = opponent.flags = 0;
	me.pos.x = me.pos.y = TILE_SIZE;
	opponent.pos.x = opponent.pos.y = TILE_SIZE * (MAP_SIZE - 2);
	if (!isServer)
		swap(me, opponent);

	Spawnable::healthsOnMap = Spawnable::flagsOnMap = 0;
	Spawnable::spawnables.clear();

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

	//bullet
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
	srand(time(NULL));
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
		bool endGame = me.updateHealthAndFlags() || opponent.updateHealthAndFlags() || updateBullets();
		if (isServer && updateAndSendSpawnables() != 0)
		{
			Menu::exitLines = {string("ERROR IN SENDING INFORMATION TO CLIENT")};
			return;
		}
		else if (!isServer && recvSpawnInfo() != 0)
		{
			Menu::exitLines = {"SERVER DISCONNECTED FROM THE GAME"};
			return;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, mapTexture, NULL, &mapRect);
		Spawnable::displaySpawnables();
		opponent.renderObject();
		me.renderObject();
		Bullet::displayBullets();
		displayHealthBars();
		displayFlagsCount();
		displayReloadTime();
		SDL_RenderPresent(renderer);

		if (endGame)
		{
			finish();
			return;
		}
	}
}

void Game::handleKeyEvents(SDL_Keycode key)
{
	int x = me.pos.x / TILE_SIZE, y = me.pos.y / TILE_SIZE;
	pair<int, int> oppPos = {opponent.pos.x / TILE_SIZE, opponent.pos.y / TILE_SIZE};
	if (key == SDLK_SPACE && !reloadTime)
	{
		Bullet::bullets.insert(new Bullet(me.pos.x, me.pos.y, me.dir, bullet));
		reloadTime = RELOAD;
	}
	else if (key == SDLK_UP || key == SDLK_w)
	{
		if (me.dir != 0)
			me.dir = 0;
		else if (Map::map[x][y - 1] == 0 && make_pair(x, y - 1) != oppPos)
			me.updatePos();
	}
	else if (key == SDLK_RIGHT || key == SDLK_d)
	{
		if (me.dir != 1)
			me.dir = 1;
		else if (Map::map[x + 1][y] == 0 && make_pair(x + 1, y) != oppPos)
			me.updatePos();
	}
	else if (key == SDLK_DOWN || key == SDLK_s)
	{
		if (me.dir != 2)
			me.dir = 2;
		else if (Map::map[x][y + 1] == 0 && make_pair(x, y + 1) != oppPos)
			me.updatePos();
	}
	else if (key == SDLK_LEFT || key == SDLK_a)
	{
		if (me.dir != 3)
			me.dir = 3;
		else if (Map::map[x - 1][y] == 0 && make_pair(x - 1, y) != oppPos)
			me.updatePos();
	}
}

int Game::recvPlayerInfo()
{
	char info[MSG_SIZE] = {-1};
	if (Network::recvRequest(info, MSG_SIZE) == 4)
		return 4;
	if (info[0] != 0)
		return -1;
	opponent.pos.x = info[1] * TILE_SIZE;
	opponent.pos.y = info[2] * TILE_SIZE;
	opponent.dir = info[3];
	if (info[4] == 1)
		Bullet::bullets.insert(new Bullet(opponent.pos.x, opponent.pos.y, opponent.dir, bullet));
	return 0;
}

int Game::sendPlayerInfo()
{
	char info[MSG_SIZE] = {0};
	info[1] = me.pos.x / TILE_SIZE;
	info[2] = me.pos.y / TILE_SIZE;
	info[3] = me.dir;
	if (reloadTime == RELOAD)
		info[4] = 1;
	return Network::sendRequest(info, MSG_SIZE);
}

bool Game::updateBullets()
{
	for (auto it = Bullet::bullets.begin(); it != Bullet::bullets.end();)
	{
		(**it).updatePos();
		pair<int, int> p = {(**it).pos.x, (**it).pos.y};
		if (Map::map[p.first / TILE_SIZE][p.second / TILE_SIZE] == 1)
		{
			Bullet::bullets.erase(it++);
			continue;
		}
		if (p == make_pair(me.pos.x, me.pos.y))
		{
			Bullet::bullets.erase(it++);
			if (--me.health <= 0)
				return me.health = 0, true;
			continue;
		}
		if (p == make_pair(opponent.pos.x, opponent.pos.y))
		{
			Bullet::bullets.erase(it++);
			if (--opponent.health <= 0)
				return opponent.health = 0, true;
			continue;
		}
		it++;
	}
	return false;
}

int Game::updateAndSendSpawnables()
{
	pair<int, int> Pos = {-1, -1};
	int type = 0;
	if (Spawnable::flagsOnMap == 0)
		Pos = spawnObject(0);
	else if (Spawnable::healthsOnMap < MAX_SPAWN)
		if (rand() * 1.0 / RAND_MAX < SPAWN_PROB)
		{
			type = rand() * 1.0 / RAND_MAX < 0.5 ? 1 : -1;
			Pos = spawnObject(type > 0 ? 2 : -2);
		}
	char info[MSG_SIZE] = {-1};
	if (Pos == make_pair(-1, -1))
		return Network::sendRequest(info, MSG_SIZE);
	info[0] = 1;
	info[1] = Pos.first / TILE_SIZE;
	info[2] = Pos.second / TILE_SIZE;
	info[3] = type;
	return Network::sendRequest(info, MSG_SIZE);
}

pair<int, int> Game::spawnObject(int healthDelta)
{
	while (true)
	{
		pair<int, int> Pos = {(rand() % MAP_SIZE) * TILE_SIZE, (rand() % MAP_SIZE) * TILE_SIZE};
		if (Spawnable::spawnables.find(Pos) != Spawnable::spawnables.end() || Map::map[Pos.first / TILE_SIZE][Pos.second / TILE_SIZE] == 1 || Pos == make_pair(me.pos.x, me.pos.y) || Pos == make_pair(opponent.pos.x, opponent.pos.y))
			continue;
		Spawnable::spawnables[Pos] = new Spawnable(Pos.first, Pos.second, healthDelta, healthDelta == 0 ? flag : (healthDelta > 0 ? health : bomb));
		if (healthDelta != 0)
			++Spawnable::healthsOnMap;
		else
			++Spawnable::flagsOnMap;
		return Pos;
	}
}

int Game::recvSpawnInfo()
{
	char info[MSG_SIZE];
	if (Network::recvRequest(info, MSG_SIZE) == 4)
		return 4;
	if (info[0] != 1)
		return info[0] + 1;
	pair<int, int> Pos = {info[1] * TILE_SIZE, info[2] * TILE_SIZE};
	Spawnable::spawnables[Pos] = new Spawnable(Pos.first, Pos.second, info[3] * 2, info[3] == 0 ? flag : (info[3] > 0 ? health : bomb));
	return 0;
}

void Game::displayHealthBars()
{
	SDL_Rect healthRect = {OFFSET, WINDOW_WIDTH + OFFSET / 2, OFFSET, OFFSET};
	SDL_Color oldColor, healthColor = {250, 0, 0, 0};
	SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	for (int i = 0; i < me.health; ++i)
	{
		SDL_SetRenderDrawColor(renderer, healthColor.r, healthColor.g, healthColor.b, healthColor.a);
		SDL_RenderFillRect(renderer, &healthRect);
		healthColor.r -= 25;
		healthColor.g += 25;
		healthRect.x += healthRect.w;
	}
	healthColor = {250, 0, 0, 0};
	healthRect.x = WINDOW_WIDTH - OFFSET;
	healthRect.w = -healthRect.w;
	for (int i = 0; i < opponent.health; ++i)
	{
		SDL_SetRenderDrawColor(renderer, healthColor.r, healthColor.g, healthColor.b, healthColor.a);
		SDL_RenderFillRect(renderer, &healthRect);
		healthColor.r -= 25;
		healthColor.g += 25;
		healthRect.x += healthRect.w;
	}
	SDL_SetRenderDrawColor(renderer, 255 - oldColor.r, 255 - oldColor.g, 255 - oldColor.b, oldColor.a);
	healthRect.x = OFFSET;
	healthRect.w = MAX_HEALTH * OFFSET;
	SDL_RenderDrawRect(renderer, &healthRect);
	healthRect.x = WINDOW_WIDTH - OFFSET;
	healthRect.w = -healthRect.w;
	SDL_RenderDrawRect(renderer, &healthRect);
	SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}

void Game::displayFlagsCount()
{
	SDL_Color oldColor;
	SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	SDL_SetRenderDrawColor(renderer, 255 - oldColor.r, 255 - oldColor.g, 255 - oldColor.b, oldColor.a);
	SDL_Rect flagRect = {OFFSET, WINDOW_WIDTH + 2 * OFFSET, OFFSET, OFFSET};
	for (int i = 0; i < me.flags; ++i)
	{
		SDL_RenderCopy(renderer, flag, NULL, &flagRect);
		SDL_RenderDrawRect(renderer, &flagRect);
		flagRect.x += flagRect.w;
	}
	for (int i = me.flags; i < FLAG_LIMIT; ++i)
	{
		SDL_RenderDrawRect(renderer, &flagRect);
		flagRect.x += flagRect.w;
	}
	flagRect.x = WINDOW_WIDTH - OFFSET - flagRect.w;
	for (int i = 0; i < opponent.flags; ++i)
	{
		SDL_RenderCopy(renderer, flag, NULL, &flagRect);
		SDL_RenderDrawRect(renderer, &flagRect);
		flagRect.x -= flagRect.w;
	}
	for (int i = opponent.flags; i < FLAG_LIMIT; ++i)
	{
		SDL_RenderDrawRect(renderer, &flagRect);
		flagRect.x -= flagRect.w;
	}
	SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}

void Game::displayReloadTime()
{
	SDL_Color oldColor;
	SDL_GetRenderDrawColor(renderer, &oldColor.r, &oldColor.g, &oldColor.b, &oldColor.a);
	SDL_SetRenderDrawColor(renderer, 255 - oldColor.r, 255 - oldColor.g, 255 - oldColor.b, oldColor.a);
	SDL_Rect reloadRect = {OFFSET, WINDOW_WIDTH + 7 * OFFSET / 2, (WINDOW_WIDTH - 2 * OFFSET) / RELOAD, OFFSET};
	for (int i = 0; i < RELOAD - reloadTime; ++i)
	{
		SDL_RenderCopyEx(renderer, bullet, NULL, &reloadRect, 90, NULL, SDL_FLIP_NONE);
		reloadRect.x += reloadRect.w;
	}
	SDL_SetRenderDrawColor(renderer, oldColor.r, oldColor.g, oldColor.b, oldColor.a);
}

// @TODO: enhance the messages
void Game::finish()
{
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
	1. gameEnd function (after story finalised?)
	2. appropriate delays on capture/death/end
	3. mega bullet powerup?
	4. check if possible for players to move to same pos at same time
*/
