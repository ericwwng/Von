#pragma once

#include <cstring>
#include <sstream>

#include "game/gamestate.h"
#include "game/level.h"
#include "game/camera.h"
#include "game/map.h"

#include "entity/player.h" 

#include "gfx/texture.h"

#include "utils/dirent.h"
#include "utils/gui/button.h"

class EditorState : public GameState
{
	public:
		EditorState(std::string filename);
		~EditorState();

		void Render() const;
		void Update(float deltaTime);
		void HandleEvents();
	private:
		DIR* dir;
		struct dirent *ent;

		bool goToEditorState;

		Map* m_dungeon;

		Uint8 id;
		std::stringstream m_selectorID;
		Texture m_selectorText;
};