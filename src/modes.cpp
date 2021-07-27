#include "modes.hpp"

vector<string> Modes::story, Modes::rules, Modes::displayLines, Modes::serverMenuLines = {"[T]HEME", "[M]AP", "[C]ONNECT WITH CLIENT", "[P]LAY", "[S]TORY", "[R]ULES"}, Modes::clientMenuLines = {"[T]HEME", "[C]ONNECT TO SERVER", "[P]LAY", "[S]TORY", "[R]ULES"};
SDL_Renderer *Modes::renderer;
SDL_Texture *Modes::frontPageTexture, *Modes::storyTexture, *Modes::renderTexture, *Modes::mapTexture;
SDL_Rect Modes::mapRect = {(WINDOW_WIDTH - PREVIEW_SIZE * MAP_SIZE) / 2, WINDOW_HEIGHT - (PREVIEW_SIZE * MAP_SIZE + OFFSET), (PREVIEW_SIZE * MAP_SIZE), (PREVIEW_SIZE * MAP_SIZE)};
Modes::modes Modes::mode;

int fillVector(string fileName, vector<string> &toFillVector)
{
	fstream file;
	file.open(fileName, ios::in);
	if (file.is_open())
	{
		string line;
		while (getline(file, line))
			toFillVector.push_back(line);
		file.close();
		return 0;
	}
	return -1;
}

int Modes::loadText()
{
	// plot text is loaded
	story.clear();
	for (int i = 0; i < FONTSCOUNT - 1; ++i)
		story.push_back("");
	if (fillVector(STORYPATH, story) != 0)
		return 2;
	for (int i = 0; i < FONTSCOUNT - 1; ++i)
		story.push_back("");

	// rules are loaded
	rules.clear();
	if (fillVector(RULESPATH, rules) != 0)
		return 3;

	return 0;
}

// sets first page and the story background
int Modes::loadMedia()
{
	SDL_Surface *surface = IMG_Load(FIRSTPAGE);
	if ((frontPageTexture = SDL_CreateTextureFromSurface(renderer, surface)) == NULL)
		return 1;

	surface = IMG_Load(PLOTIMG);
	if ((storyTexture = SDL_CreateTextureFromSurface(renderer, surface)) == NULL)
		return 2;

	return 0;
}

void Modes::reRender()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, renderTexture, NULL, NULL);
	if (mode == MAP)
		SDL_RenderCopy(renderer, mapTexture, NULL, &mapRect);
	for (int i = 0; i < displayLines.size(); ++i)
		if (!displayLines[i].empty())
			Fonts::displayText(renderer, displayLines[i].c_str(), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 + 2 * OFFSET * i);
	SDL_RenderPresent(renderer);
}

int Modes::displayPage(function<int(SDL_Event)> operation)
{
	SDL_Event e;
	int index = 0;
	reRender();
	while (true)
	{
		if (SDL_PollEvent(&e))
		{
			int ret = operation(e);
			switch (ret)
			{
			case 0:
				if (e.type == SDL_QUIT)
					return Sound::playChunk(), -1;
				else if (e.type == SDL_KEYDOWN)
					switch (e.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						return Sound::playChunk(Sound::incorrect), -1;
					case SDLK_KP_ENTER:
					case SDLK_RETURN:
						return Sound::playChunk(), 0;
					}
				break;
			case 1:
				return Sound::playChunk(), 0;
			case 2:
				Sound::playChunk();
				break;
			case 3:
				Sound::playChunk(Sound::incorrect);
				break;
			default:
				return Sound::playChunk(Sound::incorrect), -ret;
			}
		}
		if (mode == STORY)
		{
			if (!crawlText(index++ / WAIT))
				return Sound::playChunk(), 0;
		}
	}
}

bool Modes::crawlText(int index)
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, renderTexture, NULL, NULL);
	if (index > (int)displayLines.size() - FONTSCOUNT)
		return false;
	for (int i = 0; i < FONTSCOUNT; ++i)
		if (!displayLines[i + index].empty())
			Fonts::displayText(renderer, displayLines[i + index].c_str(), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 + 2 * OFFSET * i, i, {255, 255, 255});
	SDL_RenderPresent(renderer);
	return true;
}

int Modes::displayStory()
{
	mode = STORY, renderTexture = storyTexture, displayLines = story;
	return displayPage([](SDL_Event e) -> int
					   { return e.type == SDL_KEYDOWN && e.key.keysym.sym != SDLK_ESCAPE; });
}

int Modes::displayRules()
{
	mode = RULES, renderTexture = Theme::backgroundTexture, displayLines = rules;
	return displayPage([](SDL_Event e) -> int
					   { return e.type == SDL_KEYDOWN && e.key.keysym.sym != SDLK_ESCAPE; });
}

int Modes::displayFrontPage()
{
	mode = FRONT_PAGE, renderTexture = frontPageTexture, displayLines.clear();
	return displayPage([](SDL_Event e) -> int
					   { return e.type == SDL_KEYDOWN && e.key.keysym.sym != SDLK_ESCAPE; });
}

int Modes::displayPlayerSelection()
{
	mode = PLAYER, renderTexture = Theme::backgroundTexture, displayLines = {"PLAYER [1] OR PLAYER [2]?"};
	return displayPage([](SDL_Event e) -> int
					   {
						   if (e.type == SDL_KEYDOWN)
						   {
							   int key = e.key.keysym.sym;
							   if (key == SDLK_1)
							   {
								   int ret = -Network::makeServer();
								   return Game::isServer = 1, ret == 0 ? (Map::setMap(), 1) : ret;
							   }
							   else if (key == SDLK_2)
								   return Game::isServer = 0, 1;
							   else if (key != SDLK_RETURN && key != SDLK_KP_ENTER && key != SDLK_ESCAPE)
								   return 3;
						   }
						   return 0;
					   });
}

int Modes::themeMenu()
{
	mode = THEME, renderTexture = Theme::backgroundTexture, displayLines.clear();
	for (int i = 1; i <= THEMECOUNT; ++i)
		displayLines.push_back("THEME [" + to_string(i) + "]");
	return displayPage([](SDL_Event e) -> int
					   {
						   if (e.type == SDL_KEYDOWN)
						   {
							   int key = e.key.keysym.sym;
							   if (key >= SDLK_1 && key <= SDLK_0 + THEMECOUNT)
							   {
								   Theme::setTheme(key - SDLK_0, renderer), renderTexture = Theme::backgroundTexture;
								   Game::initTextures(renderer);
								   reRender();
								   return 2;
							   }
							   else if (key != SDLK_RETURN && key != SDLK_KP_ENTER && key != SDLK_ESCAPE)
								   return 3;
						   }
						   return 0;
					   });
}

int Modes::mapMenu()
{
	mode = MAP, renderTexture = Theme::backgroundTexture, displayLines = {"(0) CHOOSE ONE FOR ME", "(1) WINDMILL", "(2) TUNNELS", "(3) BAMBOO", "(4) RUINS", "(5) HONEYCOMB", "(6) RANDOM"}, Game::genMapTexture(mapTexture, PREVIEW_SIZE, WINDOW_WIDTH - 75, WINDOW_HEIGHT - 175);
	return displayPage([](SDL_Event e) -> int
					   {
						   if (e.type == SDL_KEYDOWN)
						   {
							   int key = e.key.keysym.sym;
							   if (key >= SDLK_0 && key <= SDLK_6)
							   {
								   Map::setMap(key - SDLK_0), Game::genMapTexture(mapTexture, PREVIEW_SIZE, WINDOW_WIDTH - 75, WINDOW_HEIGHT - 175);
								   reRender();
								   return 2;
							   }
							   else if (key != SDLK_RETURN && key != SDLK_KP_ENTER && key != SDLK_ESCAPE)
								   return 3;
						   }
						   return 0;
					   });
}

int Modes::connectMenu()
{
	mode = CONNECT, renderTexture = Theme::backgroundTexture, displayLines = {"ENTER IP ADDRESS", "127.0.0.1"};
	return displayPage([](SDL_Event e) -> int
					   {
						   if (e.type == SDL_KEYDOWN)
						   {
							   int ret = 0;
							   int key = e.key.keysym.sym;
							   if ((key >= SDLK_0 && key <= SDLK_9) || key == SDLK_PERIOD)
								   displayLines[1] += key, ret = 2;
							   else if (key == SDLK_BACKSPACE)
								   if (!displayLines[1].empty())
									   displayLines[1].pop_back(), ret = 2;
								   else
									   ret = 3;
							   reRender();
							   if (ret)
								   return ret;
							   return key == SDLK_RETURN || key == SDLK_KP_ENTER ? 1 : key == SDLK_ESCAPE ? 0
																										  : 3;
						   }
						   return 0;
					   });
}

int Modes::mainMenu()
{
	vector<string> &myLines = Game::isServer ? serverMenuLines : clientMenuLines;
	if (!Network::connected)
		myLines[1 + Game::isServer] = Game::isServer ? "[C]ONNECT WITH CLIENT" : "[C]ONNECT TO SERVER";

	mode = MAIN_MENU, renderTexture = Theme::backgroundTexture, displayLines = myLines;
	mapTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, PREVIEW_SIZE * MAP_SIZE, PREVIEW_SIZE * MAP_SIZE);

	return displayPage([&](SDL_Event e)
					   {
						   int ret = 0;
						   if (e.type == SDL_KEYDOWN)
						   {
							   int key = e.key.keysym.sym;
							   switch (key)
							   {
							   case SDLK_t:
								   Sound::playChunk(), themeMenu(), ret = 2;
								   break;
							   case SDLK_s:
								   Sound::playChunk(), displayStory(), renderTexture = Theme::backgroundTexture, ret = 2;
								   break;
							   case SDLK_r:
								   Sound::playChunk(), displayRules(), ret = 2;
								   break;
							   case SDLK_p:
								   if (Network::connected)
									   return 1;
								   else if (myLines[1 + Game::isServer].back() != '*')
									   myLines[1 + Game::isServer] += " *", ret = 3;
								   break;
							   case SDLK_m:
								   if (Game::isServer)
									   Sound::playChunk(), mapMenu(), ret = 2;
								   else
									   ret = 3;
								   break;
							   case SDLK_c:
								   if (Game::isServer)
								   {
									   myLines[2] = "[C]ONNECT WITH CLIENT (PLEASE WAIT)", displayLines = myLines, reRender();
									   if (Network::lookForClient() == 0)
										   myLines[2] = "[C]ONNECT WITH CLIENT (DONE)", ret = 2;
									   else
										   myLines[2] = "[C]ONNECT WITH CLIENT (RETRY)", ret = 3;
								   }
								   else
								   {
									   Sound::playChunk();
									   if (connectMenu() != -1)
										   if (Network::makeClient(displayLines[1].c_str()) == 0)
											   myLines[1] = "[C]ONNECT TO SERVER (DONE)", ret = 2;
										   else
											   myLines[1] = "[C]ONNECT TO SERVER (RETRY)", ret = 3;
								   }
								   break;
							   default:
								   ret = key != SDLK_ESCAPE ? 3 : 0;
							   }
							   displayLines = myLines, mode = MAIN_MENU;
							   reRender();
						   }
						   return ret;
					   });
}

bool Modes::exitMenu(string exitMessage)
{
	displayLines = {exitMessage};
	displayLines.push_back("[G]O BACK TO " + getModeName(mode));
	displayLines.push_back("[Q]UIT");
	renderTexture = Theme::backgroundTexture;

	return displayPage([](SDL_Event e) -> int
					   {
						   if (e.type == SDL_KEYDOWN)
						   {
							   int key = e.key.keysym.sym;
							   if (key == SDLK_g)
								   return 1;
							   else if (key == SDLK_q)
								   return -1;
							   else if (key != SDLK_ESCAPE)
								   return 3;
						   }
						   return 0;
					   });
}

string Modes::getModeName(modes mode)
{
	switch (mode)
	{
	case FRONT_PAGE:
		return "LOADING SCREEN";
	case MAIN_MENU:
		return "MAIN MENU";
	case THEME:
		return "THEME SELECTION";
	case MAP:
		return "MAP SELECTION";
	case CONNECT:
		return "CONNECTION SCREEN";
	case STORY:
		return "STORY";
	case RULES:
		return "RULES";
	case PLAY:
		return "GAME";
	default:
		return "PREVIOUS SCREEN";
	}
}

int Modes::initModes(SDL_Renderer *srcRenderer)
{
	renderer = srcRenderer;
	int ret = loadText();
	if (ret != 0)
		return ret;
	return loadMedia();
}
