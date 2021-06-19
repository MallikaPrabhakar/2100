#ifndef INTRO_H
#define INTRO_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include <fstream>
#include "theme.hpp"
#include "font.hpp"
#include "sound.hpp"
#include "game.hpp"

#define FIRSTPAGE "../assets/res/firstPage.png"

#define PLOTIMG "../assets/res/plot.jpeg"
#define STORYPATH "../assets/res/story.txt"

#define RULESPATH "../assets/res/rules.txt"

#define WAIT 75

using namespace std;

struct Intro
{
	static SDL_Renderer *renderer;
	static SDL_Texture *frontPageTexture;
	static SDL_Texture *plotTexture;
	static SDL_Texture *rulesTexture;
	static vector<string> story, firstPage, rules;

	static int displayPlot();
	static int displayStartingPage();
	static int displayRules();
	static int displayPage(SDL_Texture *texture, vector<string> &vec);
	static bool crawlText(int index, vector<string> &lines);
	static int loadMedia();
	static int loadText();
	static int initIntro(SDL_Renderer *srcRenderer);
};

#endif
