#include "menu.hpp"

int whichPlayer(SDL_Renderer *renderer)
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
		displayText(renderer, text, Fonts::fonts[2], 500, 440);
		SDL_RenderPresent(renderer);
	}
}

vector<string> serverMenuLines = {"[T]HEME", "[M]AP", "[C]ONNECT WITH CLIENT", "[P]LAY"}, clientMenuLines = {"[T]HEME", "[C]ONNECT TO SERVER", "[P]LAY"}, lines;
int mode;
bool done;

int displayLines(SDL_Renderer *renderer, SDL_Keycode key, bool isServer)
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
		else if (key == SDLK_m && isServer)
		{
			mode = 2;
			lines = {" ", " ", " ", " "}; // @TODO: after maps decided
		}
		else if (key == SDLK_c && !done)
			mode = 3 + isServer;
		else if (key == SDLK_p)
			return 1;
	}
	else if (mode == 1)
	{
		if (key >= SDLK_1 && key <= SDLK_0 + THEMECOUNT)
			Theme::setTheme(key - SDLK_0, renderer);
		else if (key == SDLK_KP_ENTER || key == SDLK_RETURN)
		{
			mode = 0;
			lines = (isServer ? serverMenuLines : clientMenuLines);
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
			clientMenuLines[1] = "[C]ONNECT TO SERVER (DONE)", done = true;
		else
			clientMenuLines[1] = "[C]ONNECT TO SERVER (RETRY)";
		lines = clientMenuLines;
	}
	else if (mode == 4)
	{
		int ret = Network::lookForClient();
		mode = 0;
		if (ret == 0)
			serverMenuLines[2] = "[C]ONNECT WITH CLIENT (DONE)", done = true;
		else
			serverMenuLines[2] = "[C]ONNECT WITH CLIENT (RETRY)";
		lines = serverMenuLines;
	}
	SDL_RenderClear(renderer);
	for (int i = 0; i < lines.size(); ++i)
	{
		char *msg = new char[lines[i].size() + 1];
		if (!lines[i].empty())
		{
			strcpy(msg, lines[i].c_str());
			displayText(renderer, msg, Fonts::fonts[1], 500, 240 + 50 * i);
		}
	}
	SDL_RenderPresent(renderer);
	return 0;
}

int serverMenu(SDL_Renderer *renderer)
{
	SDL_Event e;
	mode = 0;
	done = false;
	lines = serverMenuLines;
	int key = -1;
	while (true)
	{
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				return -1;
			else if (e.type == SDL_KEYDOWN)
				key = e.key.keysym.sym;
		int ret = displayLines(renderer, key, true);
		if (ret != 0)
			return ret;
		key = -1;
	}
}

int clientMenu(SDL_Renderer *renderer)
{
	SDL_Event e;
	mode = 0;
	done = false;
	lines = clientMenuLines;
	int key = -1;
	while (true)
	{
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				return -1;
			else if (e.type == SDL_KEYDOWN)
				key = e.key.keysym.sym;
		int ret = displayLines(renderer, key, false);
		if (ret != 0)
			return ret;
		key = -1;
	}
}
