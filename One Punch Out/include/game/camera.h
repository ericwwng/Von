#pragma once

#include "utils/general.h"
#include <iostream>

#include "utils/general.h"
#include "utils/singleton.h"
#include "utils/Vector2f.h"

#include "physics/AABB.h"

class Camera : public Singleton<Camera>
{
    public:
		Camera() { collisionBox = { Vector2f(0, 0), SCREEN_WIDTH, SCREEN_HEIGHT }; }

		void Update()
		{
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
				glLoadIdentity();

				glTranslatef(-collisionBox.position.x, -collisionBox.position.y, 0.f);

			glPushMatrix();
		}

		void setCoords(Vector2f pos) { collisionBox.position = pos; }
		void addCoords(Vector2f pos) { collisionBox.position = collisionBox.position + pos; }

		AABB collisionBox;
};
