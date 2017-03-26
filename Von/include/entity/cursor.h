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
private:
	ParticleEmitter* m_particleEmitter;
};