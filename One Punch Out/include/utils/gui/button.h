#pragma once

#include <string>

#include "physics/AABB.h"

#include "gfx/texture.h"

#include "sfx/music.h"

#include "entity/player.h"

#include "utils/general.h"
#include "utils/vector2f.h"

class Button
{
	public:
		Button() {};
		Button(Vector2f pos, GLuint w, GLuint h, char* msg);
		~Button();

		void Render() const;
		void HandleEvents();

		bool getClicked() { return click; }
	private:
		AABB collisionBox;

		Texture buttonTex;
		Texture messageTex;

		bool hover;
		bool click;

		Sfx menuHover;
		Sfx menuClick;
};

void AddButton(std::vector<Button*> &buttons, char* message, Vector2f pos, GLuint w, GLuint h);