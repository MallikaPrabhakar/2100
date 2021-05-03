#include "game.hpp"

SDL_Texture *Game::mapTexture, *Game::tile, *Game::me, *Game::opponent, *Game::wall, *Game::myBase, *Game::oppBase;
SDL_Renderer *Game::renderer;
SDL_Rect Game::mapRect;
bool Game::isServer;

void Game::renderInit(SDL_Renderer *sourceRenderer, bool server)
{
	renderer = sourceRenderer;
	isServer = server;
	mapRect.x = 0;
	mapRect.y = HEALTH_HEIGHT + GAP;
	mapRect.w = WINDOW_WIDTH;
	mapRect.h = WINDOW_HEIGHT - mapRect.y;
	initTextures();
	mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, TILE_SIZE * MAP_SIZE, TILE_SIZE * MAP_SIZE);
	SDL_SetRenderTarget(renderer, mapTexture);
	SDL_Rect rect;
	rect.h = rect.w = TILE_SIZE;

	for (int i = 0; i < MAP_SIZE; ++i)
		for (int j = 0; j < MAP_SIZE; ++j)
		{
			rect.x = TILE_SIZE * j;
			rect.y = TILE_SIZE * i;
			if (Map::map[i][j] == 0)
				SDL_RenderCopy(renderer, tile, NULL, &rect);
			else if (Map::map[i][j] == 1)
				SDL_RenderCopy(renderer, wall, NULL, &rect);
			else if (Map::map[i][j] == 2)
				SDL_RenderCopy(renderer, (isServer ? myBase : oppBase), NULL, &rect);
			else if (Map::map[i][j] == 3)
				SDL_RenderCopy(renderer, (!isServer ? myBase : oppBase), NULL, &rect);
		}
	SDL_SetRenderTarget(renderer, NULL);
}

// @TODO: home bases need to be changed to look contrasting
void Game::initTextures()
{
	SDL_Surface *surface;

	surface = IMG_Load(string(Theme::themeSource + "tile.jpg").c_str());
	tile = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load(string(Theme::themeSource + (isServer ? "player1.jpg" : "player2.jpg")).c_str());
	me = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load(string(Theme::themeSource + (!isServer ? "player1.jpg" : "player2.jpg")).c_str());
	opponent = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load(string(Theme::themeSource + "wall.jpg").c_str());
	wall = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load(string(Theme::themeSource + (isServer ? "home1.jpg" : "home2.jpg")).c_str());
	myBase = SDL_CreateTextureFromSurface(renderer, surface);

	surface = IMG_Load(string(Theme::themeSource + (!isServer ? "home1.jpg" : "home2.jpg")).c_str());
	oppBase = SDL_CreateTextureFromSurface(renderer, surface);
}

void Game::loopGame()
{
	SDL_Event e;
	while (true)
	{
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				return;
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, Game::mapTexture, NULL, &mapRect);
		SDL_RenderPresent(renderer);
	}
}
