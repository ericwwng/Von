/*
Final boss of the game.
One long, difficult phase followed by a save point into 2 shorter phases
*/
#pragma once

#include "entity/boss.h"
#include "entity/warp.h"

#include "gfx/texture.h"
#include "gfx/texutils.h"

#include "physics/AABB.h"

#include "utils/timer.h"

class Sun : public Boss
{
public:
	Sun();
	~Sun();

	void render();
	void update(float deltaTime, Player* player);

	void phaseOne(float deltaTime);
	void phaseTwo(float deltaTime, Player* player);
	void phaseThree(float deltaTime);

	void random360();
	void aimedShot(Vector2f position);
	void explodeAttack(Vector2f position);
	void implodeAttack();

private:
	const int MAX_PROJECTILE_AMOUNT = 800;
};
