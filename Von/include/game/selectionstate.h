#pragma once

#include <cstring>

#include "game/gamestate.h"
#include "game/editorstate.h"
#include "game/camera.h"

#include "entity/cursor.h"

#include "gfx/texture.h"

#include "utils/dirent.h"
#include "utils/gui/selectionitem.h"

class SelectionState : public GameState
{
public:
	SelectionState(bool goToEditor);
	~SelectionState();

	void render();
	void update(float deltaTime);
	void handleEvents(SDL_Event* event);
private:
	Camera* m_camera;
	Cursor* m_cursor;

	Texture m_previewBackground;

	std::vector<SelectionItem*> m_selectionItems;

	DIR* m_dir;
	struct dirent *m_ent;

	bool m_goToEditorState;

	Bgm m_menuTheme;
};
