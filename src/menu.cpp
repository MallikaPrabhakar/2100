#include "menu.hpp"

vector<string> Menu::serverMenuLines = {"[T]HEME", "[M]AP", "[C]ONNECT WITH CLIENT", "[P]LAY", "[S]TORY", "[R]ULES"}, Menu::clientMenuLines = {"[T]HEME", "[C]ONNECT TO SERVER", "[P]LAY", "[S]TORY", "[R]ULES"}, Menu::exitLines, Menu::lines;
SDL_Renderer *Menu::renderer;
SDL_Texture *Menu::mapTexture;
SDL_Rect Menu::mapRect = {(WINDOW_WIDTH - PREVIEW_SIZE * MAP_SIZE) / 2, WINDOW_HEIGHT - (PREVIEW_SIZE * MAP_SIZE + OFFSET), (PREVIEW_SIZE * MAP_SIZE), (PREVIEW_SIZE * MAP_SIZE)};

void Menu::displayLines(bool displayMap)
{
	SDL_RenderClear(renderer);
	for (int i = 0; i < lines.size(); ++i)
		if (!lines[i].empty())
			Fonts::displayText(renderer, lines[i].c_str(), 1, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 + 2 * OFFSET * i);
	if (displayMap)
		SDL_RenderCopy(renderer, mapTexture, NULL, &mapRect);
	SDL_RenderPresent(renderer);
}

int Menu::whichPlayer()
{
	lines = {"PLAYER [1] OR PLAYER [2]?", "[Q]UIT"};
	SDL_Event e;
	while (true)
	{
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				return -1;
			else if (e.type == SDL_KEYDOWN)
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
				case SDLK_q:
					return -1;
				case SDLK_1:
					return 1;
				case SDLK_2:
					return 2;
				}
		displayLines();
	}
}

void Menu::handleMenuKeyEvents(int &mode, int key)
{
	if (key == SDLK_ESCAPE)
	{
		mode = QUIT;
		return;
	}
	if (mode == MAIN_MENU)
	{
		if (key == SDLK_t)
		{
			mode = THEME;
			lines = {};
			for (int i = 1; i <= THEMECOUNT; ++i)
				lines.push_back("THEME [" + to_string(i) + "]");
		}
		else if (key == SDLK_m && Game::isServer)
		{
			mode = MAP;
			lines = {"(0) CHOOSE ONE FOR ME", "(1) WINDMILL", "(2) TUNNELS", "(3) BAMBOO", "(4) RUINS", "(5) HONEYCOMB", "(6) RANDOM"};
		}
		else if (key == SDLK_c)
		{
			mode = Game::isServer ? LOOK : CONNECT;
			if (Game::isServer)
				lines[2] = "[C]ONNECT WITH CLIENT (PLEASE WAIT FOR " + to_string(TIMEOUT) + "s)";
			else
			{
				lines = {"ENTER IP ADDRESS", "127.0.0.1"};
			}
		}
		else if (key == SDLK_s)
		{
			mode = STORY;
		}
		else if (key == SDLK_r)
		{
			mode = RULES;
		}
		else if (key == SDLK_p)
		{
			if (!Network::done)
				if (Game::isServer)
					lines[2] = "[C]ONNECT WITH CLIENT (*)";
				else
					lines[1] = "[C]ONNECT TO SERVER (*)";
			else
			{
				mode = PLAY;
				return;
			}
		}
	}
	else if (mode == THEME)
	{
		if (key >= SDLK_1 && key <= SDLK_0 + THEMECOUNT)
			Theme::setTheme(key - SDLK_0, renderer);
		else if (key == SDLK_KP_ENTER || key == SDLK_RETURN)
		{
			mode = MAIN_MENU;
			lines = (Game::isServer ? serverMenuLines : clientMenuLines);
		}
	}
	else if (mode == MAP)
	{
		if (key >= SDLK_0 && key <= SDLK_6)
		{
			Map::setMap(key - SDLK_0);
			Game::genMapTexture(mapTexture, PREVIEW_SIZE, WINDOW_WIDTH - 75, WINDOW_HEIGHT - 175);
		}
		else if (key == SDLK_KP_ENTER || key == SDLK_RETURN)
		{
			mode = MAIN_MENU;
			lines = (Game::isServer ? serverMenuLines : clientMenuLines);
		}
	}
	else if (mode == RULES)
	{
		if (Intro::displayRules() == -1)
		{
			mode = QUIT;
		}
		else
		{
			mode = MAIN_MENU;
		}
	}
	else if (mode == STORY)
	{
		if (Intro::displayPlot() == -1)
		{
			mode = QUIT;
		}
		else
		{
			mode = MAIN_MENU;
		}
	}

	else if (mode == CONNECT)
	{
		if ((key >= SDLK_0 && key <= SDLK_9) || key == SDLK_PERIOD)
			lines[1] += key;
		else if (key == SDLK_BACKSPACE)
		{
			if (lines[1].empty())
			{
				mode = MAIN_MENU;
				lines = clientMenuLines;
			}
			else
				lines[1].pop_back();
		}
		else if (key == SDLK_RETURN)
		{
			int ret = Network::makeClient(lines[1].c_str());
			mode = MAIN_MENU;
			if (ret == 0)
				clientMenuLines[1] = "[C]ONNECT TO SERVER (DONE)", Network::done = true;
			else
				clientMenuLines[1] = "[C]ONNECT TO SERVER (RETRY) " + to_string(ret);
			lines = clientMenuLines;
		}
	}
	else if (mode == LOOK)
	{
		int ret = Network::lookForClient();
		if (ret == 0)
			serverMenuLines[2] = "[C]ONNECT WITH CLIENT (DONE)", Network::done = true;
		else
			serverMenuLines[2] = "[C]ONNECT WITH CLIENT (RETRY) " + to_string(ret);
		mode = MAIN_MENU;
		lines = serverMenuLines;
	}

	displayLines(mode == MAP);
}

int Menu::menuLoop()
{
	SDL_Event e;
	int mode = MAIN_MENU, key;
	Network::done = false;

	if (Game::isServer)
		Map::setMap();
	mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, PREVIEW_SIZE * MAP_SIZE, PREVIEW_SIZE * MAP_SIZE);
	Game::genMapTexture(mapTexture, PREVIEW_SIZE, mapRect.x, mapRect.y);

	lines = (Game::isServer ? serverMenuLines : clientMenuLines);
	exitLines.clear();

	while (true)
	{
		key = -1;
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				mode = QUIT;
			else if (e.type == SDL_KEYDOWN)
				key = e.key.keysym.sym;
		}
		handleMenuKeyEvents(mode, key);
		if (mode == QUIT)
			return -1;
		if (mode == PLAY)
			return 0;
	}
}

int Menu::exitMenu(string target)
{
	lines = exitLines;
	lines.push_back("[G]O BACK TO "+target);
	lines.push_back("[Q]UIT");
	SDL_Event e;
	while (true)
	{
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				return -1;
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
				case SDLK_q:
					return -1;

				case SDLK_g:
					return 0;
					break;
				}
			}
		displayLines();
	}
}
