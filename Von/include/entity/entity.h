/*
Base class for all entity objects to inherit from
*/

#pragma once

#include "gfx/texture.h"

#include "physics/AABB.h"

class Entity
{
public:
    virtual void render() const {};
    virtual void update(float deltaTime) {};

    Vector2f& getPosition() { return m_position; }
	inline void setPosition(Vector2f& position) { m_position = position; }

	Vector2f& getVelocity() { return m_velocity; }
	inline void setVelocity(Vector2f& velocity) { m_velocity = velocity; }

    AABB& getCollisionBox() { return m_collisionBox; }
protected:
    Vector2f m_position;
    Vector2f m_velocity;
	Vector2f m_velocityGoal;
    Vector2f m_direction;

    float m_angle;

    AABB m_collisionBox;

    Texture m_texture;
};
