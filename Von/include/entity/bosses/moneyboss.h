#pragma once

#include "entity/boss.h"
#include "entity/player.h"

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
		void update(float deltaTime);

		void phaseOne();
		void phaseTwo();

		void random360();
		void aimedShot();
		void explodeAttack(Vector2f position);

	private:
		Timer m_collisionTimer;

		unsigned int m_shootingRate;
		unsigned int m_shootingSpeed;
};
