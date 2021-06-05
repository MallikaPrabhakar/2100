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
#define RULESPATH "../assets/story/rules.txt"

using namespace std;

struct Intro
{
	static SDL_Renderer *renderer;
	static SDL_Texture *frontPageTexture;
	static SDL_Texture *plotTexture;
	static SDL_Texture *rulesTexture;
	static vector<string> story, firstPage, rules;

	static int displayPlot();
	//DIslpay plot: story dispplay: scroll!
	static int displayStartingPage(); //?
	static int displayRules();
	static void loadMedia();			//loads meadia
	static void loadText(); 	//1 for first page, 2 for story plot
	static void initIntro(SDL_Renderer *renderer);	
	
};

#endif