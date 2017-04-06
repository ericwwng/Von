/*
Weapons that shoots bullets
*/

#pragma once

#include "sfx/music.h"

#include "entity/projectile.h"

class Gun : public Entity
{
public:
    Gun();
    ~Gun();

    void action();

    void render() const;
    void update(Vector2f& position, float angle, float deltaTime);

	inline void setDirection(Vector2f& dir) { m_weaponDirection = dir; }

	Projectile* getProjectile() { return m_bullet; };

	//Updates the roatation in relation to the direction the player is facing
	Vector2f updateRotation();
private:
    Projectile* m_bullet;

	Vector2f m_weaponDirection;
	Vector2f m_playerCenter;
	Vector2f m_initialPosition;

	Timer m_rateOfFire;

	Sfx m_shootSfx;
};
