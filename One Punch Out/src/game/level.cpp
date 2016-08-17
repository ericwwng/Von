#include "game/level.h"

Level::Level(
	std::string filename,
	std::string worldName)
{
	m_dungeon = new Map(filename, worldName);
	Player::getInstance().setSpawnPosition(m_dungeon->getPlayerSpawn());
}

Level::~Level()
{
	delete m_dungeon;
}

void Level::render() const
{
	Camera::getInstance().update();
	m_dungeon->render();
	Player::getInstance().render();
	m_dungeon->renderSolidTiles();
	Player::getInstance().renderUI();
    Cursor::getInstance().render();
}

void Level::update(
	float deltaTime)
{
	Player::getInstance().update(deltaTime, m_dungeon->getSolids(), m_dungeon->getDimW(), m_dungeon->getDimH());
    Cursor::getInstance().update(deltaTime);
}

void Level::handleEvents()
{
	if (g_event.type == SDL_KEYDOWN)
		if (g_event.key.keysym.sym == SDLK_ESCAPE)
		{
			changeFontSize(64);
			delete g_gameState;
			g_gameState = new Menu();
		}

	Player::getInstance().handleEvents();
}
