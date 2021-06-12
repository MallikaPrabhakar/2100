#include "sound.hpp"

Mix_Music *Sound::loop;
Mix_Chunk *Sound::start, *Sound::win, *Sound::lose, *Sound::correct, *Sound::incorrect, *Sound::spawnable, *Sound::spawn_flag, *Sound::shoot, *Sound::bomb, *Sound::flag, *Sound::health, *Sound::bullet;

int Sound::initSounds()
{
	loop = Mix_LoadMUS(LOOP_SOUND);

	start = Mix_LoadWAV(GAME_START);
	win = Mix_LoadWAV(GAME_WIN);
	lose = Mix_LoadWAV(GAME_LOSE);

	correct = Mix_LoadWAV(CORRECT_CLICK);
	incorrect = Mix_LoadWAV(INCORRECT_CLICK);

	spawnable = Mix_LoadWAV(SPAWN_SPAWNABLE);
	spawn_flag = Mix_LoadWAV(SPAWN_FLAG);
	shoot = Mix_LoadWAV(SHOOT);

	bomb = Mix_LoadWAV(BOMB_HIT);
	health = Mix_LoadWAV(HEALTH_HIT);
	flag = Mix_LoadWAV(FLAG_HIT);
	bullet = Mix_LoadWAV(BULLET_HIT);

	return 0;
}
