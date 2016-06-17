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

		void Render() const;
		void Update(float deltaTime_f);
		void HandleEvents();

	private:
		std::vector<Button*> m_buttons;

		Texture m_background;

		Bgm menuTheme;
};
