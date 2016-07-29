#include "physics/physics.h"

float LerpApproach(float Goal, float Current, float deltaTime)
{
    float Difference = Goal - Current;

    if(Difference > deltaTime)
        return Current + deltaTime;
    if(Difference < -deltaTime)
        return Current - deltaTime;

    return Goal;
}

GLdouble DistanceSquared(Vector2f a, Vector2f b)
{
	GLuint deltaX = b.x - a.x;
	GLuint deltaY = b.y - a.y;
	return deltaX*deltaX + deltaY*deltaY;
}