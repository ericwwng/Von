#pragma once

#include "entity/projectile.h"

class Bullet : public Projectile
{
    public:
        Bullet();
        ~Bullet();

		void update(
			float deltaTime);
		void render() const;
};
