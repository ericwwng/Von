#pragma once

#include "game/gamestate.h"
#include "game/map.h"
#include "game/camera.h"

#include "entity/player.h"

class Level : public GameState
{
    public:
		Level(std::string filename);
        ~Level();

        void Render() const;
        void Update(float deltaTime);
        void HandleEvents();

    private:
		Map* m_dungeon;
		Player* m_player;
};
