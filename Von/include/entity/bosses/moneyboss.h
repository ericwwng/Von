#pragma once

#include "entity/boss.h"
#include "entity/warp.h"

#include "utils/timer.h"

#include "physics/AABB.h"

#include "gfx/texture.h"
#include "gfx/texutils.h"

class BigMoney : public Boss
{
public:
	BigMoney();
	~BigMoney();

	void render();
	void update(float deltaTime, Player* player);

	void phaseOne();
	void phaseTwo(float deltaTime, Player* player);
	void phaseThree(float deltaTime);

	void random360();
	void aimedShot(Vector2f position);
	void explodeAttack(Vector2f position);
	void implodeAttack();

private:
	const int MAX_PROJECTILE_AMOUNT = 800;
};
