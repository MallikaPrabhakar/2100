#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "font.hpp"

int whichPlayer(SDL_Renderer *renderer);
void serverMenu(SDL_Renderer *renderer);
void clientMenu(SDL_Renderer *renderer);

#endif
