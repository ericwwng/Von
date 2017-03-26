#pragma once

#include "entity/projectile.h"
#include "entity/player.h"

#include "utils/Vector2f.h"

#include "physics/AABB.h"

#include "gfx/texture.h"

class Boss : public Entity
{
public:
	virtual void render() {};
	virtual void update(float deltaTime, Player* player) {};
	virtual void handleEvents(SDL_Event* event) {};

	//This is more of an outline
	void phaseOne() {};
	void phaseTwo() {};
	void phaseThree() {};

	Vector2f& getPosition() { return m_position; }

	AABB& getCollisionBox() { return m_collisionBox; }

	Projectile* getProjectiles() { return m_Projectiles; }
protected:
	float m_health;

	uint8_t m_phaseNumber;

	Projectile* m_Projectiles;

	SDL_Color m_healthColor;
	ParticleEmitter* m_healthBarParticleEmitter;

	Timer m_blinkTimer;
	Timer m_collisionTimer;
	Timer m_bossCollisionTimer;

	float m_shootingRate;
	float m_shootingSpeed;
};
