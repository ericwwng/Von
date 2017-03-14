#pragma once

#include "game/application.h"
#include "game/selectionstate.h"

#include "entity/cursor.h"

#include "gfx/texture.h"

#include "sfx/music.h"

#include "utils/gui/button.h"

class OptionsState : public GameState
{
public:
	OptionsState(GLfloat initialScroll);
	~OptionsState();

	void render();
	void update(float deltaTime_f);
	void handleEvents();


private:
	Cursor* m_cursor;

	std::vector<Button*> m_buttons;

	GLfloat m_backgroundX;
	Texture m_background;
	Texture m_backgroundLogo;

	Bgm m_menuTheme;
};
