#pragma once

#include "entity/weapon.h"
#include "entity/projectiles/bullet.h"

class Gun : public Weapon
{
    public:
        Gun();
        ~Gun();

        void Action();

        void Render() const;
        void Update(Vector2f& pos, float angle, float deltaTime);
    private:
        Projectile* bullet;
};