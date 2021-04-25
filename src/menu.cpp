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

vector<string> serverMenuLines = {"[T]HEME", "[P]LAY"};

void displayLines(SDL_Renderer *renderer, vector<string> lines)
{
	SDL_RenderClear(renderer);
	for (int i = 0; i < lines.size(); ++i)
	{
		char *msg = new char[lines[i].size() + 1];
		strcpy(msg, lines[i].c_str());
		displayText(renderer, msg, Fonts::fonts[1], 500, 240 + 50 * i);
	}
	SDL_RenderPresent(renderer);
}

int serverMenu(SDL_Renderer *renderer)
{
	SDL_Event e;
	int mode = 0;
	vector<string> lines = serverMenuLines;
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
				case SDLK_RETURN:
				case SDLK_KP_ENTER:
					if (mode == 0)
						return 0;
					mode = 0;
					lines = serverMenuLines;
					break;
				case SDLK_t:
					if (mode == 0)
					{
						mode = 1;
						lines = {"THEME [1] OR THEME [2]?"};
					}
					break;
				case SDLK_p:
					if (mode == 0)
						return 0;
				case SDLK_1:
					if (mode == 1)
						Theme::setTheme(1, renderer);
					break;
				case SDLK_2:
					if (mode == 1)
						Theme::setTheme(2, renderer);
					break;
				case SDLK_BACKSPACE:
					mode = 0;
					lines = serverMenuLines;
				default:
					continue;
				}
		displayLines(renderer, lines);
	}
}

int clientMenu(SDL_Renderer *renderer)
{
	return 0;
}
