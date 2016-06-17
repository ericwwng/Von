#include "game/level.h"

Level::Level(std::string filename)
{
	m_dungeon = new Map(filename);
	m_player = new Player(m_dungeon->getPlayerSpawn());
}

Level::~Level()
{
	delete m_dungeon;
}

void Level::Render() const
{
	Camera::getInstance().Update();
	m_dungeon->Render();
	m_player->Render();
	m_dungeon->renderSolidTiles();
	m_player->RenderUI();
    Cursor::getInstance().Render();
}

void Level::Update(float deltaTime)
{
	m_player->Update(deltaTime, m_dungeon->getSolids(), m_dungeon->getDimW(), m_dungeon->getDimH());
    Cursor::getInstance().Update(deltaTime);
}

void Level::HandleEvents()
{
	m_player->HandleEvents();
}
