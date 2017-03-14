#pragma once

#include "game/application.h"
#include "game/selectionstate.h"
#include "game/optionsstate.h"

#include "entity/cursor.h"

#include "gfx/texture.h"
#include "gfx/particle.h"

#include "sfx/music.h"

#include "utils/gui/button.h"

//TODO COMBINE MENU AND OPTIONS STATE

class Menu : public GameState
{
public:
	Menu(bool playBgm, GLfloat intialScroll = 0);
	~Menu();

	void render();
	void update(float deltaTime_f);
	void handleEvents();

	void setInitialScroll(GLfloat x) { m_backgroundX = x; }

private:
	Cursor* m_cursor;

	std::vector<Button*> m_buttons;

	GLfloat m_backgroundX;
	Texture m_background;
	Texture m_backgroundLogo;
	GLfloat m_backgroundAlpha;
	GLfloat m_alphaChange;

	Bgm m_menuTheme;
};
