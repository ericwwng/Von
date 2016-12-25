#include "game/level.h"

Level::Level(
	std::string filename,
	std::string worldName)
{
	worldName.erase(worldName.end() - 4, worldName.end());

	m_dungeon = new Map(filename, worldName);

	m_camera = new Camera();

	m_cursor = new Cursor();

	m_player = new Player();
	m_player->setSpawnPosition(m_dungeon->getPlayerSpawn());
	m_player->setPlayerHealth(3);

	m_boss = NULL;

	m_alpha = 255;

	m_transitionTimer.start();
	m_fadeTimer.start();

	printf("%s \n", worldName.c_str());

	if (worldName == "Introduction")
	{
		m_levelBgm.loadMusicFile("res/Music/bgm/Introduction.ogg");
		m_levelBgm.repeatMusic();
		Warp::getInstance().setPosition(Vector2f(500, 500));
		//Warp::getInstance().setPosition(Vector2f(3900, 1024));
	}
	else if (worldName == "Transition")
	{
		m_levelBgm.loadMusicFile("res/Music/bgm/lolol.ogg");
		m_levelBgm.playMusic();
		Warp::getInstance().setPosition(Vector2f(3900, 328));
	}
	else if (worldName == "BigMoney")
	{
		m_boss = new BigMoney();
		m_levelBgm.loadMusicFile("res/Music/bgm/Big Money.ogg");
		m_levelBgm.playMusic();
	}
	else if (worldName == "Satori")
	{
		m_boss = new Satori();
		m_levelBgm.loadMusicFile("res/Music/bgm/3rdeyerag.ogg");
		m_levelBgm.playMusic();
	}
	else
	{
		m_boss = new BigMoney();
		m_levelBgm.loadMusicFile("res/Music/bgm/lolol.ogg");
		m_levelBgm.playMusic();
	}
}

Level::~Level()
{
	delete m_camera;
	delete m_dungeon;
	delete m_boss;
	delete m_player;
	delete m_cursor;
}

void Level::screenTransition()
{
	if (m_fadeTimer.getTicks() > 1)
	{
		m_alpha -= 2;
		m_fadeTimer.start();
	}

	glPopMatrix();
	glPushMatrix();

	//glTranslatef(0.f, 0.f, 0.f);

	glColor4f(0.f, 0.f, 0.f, m_alpha / 255.f);

	glBegin(GL_QUADS);
		glVertex2f(0.f, 0.f);
		glVertex2f((GLfloat)SCREEN_WIDTH, 0);
		glVertex2f((GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);
		glVertex2f(0.f, (GLfloat)SCREEN_HEIGHT);
	glEnd();
}

void Level::render()
{
	m_camera->update();
	m_dungeon->render();
	m_dungeon->renderSolidTiles();
	
	Warp::getInstance().render();

	m_player->render();
	m_player->renderUI(m_camera);

	if (m_boss) m_boss->render();

	m_cursor->render();

	if (m_transitionTimer.getTicks() < 1000)
		screenTransition();
}

void Level::update(
	float deltaTime)
{
	if (m_boss) m_boss->update(deltaTime, m_player);
	Warp::getInstance().update(deltaTime);

	m_player->update(deltaTime, m_dungeon->getSolids(),
 		m_dungeon->getDimW(), m_dungeon->getDimH(), m_cursor->getPosition(), m_camera);

	m_cursor->update(deltaTime, m_camera->getPosition());

	//Handle collision with warp block
	if (Collision(m_player->getCollisionBox(), Warp::getInstance().getCollisionBox()))
		Warp::getInstance().goToNextLevel();

	//Game over screen
	if (g_isPlayerDead)
	{
		m_player->setPlayerHealth(3);
		g_isPlayerDead = false;
		Warp::getInstance().goToCertainLevel(0);
	}
}

void Level::handleEvents()
{
	m_player->handleEvents();

	if (g_event.type == SDL_KEYDOWN)
		if (g_event.key.keysym.sym == SDLK_ESCAPE)
		{
			changeFontSize(64);
			delete g_gameState;
			g_gameState = new Menu();
		}

	while(SDL_PollEvent(&g_event))
	{
		if (g_event.type == SDL_QUIT)
		{
			changeFontSize(64);
			g_gameState = new Menu();
		}
	}
}
