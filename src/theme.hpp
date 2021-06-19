#ifndef THEME_H
#define THEME_H

#include <SDL.h>
#include <string>
#include <SDL_ttf.h>
#include <SDL_image.h>

#define THEMECOUNT 3

#define THEME1_FONT \
	{               \
		48, 25, 52  \
	}
#define THEME1_ANTIFONT \
	{                   \
		255, 229, 180   \
	}
#define THEME1_BACKGROUND "../assets/themes/theme1/background.tif"
#define THEME1 "../assets/themes/theme1/"

#define THEME2_FONT   \
	{                 \
		255, 229, 180 \
	}
#define THEME2_ANTIFONT \
	{                   \
		48, 25, 52      \
	}
#define THEME2_BACKGROUND "../assets/themes/theme2/background.tif"
#define THEME2 "../assets/themes/theme2/"

#define THEME3_FONT   \
	{                 \
		255, 216, 177 \
	}
#define THEME3_ANTIFONT \
	{                   \
		42, 13, 93      \
	}
#define THEME3_BACKGROUND "../assets/themes/theme3/background.tif"
#define THEME3 "../assets/themes/theme3/"

class Theme
{
public:
	static SDL_Color textColor;
	static std::string themeSource;
	static SDL_Texture *backgroundTexture;

	static void setTheme(int themeNumber, SDL_Renderer *renderer); //sets the background texture
};

#endif
