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
	void phaseOne();
	void phaseTwo();

	void random360();
	void aimedShot(Vector2f position);
	void explodeAttack(Vector2f position);
private:
	const int MAX_PROJECTILE_AMOUNT = 800;

	Timer m_songTimer; //Timer up to the length of the song
	Timer m_healthDecreaseTimer; //Health decreases in proportion to song length

	Vector2f m_bulletSpawnPosition;
	Projectile m_spawnCircle;
};
