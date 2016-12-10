#pragma once

#include "entity/boss.h"
#include "entity/player.h"

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
		void update(float deltaTime);

		void phaseOne();
		void phaseTwo();
		void randomPhase();

		void random360();
		void aimedShot();
		void explodeAttack(Vector2f position);

	private:
		Timer m_songTimer;
		Timer m_healthDecreaseTimer;
		Timer m_speedUpTimer;
		Timer m_collisionTimer;

		unsigned int m_shootingRate;
		unsigned int m_shootingSpeed;

		Vector2f m_bulletSpawnPosition;
		Projectile m_spawnCircle;
};
