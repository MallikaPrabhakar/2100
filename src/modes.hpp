#ifndef MODES_H
#define MODES_H

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

#define WAIT 100
#define PREVIEW_SIZE 16

using namespace std;

struct Modes
{
	enum modes
	{
		FRONT_PAGE,
		MAIN_MENU,
		THEME,
		MAP,
		CONNECT,
		LOOK,
		STORY,
		RULES,
		PLAY,
		INIT,
		QUIT = -1
	};

	static modes mode;
	static SDL_Renderer *renderer;
	static SDL_Texture *frontPageTexture, *storyTexture, *renderTexture, *mapTexture;
	static SDL_Rect mapRect;
	static vector<string> story, rules, displayLines;

	static int displayStory();
	static int displayFrontPage();
	static int displayRules();
	static int themeMenu();
	static int mapMenu();
	static int connectMenu();
	static int displayPage(function<bool(SDL_Event)> operation);
	static bool crawlText(int index);
	static int loadMedia();
	static int loadText();
	static int initModes(SDL_Renderer *srcRenderer);
};

#endif
