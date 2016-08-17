#include "entity/projectile.h"

void Projectile::reload(
	Vector2f pos,
	Vector2f vel, 
	float a, 
	float s)
{
	m_position = pos;
	m_velocity = vel * s;
	m_projectileSpeed = (int)s;
	m_angle = a;
	m_active = false;
}

void Projectile::setProjectileSpeed(
	int speed)
{
	m_projectileSpeed = speed;
}

void Projectile::setActive(
	bool a)
{
	m_active = a;
}

bool Projectile::isActive() const
{
   return m_active;
}
