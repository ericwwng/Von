#pragma once

#include "entity/projectile.h"

class CircleProjectile : public Projectile
{
	public:
		CircleProjectile();
		CircleProjectile(GLubyte tileID);

		~CircleProjectile();

		void Update(float deltaTime);
		void Render() const;

	private:
		CircleBB m_collisionCircle;
};
