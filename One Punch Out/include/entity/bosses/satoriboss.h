#pragma once

#include "entity/boss.h"

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
		void phaseThree();

		void random360();

	private:
		Timer m_songTimer;
		Timer m_healthDecreaseTimer;
		Timer m_speedUpTimer;
		Timer m_repeatedTimer;

		unsigned int m_shootingRate;
		unsigned int m_shootingSpeed;

		Vector2f m_bulletSpawnPosition;
		CircleProjectile m_spawnCircle;
};
