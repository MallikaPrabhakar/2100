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
}

void Intro::displayStartingPage()
{
}

void Intro::exitIntro()
{
}

void Intro::initIntro()
{
	loadText(1);
	loadText(2);
}