/*
Removed in final release
*/

#pragma once

#include "physics/AABB.h"

#include "gfx/texture.h"

#include "sfx/music.h"

#include "entity/player.h"

#include "utils/general.h"
#include "utils/vector2f.h"

class SelectionItem
{
public:
	SelectionItem() {};
	SelectionItem(
		std::string path, 
		Vector2f pos, 
		GLuint w, 
		GLuint h);
	~SelectionItem();

	void render() const;
	void handleEvents(SDL_Event* event, AABB cursorCollisionBox);

	bool isClicked() { return m_click; }
		
	void updatePosition();
	std::string getWorldName() { return m_worldName; }

	AABB getCollisionBox() { return m_collisionBox; }
private:
	AABB m_collisionBox;

	Texture m_messageTex;

	std::string m_worldName;

	bool m_hover;
	bool m_click;

	Sfx m_menuHover;
	Sfx m_menuClick;
};