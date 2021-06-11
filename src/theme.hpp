#ifndef THEME_H
#define THEME_H

#include <SDL.h>
#include <string>

using namespace std;

#define THEMECOUNT 3

#define THEME1_FONT {48, 25, 52}
#define THEME1_BACKGROUND {255, 229, 180}
#define THEME1 "../assets/themes/theme1/"

#define THEME2_FONT {255, 229, 180}
#define THEME2_BACKGROUND {48, 25, 52}
#define THEME2 "../assets/themes/theme2/"

#define THEME3_FONT {255, 229, 180}
#define THEME3_BACKGROUND {42, 13, 93}
#define THEME3 "../assets/themes/theme2/"

class Theme
{
public:
	static SDL_Color textColor, backgroundColor;
	static string themeSource;

	static void setTheme(int themeNumber, SDL_Renderer *renderer);
};

#endif
