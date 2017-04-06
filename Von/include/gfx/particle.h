/*
Implementation of 2d particle engine
Renders particles from a bank of particles and emits them at a rate according 
	to the total particles and life duration from the particle emitter
*/

#pragma once

#include <vector>
#include <iostream>

#include "gfx/texutils.h"

#include "physics/physics.h"

#include "utils/timer.h"

class Particle
{
public:
	Particle() = default;
	Particle(Vector2f& position, 
		Vector2f& velocity,
		SDL_Color color, 
		float lifeDuration,
		GLfloat radius = 8.f);
	~Particle() = default;

	void render() const;
	bool update(float deltaTime);

	//Particle is updated and rendered if it's alive
	inline void setAlive(bool a) { m_alive = a; }
	inline bool isAlive() const { return m_alive; }

	inline void setColor(SDL_Color color) { m_color = color; }
private:
	Vector2f m_position;
	Vector2f m_velocity;

	bool m_alive;
	float m_lifeDuration;

	SDL_Color m_color;
	GLfloat m_alpha;
	GLfloat m_alphaChange;

	GLfloat m_radius;
};

class ParticleEmitter
{
public:
	ParticleEmitter(int totalParticles,
		Vector2f& position,
		SDL_Color color,
		float lifeDuration,
		float lifeVariance,
		GLfloat radius);
	~ParticleEmitter();

	void update(float deltaTime, Vector2f& velocity);
	void render() const;

	void setEmissionRate(float emissionRate) { m_emissionRate = emissionRate; }

	void setPosition(Vector2f& position);
private:
	Vector2f m_position;

	int m_totalParticles; //Total amount of particles that can be emitted from this emitter
	Particle* m_particles;

	int m_particleCount; //Amount of particles emitted at a given moment

	SDL_Color m_color;

	Timer m_emissionTimer; //For the emission rate
	float m_emissionRate;

	float m_lifeVariance;
	float m_lifeDuration;

	GLfloat m_radius;
};