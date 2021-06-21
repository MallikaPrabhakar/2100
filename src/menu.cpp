#include "menu.hpp"

vector<string> Menu::serverMenuLines = {"[T]HEME", "[M]AP", "[C]ONNECT WITH CLIENT", "[P]LAY", "[S]TORY", "[R]ULES"}, Menu::clientMenuLines = {"[T]HEME", "[C]ONNECT TO SERVER", "[P]LAY", "[S]TORY", "[R]ULES"}, Menu::lines;
SDL_Renderer *Menu::renderer;

void Menu::displayLines()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, Theme::backgroundTexture, NULL, NULL);
	for (int i = 0; i < lines.size(); ++i)
		if (!lines[i].empty())
			Fonts::displayText(renderer, lines[i].c_str(), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 + 2 * OFFSET * i);
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
				return Sound::playChunk(Sound::correct), -1;
			else if (e.type == SDL_KEYDOWN)
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
				case SDLK_q:
					return Sound::playChunk(Sound::correct), -1;
				case SDLK_1:
					return Sound::playChunk(Sound::correct), 1;
				case SDLK_2:
					return Sound::playChunk(Sound::correct), 2;
				default:
					Sound::playChunk(Sound::incorrect);
					break;
				}
		displayLines();
	}
}

void Menu::handleMenuKeyEvents(int key)
{
	if (key == SDLK_ESCAPE)
	{
		Modes::mode = Modes::QUIT;
		Sound::playChunk(Sound::correct);
		return;
	}
	if (key == SDLK_t)
	{
		Sound::playChunk(Sound::correct);
		Modes::themeMenu();
		Modes::mode = Modes::MAIN_MENU;
	}
	else if (key == SDLK_m && Game::isServer)
	{
		Sound::playChunk(Sound::correct);
		Modes::mapMenu();
		Modes::mode = Modes::MAIN_MENU;
	}
	else if (key == SDLK_c)
	{
		if (Game::isServer)
		{
			Sound::playChunk(Sound::correct);
			int ret = Network::lookForClient();
			if (ret == 0)
				Sound::playChunk(Sound::correct), serverMenuLines[2] = "[C]ONNECT WITH CLIENT (DONE)", Network::done = true;
			else
				Sound::playChunk(Sound::incorrect), serverMenuLines[2] = "[C]ONNECT WITH CLIENT (RETRY) " + to_string(ret);
			lines = serverMenuLines;
		}
		else if (Modes::connectMenu() == 0)
		{
			Sound::playChunk(Sound::correct);
			int ret = Network::makeClient(lines[1].c_str());
			if (ret == 0)
				Sound::playChunk(Sound::correct), clientMenuLines[1] = "[C]ONNECT TO SERVER (DONE)", Network::done = true;
			else
				Sound::playChunk(Sound::incorrect), clientMenuLines[1] = "[C]ONNECT TO SERVER (RETRY) " + to_string(ret);
			lines = clientMenuLines;
		}
		Modes::mode = Modes::MAIN_MENU;
	}
	else if (key == SDLK_s)
	{
		Sound::playChunk(Sound::correct);
		if (Modes::displayStory() == -1)
			Modes::mode = Modes::QUIT;
		else
			Modes::mode = Modes::MAIN_MENU;
	}
	else if (key == SDLK_r)
	{
		Sound::playChunk(Sound::correct);
		if (Modes::displayRules() == -1)
			Modes::mode = Modes::QUIT;
		else
			Modes::mode = Modes::MAIN_MENU;
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
			Sound::playChunk(Sound::correct);
			Modes::mode = Modes::PLAY;
			return;
		}
	}

	displayLines();
}

int Menu::menuLoop()
{
	SDL_Event e;
	int key;
	Modes::mode = Modes::MAIN_MENU;
	Network::done = false;

	Modes::mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, PREVIEW_SIZE * MAP_SIZE, PREVIEW_SIZE * MAP_SIZE);

	if (Game::isServer)
		serverMenuLines[2] = "[C]ONNECT WITH CLIENT", lines = serverMenuLines;
	else
		clientMenuLines[1] = "[C]ONNECT TO SERVER", lines = clientMenuLines;

	while (true)
	{
		key = -1;
		if (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				Modes::mode = Modes::QUIT;
			else if (e.type == SDL_KEYDOWN)
				key = e.key.keysym.sym;
		}

		handleMenuKeyEvents(key);

		if (Modes::mode == Modes::QUIT)
			return -1;
		if (Modes::mode == Modes::PLAY)
			return 0;
	}
}

int Menu::exitMenu(string target, string exitMessage)
{
	lines = {exitMessage};
	lines.push_back("[G]O BACK TO " + target);
	lines.push_back("[Q]UIT");
	SDL_Event e;
	while (true)
	{
		if (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				return Sound::playChunk(Sound::correct), -1;
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
				case SDLK_q:
					return Sound::playChunk(Sound::correct), -1;
				case SDLK_g:
					return Sound::playChunk(Sound::correct), 0;
				default:
					Sound::playChunk(Sound::incorrect);
					break;
				}
			}
		displayLines();
	}
}
