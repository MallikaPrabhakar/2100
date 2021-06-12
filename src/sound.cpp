#include "sound.hpp"

int Sound::initSounds()
{
	loop = Mix_LoadMUS(LOOP_SOUND);

	start = Mix_LoadWAV(GAME_START);
	win = Mix_LoadWAV(GAME_WIN);
	lose = Mix_LoadWAV(GAME_LOSE);

	correct = Mix_LoadWAV(CORRECT_CLICK);
	incorrect = Mix_LoadWAV(INCORRECT_CLICK);

	spawnable = Mix_LoadWAV(SPAWN_SPAWNABLE);
	flag = Mix_LoadWAV(SPAWN_FLAG);
	shoot = Mix_LoadWAV(SHOOT);

	bomb = Mix_LoadWAV(BOMB_HIT);
	health = Mix_LoadWAV(HEALTH_HIT);
	flag = Mix_LoadWAV(FLAG_HIT);
	bullet = Mix_LoadWAV(BULLET_HIT);

	return 0;
}
