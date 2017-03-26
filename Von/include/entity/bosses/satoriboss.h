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

	void phaseOne();
	void phaseTwo();

	void random360();
	void aimedShot(Vector2f position);
	void explodeAttack(Vector2f position);

private:
	const int MAX_PROJECTILE_AMOUNT = 800;

	Timer m_songTimer;
	Timer m_healthDecreaseTimer;
	Timer m_collisionTimer;

	Vector2f m_bulletSpawnPosition;
	Projectile m_spawnCircle;
};
