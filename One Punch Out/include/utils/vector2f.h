#pragma once

#include "utils/general.h"
#include <cmath>

class Vector2f
{
    public:
        Vector2f() : x(0), y(0) {}
		Vector2f(GLfloat X, GLfloat Y) : x(X), y(Y) {}

        const inline Vector2f operator+(const Vector2f& v) const { return Vector2f(x + v.x, y + v.y); }
        const inline Vector2f operator-(const Vector2f& v) const { return Vector2f(x - v.x, y - v.y); }
		const inline Vector2f operator*(GLfloat s) const { return Vector2f(x * s, y * s); }
		const inline Vector2f operator/(GLfloat s) const { return Vector2f(x / s, y / s); }

        const inline float Length() const { return sqrt(x*x + y*y); }
        //Used for comparing multiple distances
        const inline float LengthSqr() const { return (x*x + y*y); }

        const inline Vector2f Normalized() const { return Vector2f((*this) / Length()); }

        GLfloat x, y;
};

struct Rectf
{
	GLfloat x;
	GLfloat y;
	GLfloat w;
	GLfloat h;
};