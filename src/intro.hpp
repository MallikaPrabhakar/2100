#ifndef INTRO_H
#define INTRO_H

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <fstream>
#include "theme.hpp"
#include "font.hpp"
#include "map.hpp"
#include "networking.hpp"
#include "game.hpp"
#include "menu.hpp"

#define STORYPATH "../assets/story/story.txt"
#define FIRSTPAGEPATH "../assets/story/firstPage.txt"

using namespace std;

struct Intro
{
	static SDL_Renderer *renderer;
	static vector<string> story, firstPage;

	static void displayPlot();
	//DIslpay plot: story dispplay: scroll!
	static void displayStartingPage(); //?
	static void exitIntro();				//goes to menu
	static void loadMedia(int number);			//loads meadia
	static void loadText(int number); 	//1 for first page, 2 for story plot
	static void initIntro(SDL_Renderer *renderer);			//initializes vectors
};

#endif