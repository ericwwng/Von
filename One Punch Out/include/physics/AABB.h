#pragma once

#include "physics/physics.h"

#include "utils/general.h"
#include "utils/vector2f.h"

class AABB
{
    public:
        AABB() {}

		AABB(Vector2f& pos, GLuint w, GLuint h)
        {
            position = pos;
            width = w;
            height = h;
        }

        Vector2f position;
		GLuint width, height;
};

class CircleBB
{
	public:
		CircleBB() {}

		CircleBB(Vector2f& pos, GLuint r)
		{
			position = pos;
			radius = r;
		}

		Vector2f position;
		GLuint radius;
};

inline bool Collision(AABB& a, AABB& b)
{
    return (a.position.x < b.position.x + b.width &&
        a.position.x + a.width > b.position.x &&
        a.position.y < b.position.y + b.height &&
        a.height + a.position.y > b.position.y);
};

inline bool Collision(CircleBB& a, CircleBB& b)
{
	return(distanceSquared(a.position, b.position) <
		(a.radius + b.radius) * (a.radius + b.radius));
}

inline bool Collision(CircleBB& a, AABB& b)
{
	//Closest point on collision box
	GLfloat cX, cY;

	if (a.position.x < b.position.x)
		cX = b.position.x;
	else if (a.position.x > b.position.x + b.width)
		cX = b.position.x + b.width;
	else
		cX = a.position.x;

	if (a.position.y < b.position.y)
		cY = b.position.y;
	else if (a.position.y > b.position.y + b.height)
		cY = b.position.y + b.height;
	else
		cY = a.position.y;

	return (distanceSquared(a.position, Vector2f(cX, cY)) < a.radius * a.radius);
}
