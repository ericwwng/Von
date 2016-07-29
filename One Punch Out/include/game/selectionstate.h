#pragma once

#include <cstring>

#include "game/menu.h"
#include "game/gamestate.h"
#include "game/editorstate.h"
#include "game/level.h"
#include "game/camera.h"

#include "entity/player.h" 

#include "gfx/texture.h"

#include "utils/dirent.h"
#include "utils/gui/button.h"
#include "utils/gui/selectionitem.h"

class SelectionState : public GameState
{
	public:
		SelectionState(bool goToEditor);
		~SelectionState();

		void Render() const;
		void Update(float deltaTime);
		void HandleEvents();
	private:
		Texture previewBackground;

		std::vector<SelectionItem*> selectionItems;

		DIR* dir;
		struct dirent *ent;

		bool goToEditorState;
};