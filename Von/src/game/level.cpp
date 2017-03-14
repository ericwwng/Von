#include "game/level.h"

Level::Level(std::string filename, std::string worldName) :
	m_filename(filename),
	m_worldName(worldName)
{
	worldName.erase(worldName.end() - 4, worldName.end());

	m_dungeon = new Map(filename, worldName);

	m_camera = new Camera();

	m_cursor = new Cursor();

	m_player = new Player();
	m_player->setSpawnPosition(m_dungeon->getPlayerSpawn());
	m_player->setPlayerHealth(m_player->getMaxHealth());

	m_boss = NULL;

	m_gameOverBgm.loadMusicFile("res/Music/bgm/Las Estrellas.ogg");

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
		m_levelBgm.loadMusicFile("res/Music/bgm/Icarus Complex.ogg");
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
	else if (worldName == "Sun")
	{
		m_boss = new Sun();
		m_levelBgm.loadMusicFile("res/Music/bgm/Idea vs Reality.ogg");
		m_levelBgm.playMusic();
	}
	else
	{
		m_boss = new BigMoney();
		m_levelBgm.loadMusicFile("res/Music/bgm/lolol.ogg");
		m_levelBgm.playMusic();
	}

	m_alpha = 255.f;

	m_transitionTimer.start();
	m_fadeTimer.start();

	m_isGameOver = false;

	changeFontSize(128);
	m_gameOverButtons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.50, SCREEN_HEIGHT * 0.25), (GLuint)(SCREEN_WIDTH / 1.5), SCREEN_HEIGHT / 2, "GAME OVER", false));
	m_gameOverButtons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.25, SCREEN_HEIGHT  * 0.75), SCREEN_WIDTH / 3, SCREEN_HEIGHT / 4, "Quit", true));
	m_gameOverButtons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT * 0.75), SCREEN_WIDTH / 3, SCREEN_HEIGHT / 4, "Restart", true));
	changeFontSize(16);
}

Level::~Level()
{
	delete m_camera;
	delete m_dungeon;
	delete m_boss;
	delete m_player;
	delete m_cursor;
	for (Button* button : m_gameOverButtons)
	{
		delete button;
	}
}

void Level::screenTransition(float addBy)
{
	m_alpha += addBy * m_deltaTime;

	glLoadIdentity();

	glTranslatef(0.f, 0.f, 0.f);
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
	m_dungeon->render();
	m_dungeon->renderSolidTiles();

	Warp::getInstance().render();

	if (m_boss) m_boss->render();

	m_player->render();
	m_player->renderUI(m_camera);

	if (m_transitionTimer.getTicks() < 4000) screenTransition(-80.f);

	if (g_isPlayerDead) screenTransition(40.f);
	if (m_isGameOver && m_alpha >= 255)
	{
		g_isPlayerDead = false;
		glLoadIdentity();

		glTranslatef(0.f, 0.f, 0.f);
		glColor3f(0.1f, 0.05f, 0.05f);

		glBegin(GL_QUADS);
			glVertex2f(0.f, 0.f);
			glVertex2f((GLfloat)SCREEN_WIDTH, 0);
			glVertex2f((GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);
			glVertex2f(0.f, (GLfloat)SCREEN_HEIGHT);
		glEnd();
		for (Button* button : m_gameOverButtons)
		{
			button->render();
		}
	}

	m_cursor->render();
}

void Level::update(float deltaTime)
{
	m_deltaTime = deltaTime;

	if (!m_isGameOver)
	{
		m_camera->update();

		m_dungeon->updateScroll(deltaTime);

		if (m_boss) m_boss->update(deltaTime, m_player);
		Warp::getInstance().update(deltaTime);

		m_player->update(deltaTime, m_dungeon->getSolids(),
			m_dungeon->getDimW(), m_dungeon->getDimH(), m_cursor->getPosition(), m_camera);

		//Handle collision with warp block
		if (Collision(m_player->getCollisionBox(), Warp::getInstance().getCollisionBox()))
			Warp::getInstance().goToNextLevel();
	}

	if (g_isPlayerDead && !m_isGameOver)
	{
		m_isGameOver = true;
		m_alpha = 0.f;
		m_gameOverBgm.repeatMusic();
	}

	m_cursor->update(deltaTime, m_camera->getPosition());
}

void Level::handleEvents()
{
	if(!m_isGameOver)
		m_player->handleEvents();

	if (m_isGameOver && m_alpha >= 255)
	{
		for (Button* button : m_gameOverButtons)
			button->handleEvents(m_cursor->getCollisionBox());

		//Exit
		if (m_gameOverButtons[1]->isClicked())
		{
			changeFontSize(64);
			g_gameState = new Menu(true);
		}

		//Restart
		if (m_gameOverButtons[2]->isClicked())
		{
			g_gameState = new Level(m_filename, m_worldName);
		}
	}

	if (g_event.type == SDL_KEYDOWN)
		if (g_event.key.keysym.sym == SDLK_ESCAPE)
		{
			changeFontSize(64);
			delete g_gameState;
			g_gameState = new Menu(true);
		}

	while(SDL_PollEvent(&g_event))
	{
		if (g_event.type == SDL_QUIT)
		{
			changeFontSize(64);
			delete g_gameState;
			g_gameState = new Menu(true);
		}
	}

}
