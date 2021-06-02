#include "intro.hpp"

vector<string> Intro::story, Intro::firstPage;
SDL_Renderer *Intro::renderer;

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

void Intro::loadMedia(int name){

}
void Intro::displayPlot()
{
	//intro
	//scrolling? -- auto scroll or manual??
	//auto scroll-- skip??
	//random key press: skip plot
	//return;
	//next function called is choose player.

}

void Intro::displayStartingPage()
{
	//single image
	//random key press: goes to intro
}

void Intro::exitIntro()
{
}

void Intro::initIntro( SDL_Renderer *srcRender)
{
	loadText(1);
	loadText(2);
	renderer=srcRender;


}