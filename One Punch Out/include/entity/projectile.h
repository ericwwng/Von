#pragma once

#include "utils/general.h"

#include "entity/entity.h"

class Projectile : public Entity
{
    public:
        virtual ~Projectile() {}

		virtual void Reload(Vector2f pos, Vector2f vel, float a, float s);

		virtual void Render() const {}
		virtual void Update(float deltaTime) {}

        void setProjectileSpeed(int speed);

        void setActive(bool a);
        bool isActive() const;
    protected:
        int projectileSpeed;

        //whether projectile is on screen or not
        bool active;
};
