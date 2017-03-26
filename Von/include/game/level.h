#pragma once

#include "game/gamestate.h"
#include "game/map.h"
#include "game/menu.h"
#include "game/camera.h"

#include "entity/player.h"
#include "entity/warp.h"
#include "entity/bosses/satoriboss.h"
#include "entity/bosses/moneyboss.h"
#include "entity/bosses/sunboss.h"

#include "utils/gui/button.h"

class Level : public GameState
{
public:
	Level(std::string filename, std::string worldName);
    ~Level();

	void screenTransition(float addBy);

    void render();
    void update(float deltaTime);
    void handleEvents(SDL_Event* event);

private:
	Camera* m_camera;
	Map* m_dungeon;
	Boss* m_boss;
	Player* m_player;
	Cursor* m_cursor;

	std::string m_filename;
	std::string m_worldName;

	Bgm m_levelBgm;
	Bgm m_gameOverBgm;

	Timer m_transitionTimer;

	std::vector<Button*> m_gameOverButtons;
	bool m_isGameOver;

	//For Screen Transition
	float m_alpha;
	float m_deltaTime;
	Timer m_fadeTimer;
};
