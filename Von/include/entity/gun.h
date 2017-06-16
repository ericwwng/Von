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
    void update(Vector2f& position, float angle, float deltaTime, Camera* camera);

	inline void setDirection(Vector2f& dir) { m_weaponDirection = dir; }

	Projectile* getProjectiles() { return m_bullets; };

	//Updates the roatation in relation to the direction the player is facing
	Vector2f updateRotation();

	const int getBulletAmount() { return BULLET_AMOUNT; }
private:
	const int BULLET_AMOUNT = 3;
    Projectile* m_bullets;

	Vector2f m_weaponDirection;
	Vector2f m_playerCenter;
	Vector2f m_initialPosition;

	Timer m_rateOfFire;

	Sfx m_shootSfx;
};
