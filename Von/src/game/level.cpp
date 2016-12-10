#include "game/level.h"

Level::Level(
	std::string filename,
	std::string worldName)
{
	worldName.erase(worldName.end() - 4, worldName.end());

	m_dungeon = new Map(filename, worldName);
	Player::getInstance().setSpawnPosition(m_dungeon->getPlayerSpawn());
	Player::getInstance().setPlayerHealth(3);

	m_boss = NULL;

	printf("%s \n", worldName.c_str());

	if (worldName == "Satori")
	{
		m_boss = new Satori();
		m_levelBgm.loadMusicFile("res/Music/bgm/3rdeyerag.ogg");
		m_levelBgm.playMusic();
	}
	else if (worldName == "BigMoney")
	{
		m_boss = new BigMoney();
		m_levelBgm.loadMusicFile("res/Music/bgm/Big Money.ogg");
		m_levelBgm.repeatMusic();
	}
	else if (worldName == "Transition")
	{
		m_levelBgm.loadMusicFile("res/Music/bgm/lolol.ogg");
		m_levelBgm.playMusic();
	}
	else
	{

	}
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
	if (m_boss) m_boss->render();
	Player::getInstance().render();
	Player::getInstance().renderUI();
	Cursor::getInstance().render();
}

void Level::update(
	float deltaTime)
{
	if (m_boss) m_boss->update(deltaTime);
	Player::getInstance().update(deltaTime, m_dungeon->getSolids(),
 		m_dungeon->getDimW(), m_dungeon->getDimH());

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
			g_gameState = new Menu();
		}
	}
}
