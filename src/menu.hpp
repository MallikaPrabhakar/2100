#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include "theme.hpp"
#include "font.hpp"
#include "map.hpp"
#include "networking.hpp"

using namespace std;

int whichPlayer(SDL_Renderer *renderer);
int serverMenu(SDL_Renderer *renderer);
int clientMenu(SDL_Renderer *renderer);

#endif
