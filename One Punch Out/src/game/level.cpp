#include "game/level.h"

Level::Level(
	std::string filename,
	std::string worldName)
{
	m_dungeon = new Map(filename, worldName);
	Player::getInstance().setSpawnPosition(m_dungeon->getPlayerSpawn());
	Player::getInstance().setPlayerHealth(3);
}

Level::~Level()
{
	delete m_dungeon;
}

void Level::render() const
{
	Camera::getInstance().update();
	m_dungeon->render();
	m_dungeon->renderSolidTiles();
	if (m_dungeon->hasBoss()) m_dungeon->getBoss()->render();
	Player::getInstance().render();
	Player::getInstance().renderUI();
	Cursor::getInstance().render();
}

void Level::update(
	float deltaTime)
{
	Player::getInstance().update(deltaTime, m_dungeon->getSolids(),
 		m_dungeon->getDimW(), m_dungeon->getDimH(), m_dungeon->getBoss()->getProjectiles());
	if(m_dungeon->hasBoss()) m_dungeon->getBoss()->update(deltaTime);

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

	while(SDL_PollEvent(&g_event))
	{
		if (g_event.type == SDL_QUIT)
		{
			changeFontSize(64);
			m_dungeon->getBgm()->stopMusic();
			g_gameState = new Menu();
		}
	}
}
