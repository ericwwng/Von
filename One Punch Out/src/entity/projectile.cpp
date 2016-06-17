#include "entity/projectile.h"

void Projectile::Reload(Vector2f pos, Vector2f vel, float a, float s)
{
	position = pos;
	velocity = vel * s;
	projectileSpeed = (int)s;
	angle = a;
	active = false;
}

void Projectile::setProjectileSpeed(int speed)
{
    projectileSpeed = speed;
}

void Projectile::setActive(bool a)
{
    active = a;
}

bool Projectile::isActive() const
{
   return active;
}
