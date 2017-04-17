/*
Handles the cursor sprite, particle effect, and collision for buttons
*/

#pragma once

#include "entity/entity.h"

#include "gfx/particle.h"

class Cursor : public Entity
{
public:
	Cursor();
	~Cursor() = default;

	void render() const;
	void update(float deltaTime, Vector2f cameraPosition);
	void handleEvents(SDL_Event* event); //For updating the particles when cursor moves
private:
	ParticleEmitter* m_particleEmitter;

	float deltaTime;
};