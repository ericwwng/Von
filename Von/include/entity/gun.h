#pragma once

#include "utils/timer.h"

#include "sfx/music.h"

#include "entity/entity.h"
#include "entity/projectile.h"

class Gun : public Entity
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

	void setDirection(Vector2f& dir) { m_weaponDirection = dir; }

	Projectile* getProjectile() { return m_bullet; };

	Vector2f updateRotation();
private:
    Projectile* m_bullet;

	Vector2f m_weaponDirection;

	Timer m_rateOfFire;

	Sfx m_shootSfx;
};
