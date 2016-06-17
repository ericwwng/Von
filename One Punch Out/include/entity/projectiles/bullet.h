#pragma once

#include "entity/projectile.h"

class Bullet : public Projectile
{
    public:
        Bullet();
        ~Bullet();

		void Update(float deltaTime);
		void Render() const;
};
