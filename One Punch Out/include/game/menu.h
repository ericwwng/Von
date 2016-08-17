#pragma once

#include "game/gamestate.h"
#include "game/level.h"
#include "game/selectionstate.h"

#include "entity/player.h"

#include "gfx/texture.h"

#include "sfx/music.h"

#include "utils/gui/button.h"

class Menu : public GameState
{
	public:
		Menu();
		~Menu();

		void render() const;
		void update(
			float deltaTime_f);
		void handleEvents();

	private:
		std::vector<Button*> m_buttons;

		Texture m_background;

		Bgm m_menuTheme;
};
