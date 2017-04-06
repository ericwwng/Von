/*
Base entity for all individual bosses to inherit from
*/
#pragma once

#include "entity/projectile.h"
#include "entity/player.h"

class Boss : public Entity
{
public:
	//Extended from Entity superclass
	virtual void render() {};
	virtual void update(float deltaTime, Player* player) {};
	virtual void handleEvents(SDL_Event* event) {};

	Projectile* getProjectiles() { return m_Projectiles; }

	//Individual attacks not here as they are implemented on a case by case basis
protected:
	float m_health;

	//To remove redundancy as every boss at the moment uses phases
	int m_phaseNumber;

	Projectile* m_Projectiles;
	float m_shootingRate;
	float m_shootingSpeed;

	SDL_Color m_healthColor;
	//For the bubbly particle effect in the healthbar
	ParticleEmitter* m_healthBarParticleEmitter;

	Timer m_blinkTimer; //Blinks whenever the boss is hit
	Timer m_collisionTimer; //Player invincibility timer
	Timer m_bossCollisionTimer; //Boss invincibility timer
};
