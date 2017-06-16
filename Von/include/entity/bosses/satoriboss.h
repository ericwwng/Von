/*
Boss that shoots projectiles according to the music
*/

#pragma once

#include "entity/boss.h"
#include "entity/player.h"
#include "entity/warp.h"

#include "utils/timer.h"

#include "physics/AABB.h"

#include "gfx/texture.h"
#include "gfx/texutils.h"

class Satori : public Boss
{
public:
	Satori();
	~Satori();

	void render();
	void update(float deltaTime, Player* player);

	//All attacks are hard coded to a point in the song
	//returns a boolean value to indicate when the attack has ended
	void attack(Player* player);
	bool attack1();
	bool attack2(Player* player);

	void random360(Vector2f position);
	void aimedShot(Vector2f position);
	void explodeAttack(Vector2f position);
private:
	const int MAX_PROJECTILE_AMOUNT = 1000;

	Timer m_songTimer; //Timer up to the length of the song
	Timer m_healthDecreaseTimer; //Health decreases in proportion to song length

	Vector2f m_bulletSpawnPosition;
	Projectile m_spawnCircle;
};
