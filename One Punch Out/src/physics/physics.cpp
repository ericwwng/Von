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
