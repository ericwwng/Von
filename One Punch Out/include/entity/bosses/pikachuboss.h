#pragma once

#include <stdlib.h>     /* srand, rand */

#include "entity/boss.h"

#include "utils/Vector2f.h"
#include "utils/timer.h"

#include "physics/AABB.h"

#include "gfx/texture.h"
#include "gfx/spritesheet.h"

class PikachuBoss : public Boss
{
	public:
		PikachuBoss();
		~PikachuBoss();

		void render();
		void update(float deltaTime);

		void phaseOne();

		void attackOne();
		void attackTwo();
		void attackThree();

		void random360();

	private:
		/*
			0-9 Boundaries
			10-
		*/

		Timer m_songTimer;
};
