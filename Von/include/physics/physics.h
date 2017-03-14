#pragma once

#include "utils/vector2f.h"

float lerpApproach(
	float Goal,
	float Current, 
	float deltaTime);

GLdouble distanceSquared(Vector2f a, Vector2f b);

float randFloat(float max, float low = 0.f);