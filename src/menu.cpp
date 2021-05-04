#include "menu.hpp"

vector<string> Menu::serverMenuLines = {"[T]HEME", "[M]AP", "[C]ONNECT WITH CLIENT", "[P]LAY"}, Menu::clientMenuLines = {"[T]HEME", "[C]ONNECT TO SERVER", "[P]LAY"}, Menu::lines;
int Menu::mode, Menu::key;
SDL_Renderer *Menu::renderer;

int Menu::whichPlayer()
{
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
					return -1;
				case SDLK_1:
					return 1;
				case SDLK_2:
					return 2;
				default:
					continue;
				}
		SDL_RenderClear(renderer);
		char text[] = "PLAYER [1] OR PLAYER [2]?";
		displayText(renderer, text, Fonts::fonts[2], WINDOW_WIDTH / 2 - 3 * OFFSET, WINDOW_HEIGHT / 2 - OFFSET);
		SDL_RenderPresent(renderer);
	}
}

int Menu::displayLines()
{
	if (key == SDLK_ESCAPE)
		return -1;
	if (mode == 0)
	{
		if (key == SDLK_KP_ENTER || key == SDLK_RETURN)
			return -1;
		if (key == SDLK_t)
		{
			mode = 1;
			lines = {"THEME [1]", "THEME [2]"};
		}
		else if (key == SDLK_m && Game::isServer)
		{
			mode = 2;
			lines = {" ", " ", " ", " "}; // @TODO: after maps decided
		}
		else if (key == SDLK_c)
		{
			mode = 3 + Game::isServer;
			if (Game::isServer)
				lines[2] = "[C]ONNECT WITH CLIENT (PLEASE WAIT FOR 5s)";
			else
				lines[1] = "[C]ONNECT TO SERVER (PLEASE WAIT FOR 5s)";
		}
		else if (key == SDLK_p)
		{
			// if (!Network::done)
			// 	if (Game::isServer)
			// 		lines[2] = "[C]ONNECT WITH CLIENT (*)";
			// 	else
			// 		lines[1] = "[C]ONNECT TO SERVER (*)";
			// else
				return 1;
		}
	}
	else if (mode == 1)
	{
		if (key >= SDLK_1 && key <= SDLK_0 + THEMECOUNT)
			Theme::setTheme(key - SDLK_0, renderer);
		else if (key == SDLK_KP_ENTER || key == SDLK_RETURN)
		{
			mode = 0;
			lines = (Game::isServer ? serverMenuLines : clientMenuLines);
		}
	}
	else if (mode == 2)
	{
		// @TODO: after maps decided
		mode = 0;
		lines = serverMenuLines;
	}
	else if (mode == 3)
	{
		int ret = Network::makeClient();
		mode = 0;
		if (ret == 0)
			clientMenuLines[1] = "[C]ONNECT TO SERVER (DONE)", Network::done = true;
		else
			clientMenuLines[1] = "[C]ONNECT TO SERVER (RETRY)";
		lines = clientMenuLines;
	}
	else if (mode == 4)
	{
		int ret = Network::lookForClient();
		if (ret == 0)
			serverMenuLines[2] = "[C]ONNECT WITH CLIENT (DONE)", Network::done = true;
		else
			serverMenuLines[2] = "[C]ONNECT WITH CLIENT (RETRY)";
		mode = 0;
		lines = serverMenuLines;
	}
	SDL_RenderClear(renderer);
	for (int i = 0; i < lines.size(); ++i)
	{
		char *msg = new char[lines[i].size() + 1];
		if (!lines[i].empty())
		{
			strcpy(msg, lines[i].c_str());
			displayText(renderer, msg, Fonts::fonts[1], WINDOW_WIDTH / 2 - 3 * OFFSET, WINDOW_HEIGHT / 4 + OFFSET * i);
		}
	}
	SDL_RenderPresent(renderer);
	return 0;
}

int Menu::serverMenu()
{
	SDL_Event e;
	mode = 0;
	Network::done = false, Game::isServer = true;
	Map::setMap();
	lines = serverMenuLines;
	while (true)
	{
		key = -1;
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				return -1;
			else if (e.type == SDL_KEYDOWN)
				key = e.key.keysym.sym;
		int ret = displayLines();
		if (ret != 0)
			return ret;
	}
}

int Menu::clientMenu()
{
	SDL_Event e;
	mode = 0;
	Network::done = false, Game::isServer = false;
	lines = clientMenuLines;
	while (true)
	{
		key = -1;
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				return -1;
			else if (e.type == SDL_KEYDOWN)
				key = e.key.keysym.sym;
		int ret = displayLines();
		if (ret != 0)
			return ret;
	}
}
