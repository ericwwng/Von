#pragma once

#include "game/application.h"
#include "game/selectionstate.h"

#include "entity/cursor.h"

#include "gfx/texture.h"
#include "gfx/particle.h"

#include "sfx/music.h"

#include "utils/gui/button.h"

enum MenuState 
{
	MAIN_MENU,
	OPTIONS_MENU
};

class Menu : public GameState
{
public:
	Menu(bool playBgm);
	~Menu();

	void render();
	void update(float deltaTime_f);
	void handleEvents(SDL_Event* event);

	void setInitialScroll(GLfloat x) { m_backgroundX = x; }

private:
	Cursor* m_cursor;

	MenuState m_menuState;

	std::vector<Button*> m_buttons;
	std::vector<Button*> m_optionsButtons;

	GLfloat m_backgroundX;
	Texture m_background;
	Texture m_backgroundLogo;
	GLfloat m_backgroundAlpha;
	GLfloat m_alphaChange;

	Bgm m_menuTheme;
};
