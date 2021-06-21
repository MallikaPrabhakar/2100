#include "modes.hpp"

vector<string> Modes::story, Modes::rules, Modes::displayLines;
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
		{
			toFillVector.push_back(line);
		}
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

int Modes::displayPage(function<bool(SDL_Event)> operation)
{
	SDL_Event e;
	int index = 0;
	while (true)
	{
		if (SDL_PollEvent(&e))
		{
			if (operation(e))
				return Sound::playChunk(Sound::correct), 0;
			else if (e.type == SDL_QUIT)
				return Sound::playChunk(Sound::correct), -1;
			else if (e.type == SDL_KEYDOWN)
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					return Sound::playChunk(Sound::correct), -1;
				case SDLK_KP_ENTER:
				case SDLK_RETURN:
					return Sound::playChunk(Sound::correct), 0;
				}
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, renderTexture, NULL, NULL);
		if (mode == MAP)
			SDL_RenderCopy(renderer, mapTexture, NULL, &mapRect);
		if (mode == STORY)
		{
			if (!crawlText(index++ / WAIT))
				return Sound::playChunk(Sound::correct), 0;
		}
		else
			for (int i = 0; i < displayLines.size(); ++i)
				if (!displayLines[i].empty())
					Fonts::displayText(renderer, displayLines[i].c_str(), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 + 2 * OFFSET * i);
		SDL_RenderPresent(renderer);
	}
}

bool Modes::crawlText(int index)
{
	if (index > (int)displayLines.size() - FONTSCOUNT)
		return false;
	for (int i = 0; i < FONTSCOUNT; ++i)
		if (!displayLines[i + index].empty())
			Fonts::displayText(renderer, displayLines[i + index].c_str(), WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 + 2 * OFFSET * i, i, {255, 255, 255});
	return true;
}

int Modes::displayStory()
{
	mode = STORY, renderTexture = storyTexture, displayLines = story;
	return displayPage([](SDL_Event e)
					   { return e.type == SDL_KEYDOWN && e.key.keysym.sym != SDLK_ESCAPE; });
}

int Modes::displayRules()
{
	mode = RULES, renderTexture = Theme::backgroundTexture, displayLines = rules;
	return displayPage([](SDL_Event e)
					   { return e.type == SDL_KEYDOWN && e.key.keysym.sym != SDLK_ESCAPE; });
}

int Modes::displayFrontPage()
{
	mode = FRONT_PAGE, renderTexture = frontPageTexture, displayLines.clear();
	return displayPage([](SDL_Event e)
					   { return e.type == SDL_KEYDOWN && e.key.keysym.sym != SDLK_ESCAPE; });
}

int Modes::themeMenu()
{
	mode = THEME, renderTexture = Theme::backgroundTexture, displayLines.clear();
	for (int i = 1; i <= THEMECOUNT; ++i)
		displayLines.push_back("THEME [" + to_string(i) + "]");
	return displayPage([](SDL_Event e)
					   {
						   if (e.type == SDL_KEYDOWN && e.key.keysym.sym >= SDLK_1 && e.key.keysym.sym <= SDLK_0 + THEMECOUNT)
							   Sound::playChunk(Sound::correct), Theme::setTheme(e.key.keysym.sym - SDLK_0, renderer), Game::initTextures(renderer);
						   renderTexture = Theme::backgroundTexture;
						   return 0;
					   });
}

int Modes::mapMenu()
{
	mode = MAP, displayLines = {"(0) CHOOSE ONE FOR ME", "(1) WINDMILL", "(2) TUNNELS", "(3) BAMBOO", "(4) RUINS", "(5) HONEYCOMB", "(6) RANDOM"}, Game::genMapTexture(mapTexture, PREVIEW_SIZE, WINDOW_WIDTH - 75, WINDOW_HEIGHT - 175);
	return displayPage([](SDL_Event e)
					   {
						   if (e.type == SDL_KEYDOWN && e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_6)
							   Sound::playChunk(Sound::correct), Map::setMap(e.key.keysym.sym - SDLK_0), Game::genMapTexture(mapTexture, PREVIEW_SIZE, WINDOW_WIDTH - 75, WINDOW_HEIGHT - 175);
						   return 0;
					   });
}

int Modes::connectMenu()
{
	mode = CONNECT, displayLines = {"ENTER IP ADDRESS", "127.0.0.1"};
	return displayPage([](SDL_Event e)
					   {
						   if (e.type == SDL_KEYDOWN)
						   {
							   int key = e.key.keysym.sym;
							   if ((key >= SDLK_0 && key <= SDLK_9) || key == SDLK_PERIOD)
								   Sound::playChunk(Sound::correct), displayLines[1] += key;
							   else if (key == SDLK_BACKSPACE)
								   if (!displayLines[1].empty())
									   Sound::playChunk(Sound::correct), displayLines[1].pop_back();
						   }
						   return 0;
					   });
}

int Modes::initModes(SDL_Renderer *srcRenderer)
{
	mode = INIT;
	renderer = srcRenderer;
	int ret = loadText();
	if (ret != 0)
		return ret;
	return loadMedia();
}
