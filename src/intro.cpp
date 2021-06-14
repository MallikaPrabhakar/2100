#include "intro.hpp"

vector<string> Intro::story, Intro::firstPage, Intro::rules;
SDL_Renderer *Intro::renderer;
SDL_Texture *Intro::frontPageTexture, *Intro::plotTexture, *Intro::rulesTexture;

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

int Intro::loadText()
{
	//first page text (if any)
	if (fillVector(FIRSTPAGEPATH, firstPage) != 0)
		return 1;

	//plot text is loaded
	if (fillVector(STORYPATH, story) != 0)
		return 2;

	//rules are loaded
	if (fillVector(RULESPATH, rules) != 0)
		return 3;

	return 0;
}

//Sets first page and the story background
int Intro::loadMedia()
{
	SDL_Surface *surface = IMG_Load(FIRSTPAGE);
	if ((frontPageTexture = SDL_CreateTextureFromSurface(renderer, surface)) == NULL)
		return 1;

	surface = IMG_Load(PLOTIMG);
	if ((plotTexture = SDL_CreateTextureFromSurface(renderer, surface)) == NULL)
		return 2;

	surface = IMG_Load(RULESIMG);
	if ((rulesTexture = SDL_CreateTextureFromSurface(renderer, surface)) == NULL)
		return 3;

	return 0;
}

int Intro::displayPage(SDL_Texture *texture, vector<string> &vec)
{
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
					return Sound::playChunk(Sound::correct), -1;
				default:
					return Sound::playChunk(Sound::correct), 0;
				}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, plotTexture, NULL, NULL);
		for (int i = 0; i < vec.size(); ++i)
			if (!vec[i].empty())
				Fonts::displayText(renderer, vec[i].c_str(), 1, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 + 2 * OFFSET * i);
		SDL_RenderPresent(renderer);
	}
}

int Intro::displayPlot()
{
	return displayPage(plotTexture, story);
}

int Intro::displayRules()
{
	return displayPage(rulesTexture, rules);
}

int Intro::displayStartingPage()
{
	return displayPage(frontPageTexture, firstPage);
}

int Intro::initIntro(SDL_Renderer *srcRender)
{
	renderer = srcRender;
	int ret = loadText();
	if (ret != 0)
		return ret;
	return loadMedia();
}
