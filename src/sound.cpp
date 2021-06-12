#include "sound.hpp"

Mix_Music *Sound::loop;
Mix_Chunk *Sound::start, *Sound::win, *Sound::lose, *Sound::correct, *Sound::incorrect, *Sound::spawnable, *Sound::spawn_flag, *Sound::shoot, *Sound::bombHit, *Sound::flagHit, *Sound::healthHit, *Sound::bulletHit;

int Sound::initSounds()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		return -1;

	loop = Mix_LoadMUS(LOOP_SOUND);
	if (loop == NULL)
		return 1;

	start = Mix_LoadWAV(GAME_START);
	win = Mix_LoadWAV(GAME_WIN);
	lose = Mix_LoadWAV(GAME_LOSE);

	correct = Mix_LoadWAV(CORRECT_CLICK);
	incorrect = Mix_LoadWAV(INCORRECT_CLICK);

	spawnable = Mix_LoadWAV(SPAWN_SPAWNABLE);
	spawn_flag = Mix_LoadWAV(SPAWN_FLAG);
	shoot = Mix_LoadWAV(SHOOT);

	bombHit = Mix_LoadWAV(BOMB_HIT);
	healthHit = Mix_LoadWAV(HEALTH_HIT);
	flagHit = Mix_LoadWAV(FLAG_HIT);
	bulletHit = Mix_LoadWAV(BULLET_HIT);

	return 0;
}

void Sound::playChunk(Mix_Chunk *chunk)
{
	Mix_PlayChannel(-1, chunk, 0);
}

void Sound::startMusic()
{
	if (!Mix_PlayingMusic())
		return Mix_PlayMusic(loop, -1), void();
}

void Sound::haltMusic()
{
	if (Mix_PlayingMusic())
		return Mix_HaltMusic(), void();
}
