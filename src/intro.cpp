#include "intro.hpp"

vector<string> Intro::story, Intro::firstPage, Intro::rules;
SDL_Renderer *Intro::renderer;
SDL_Texture *Intro::frontPageTexture, *Intro::plotTexture, *Intro::rulesTexture;

//1 for first page and 2 for story
void Intro::loadText()
{
	fstream fileName;

	//first page text (if any)
	firstPage.clear();
	fileName.open(FIRSTPAGEPATH, ios::in);
	if (fileName.is_open())
	{
		string line;
		while (getline(fileName, line))
		{
			firstPage.push_back(line);
		}
		fileName.close();
	}

	//plot text is loaded
	story.clear();
	fileName.open(STORYPATH, ios::in);
	if (fileName.is_open())
	{
		string line;
		while (getline(fileName, line))
		{
			story.push_back(line);
		}
		fileName.close();
	}

	//rules are loaded
	rules.clear();
	fileName.open(RULESPATH, ios::in);
	if (fileName.is_open())
	{
		string line;
		while (getline(fileName, line))
		{
			rules.push_back(line);
		}
		fileName.close();
	}

	return;
}

//Sets first page and the story background
void Intro::loadMedia()
{
	SDL_Surface *surface = IMG_Load("../assets/story/firstPage.png");
	frontPageTexture = SDL_CreateTextureFromSurface(renderer, surface);
	surface = IMG_Load("../assets/story/plot.png");
	plotTexture = SDL_CreateTextureFromSurface(renderer, surface);
	surface = IMG_Load("../assets/story/rules.png");
	rulesTexture = SDL_CreateTextureFromSurface(renderer, surface);
}

void Intro::displayLines(vector<string> vec)
{
	for (int i = 0; i < vec.size(); ++i)
		if (!vec[i].empty())
			Fonts::displayText(renderer, vec[i].c_str(), 1, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 4 + 2 * OFFSET * i);
}

int Intro::displayPlot()
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
				default:
					return 0;
				}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, plotTexture, NULL, NULL);
		displayLines(story);
		SDL_RenderPresent(renderer);
	}
	return 0;
}

int Intro::displayRules()
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
				default:
					return 0;
				}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, rulesTexture, NULL, NULL);
		displayLines(rules);
		SDL_RenderPresent(renderer);
	}

	return 0;
}

int Intro::displayStartingPage()
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
				default:
					return 0;
				}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, frontPageTexture, NULL, NULL);
		displayLines(firstPage);
		SDL_RenderPresent(renderer);
	}
}

void Intro::initIntro(SDL_Renderer *srcRender)
{
	renderer = srcRender;
	loadText();
	loadMedia();
}
