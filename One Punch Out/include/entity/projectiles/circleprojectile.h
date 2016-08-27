#pragma once

#include "entity/projectile.h"

class CircleProjectile : public Projectile
{
	public:
		CircleProjectile();
		~CircleProjectile() = default;

		void setCircleType(
			GLubyte circleType);
		void loadCircleTexture(
			char* filename);

		inline void setScale(GLfloat scale) { m_scale = scale; }
		inline void setColor(SDL_Color color) { m_color = color; }

		void update(
			float deltaTime);
		void render() const;

		CircleBB getCollisionCircle() { return m_collisionCircle; }
		AABB getCollisionBox() { return m_collisionBox; }
	private:
		CircleBB m_collisionCircle;

		GLfloat m_scale;

		SDL_Color m_color;
};
