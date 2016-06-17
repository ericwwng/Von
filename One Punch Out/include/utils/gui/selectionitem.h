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
		SelectionItem(std::string path, Vector2f pos, GLuint w, GLuint h);
		~SelectionItem();

		void Render() const;
		void HandleEvents();

		bool getClicked() { return click; }
		
		void updatePosition();
		std::string getWorldName() { return worldName; }

		AABB getCollisionBox() { return collisionBox; }
	private:
		AABB collisionBox;

		Texture buttonTex;
		Texture messageTex;

		std::string worldName;

		bool hover;
		bool click;

		Sfx menuHover;
		Sfx menuClick;
};

void AddSelectionItem(std::vector<SelectionItem*> &selectionItems, char* path, Vector2f pos, int w, int h);