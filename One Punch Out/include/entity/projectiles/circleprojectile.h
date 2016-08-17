#pragma once

#include "entity/projectile.h"

class CircleProjectile : public Projectile
{
	public:
		CircleProjectile();
		CircleProjectile(
			GLubyte circleType,
			SDL_Color col,
			GLfloat scale = 1.f);

		~CircleProjectile();

		void update(
			float deltaTime);
		void render() const;

	private:
		CircleBB m_collisionCircle;

		GLfloat m_scale;

		SDL_Color m_color;
};
