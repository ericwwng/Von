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
		void update(float deltaTime, Player* player);

		void phaseOne();
		void phaseTwo();
		void randomPhase();

		void random360();
		void aimedShot(Vector2f position);
		void explodeAttack(Vector2f position);

	private:
		const int MAX_PROJECTILE_AMOUNT = 600;

		Timer m_songTimer;
		Timer m_healthDecreaseTimer;
		Timer m_speedUpTimer;
		Timer m_collisionTimer;

		unsigned int m_shootingRate;
		unsigned int m_shootingSpeed;

		Vector2f m_bulletSpawnPosition;
		Projectile m_spawnCircle;
};
