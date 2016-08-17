#pragma once

#include "utils/general.h"

#include "entity/entity.h"

class Projectile : public Entity
{
    public:
        virtual ~Projectile() {}

		virtual void reload(
			Vector2f pos,
			Vector2f vel,
			float a,
			float s);

		virtual void render() const {}
		virtual void update(
			float deltaTime) {}

        void setProjectileSpeed(
			int speed);

        void setActive(
			bool a);
        bool isActive() const;
    protected:
        int m_projectileSpeed;

        //whether projectile is on screen or not
        bool m_active;

		AABB m_collisionBox;
};
