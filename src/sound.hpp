#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <algorithm>
#include <cassert>

#define LOOP_SOUND "../assets/sounds/loop.wav"

#define GAME_START "../assets/sounds/game_start.wav"
#define GAME_WIN "../assets/sounds/game_win.wav"
#define GAME_LOSE "../assets/sounds/game_lose.wav"

#define CORRECT_CLICK "../assets/sounds/correct_click.wav"
#define INCORRECT_CLICK "../assets/sounds/incorrect_click.wav"

#define SPAWN_SPAWNABLE "../assets/sounds/spawn_spawnable.wav"
#define SPAWN_FLAG "../assets/sounds/spawn_flag.wav"
#define SHOOT "../assets/sounds/shoot.wav"

#define BOMB_HIT "../assets/sounds/bomb_hit.wav"
#define FLAG_HIT "../assets/sounds/flag_hit.wav"
#define HEALTH_HIT "../assets/sounds/health_hit.wav"
#define BULLET_HIT "../assets/sounds/bullet_hit.wav"

struct Sound
{
	static Mix_Music *loop;
	static Mix_Chunk *start, *win, *lose, *correct, *incorrect, *spawnable, *spawn_flag, *shoot, *bombHit, *flagHit, *healthHit, *bulletHit;

	static int initSounds();
	static void playChunk(Mix_Chunk *chunk);
	static void startMusic();
	static void haltMusic();
};

#endif
