#include "intro.hpp"

vector<string> Intro::story, Intro::firstPage;
SDL_Renderer *Intro::renderer;
SDL_Surface *Intro::frontPage;
SDL_Texture *Intro::frontPageTexture;
SDL_Surface *Intro::plot;
SDL_Texture *Intro::plotTexture;
//1 for first page and 2 for story
void Intro::loadText(int name)
{
	fstream fileName;
	//first page loading
	if (name == 1)
	{
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
	}

	//second page loading
	else if (name == 2)
	{

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
	}
	return;
}

//Sets first page and the story background
void Intro::loadMedia()
{
	frontPage = SDL_LoadBMP("../assets/story/firstPage.png");
	frontPageTexture = SDL_CreateTextureFromSurface(renderer, frontPage);
	plot = SDL_LoadBMP("../assets/story/plot.png");
	plotTexture = SDL_CreateTextureFromSurface(renderer, plot);
}
int Intro::displayPlot()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, plotTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
	//intro
	//scrolling? -- auto scroll or manual??
	//auto scroll-- skip??
	//random key press: skip plot
	//return;
	//next function called is choose player.
	return 0;
}

int Intro::displayStartingPage()
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, frontPageTexture, NULL, NULL);
	SDL_RenderPresent(renderer);
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
				default:
					return 0;
				}
		//displayLines();
	}

	//single image
	//random key press: goes to intro
	//escape or other quit method: goes to
}

void Intro::introLoop()
{
	int temp = displayStartingPage();
	if (temp==-1){
		Menu::exitMenu();
	}
}

void Intro::initIntro(SDL_Renderer *srcRender)
{
	loadText(1);
	loadText(2);
	loadMedia();
	renderer = srcRender;
}