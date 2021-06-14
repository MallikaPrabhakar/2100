#ifndef INTRO_H
#define INTRO_H

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <fstream>
#include "font.hpp"
#include "game.hpp"

#define FIRSTPAGE "../assets/res/firstPage.png"
#define FIRSTPAGEPATH "../assets/res/firstPage.txt"

#define PLOTIMG "../assets/res/plot.png"
#define STORYPATH "../assets/res/story.txt"

#define RULESIMG "../assets/res/rules.png"
#define RULESPATH "../assets/res/rules.txt"

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
	static int displayPage(SDL_Texture *texture, vector<string> &vec);
	static int loadMedia();
	static int loadText();
	static int initIntro(SDL_Renderer *renderer);	
	
};

#endif
