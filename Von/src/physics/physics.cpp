#include "physics/physics.h"

float lerpApproach(
	float goal,
	float current,
	float deltaTime)
{
    float difference = goal - current;

    if(difference > deltaTime) return current + deltaTime;
    if(difference < -deltaTime) return current - deltaTime;

    return goal;
}

GLdouble distanceSquared(Vector2f a, Vector2f b)
{
	GLdouble deltaX = b.x - a.x;
	GLdouble deltaY = b.y - a.y;
	return deltaX*deltaX + deltaY*deltaY;
}

float randFloat(float max, float low)
{
	return low + static_cast<float>(rand()) / static_cast <float> (RAND_MAX / (max - low));
}