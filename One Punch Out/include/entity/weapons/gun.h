#pragma once

#include "entity/weapon.h"
#include "entity/projectiles/bullet.h"

class Gun : public Weapon
{
    public:
        Gun();
        ~Gun();

        void action();

        void render() const;
        void update(
			Vector2f& pos,
			float angle,
			float deltaTime);
    private:
        Projectile* m_bullets;
};