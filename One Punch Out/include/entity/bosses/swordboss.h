#pragma once

#include "entity/boss.h"
#include "entity/projectiles/circleprojectile.h"

#include "utils/Vector2f.h"

#include "physics/AABB.h"

#include "gfx/texture.h"
#include "gfx/spritesheet.h"

class SwordBoss : public Boss
{
	public:
		SwordBoss();
		~SwordBoss();

		void render();
		void update();

		void phaseOne();
		void phaseTwo();
		void phaseThree();

		Vector2f& getPosition() { return m_position; }

		AABB& getCollisionBox() { return m_collisionBox; }
	private:
		CircleProjectile* m_Projectiles;
};