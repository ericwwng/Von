/*
Credits screen after the player has completed the game
*/

#pragma once

#include <vector>
#include <string>
#include <fstream>

#include "game/gamestate.h"
#include "game/menu.h"

#include "gfx/texture.h"

#include "sfx/music.h"

class Credits : public GameState
{
public:
	Credits();
	~Credits();

	void render();
	void update(float deltaTime_f);
	void handleEvents(SDL_Event* event);
private:
	std::vector<Texture*> m_creditsText;
	float m_creditsScroll;

	Bgm m_creditsTheme;
};
