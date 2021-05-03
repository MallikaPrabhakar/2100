#ifndef THEME_H
#define THEME_H

#include <SDL.h>
#include <string>

using namespace std;

#define THEMECOUNT 2

#define THEME1_FONT {255, 255, 255}
#define THEME1_BACKGROUND {0, 0, 0}
#define THEME1 "../assets/themes/basic/"

#define THEME2_FONT {0, 0, 0}
#define THEME2_BACKGROUND {255, 255, 255}
#define THEME2 "../assets/themes/basic/"

class Theme
{
public:
	static SDL_Color textColor, backgroundColor;
	static string themeSource;

	static void setTheme(int themeNumber, SDL_Renderer *renderer);
};

#endif
