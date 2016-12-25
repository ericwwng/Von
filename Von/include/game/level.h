#pragma once

#include "game/gamestate.h"
#include "game/map.h"
#include "game/menu.h"
#include "game/camera.h"

#include "entity/player.h"
#include "entity/warp.h"
#include "entity/bosses/satoriboss.h"
#include "entity/bosses/moneyboss.h"

class Level : public GameState
{
    public:
		Level(
			std::string filename,
			std::string worldName);
        ~Level();

		void screenTransition();

        void render();
        void update(float deltaTime);
        void handleEvents();

    private:
		Camera* m_camera;
		Map* m_dungeon;
		Boss* m_boss;
		Player* m_player;
		Cursor* m_cursor;

		Bgm m_levelBgm;

		Timer m_transitionTimer;

		//For Screen Transition
		int m_alpha;
		Timer m_fadeTimer;
};
