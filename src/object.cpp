#include "object.hpp"

SDL_Renderer *Object::renderer;
SDL_Texture *Player::hit;
unordered_set<Bullet *> Bullet::bullets;
int Spawnable::healthsOnMap, Spawnable::flagsOnMap;
unordered_map<pair<int, int>, Spawnable *> Spawnable::spawnables;

Object::Object(int dir, SDL_Texture *texture)
{
	this->texture = texture;
	this->dir = dir;
}

Player::Player(int dir) : Object(dir, NULL)
{
	pos.w = pos.h = TILE_SIZE;
	isHit = flags = 0;
	health = MAX_HEALTH;
}

Bullet::Bullet(int x, int y, int dir, SDL_Texture *texture) : Object(dir, texture)
{
	pos.h = pos.w = TILE_SIZE;
	pos.x = x;
	pos.y = y;
}

Spawnable::Spawnable(int x, int y, int effect, SDL_Texture *texture) : Object(0, texture)
{
	pos.h = pos.w = TILE_SIZE;
	pos.x = x;
	pos.y = y;
	healthDelta = effect;
}

void Object::updatePos()
{
	if (dir == 0)
		pos.y -= TILE_SIZE;
	else if (dir == 1)
		pos.x += TILE_SIZE;
	else if (dir == 2)
		pos.y += TILE_SIZE;
	else if (dir == 3)
		pos.x -= TILE_SIZE;
}

void Object::renderObject()
{
	SDL_RenderCopyEx(renderer, texture, NULL, &pos, dir * 90, NULL, SDL_FLIP_NONE);
}

bool Player::updateHealthAndFlags()
{
	pair<int, int> Pos = {pos.x, pos.y};
	if (Spawnable::spawnables.find(Pos) != Spawnable::spawnables.end())
	{
		Spawnable *spawnable = Spawnable::spawnables[Pos];
		Spawnable::spawnables.erase(Pos);
		if (spawnable->healthDelta == 0)
		{
			Sound::playChunk(Sound::flagHit);
			--Spawnable::flagsOnMap, flags++;
			if (flags == FLAG_LIMIT)
				return true;
		}
		else
		{
			spawnable->healthDelta > 0 ? Sound::playChunk(Sound::healthHit) : Sound::playChunk(Sound::bombHit);
			--Spawnable::healthsOnMap, health += spawnable->healthDelta;
			if (health > MAX_HEALTH)
				health = MAX_HEALTH;
			else if (health <= 0)
				return health = 0, true;
		}
	}
	return false;
}

void Player::renderObject()
{
	if (isHit)
		texture = hit, --isHit;
	else
		texture = player;
	Object::renderObject();
}

void Bullet::displayBullets()
{
	for (auto it = bullets.begin(); it != bullets.end(); it++)
		(**it).renderObject();
}

void Spawnable::displaySpawnables()
{
	for (auto it = spawnables.begin(); it != spawnables.end(); it++)
		it->second->renderObject();
}
