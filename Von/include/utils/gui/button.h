/*
Self-explanatory: button to click on which sets m_active to true, otherwise false
*/

#pragma once

#include <string>
#include <vector>

#include "gfx/texture.h"

#include "physics/AABB.h"

#include "sfx/music.h"

#include "utils/general.h"

class Button
{
public:
	Button(
		Vector2f pos,
		GLuint w,
		GLuint h,
		std::string msg,
		bool clickable); //Whether one can actually click it or not
	~Button();

	void render() const;
	void handleEvents(SDL_Event* event, AABB cursorCollisionBox);

	bool isClicked();
	
	void setColor(SDL_Color color) { m_messageTex.loadFromText(m_message.c_str(), color); }

	void setClick(bool click) { m_click = click; }
private:
	AABB m_collisionBox;

	std::string m_message;

	Texture m_messageTex;

	bool m_hover;
	bool m_click;

	bool m_clickable;

	Sfx m_menuHover;
	Sfx m_menuClick;
};