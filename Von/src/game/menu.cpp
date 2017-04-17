#include "game/menu.h"

Menu::Menu(bool playBgm)
{
	changeFontSize(64);

	m_cursor = new Cursor();

	m_background.loadFromFile("res/GUI/menu-backgroundscroll.png", 1280, 720);
	m_backgroundLogo.loadFromFile("res/GUI/menu-logo.png", 500, 180);

	if(playBgm)
	{
		m_menuTheme.loadMusicFile("res/Music/bgm/Lifeline.ogg", false);
		m_menuTheme.repeatMusic();
	}

	GLfloat offsetWidth = SCREEN_WIDTH * 0.50;

	m_menuState = MAIN_MENU;

	m_buttons.push_back(new Button(Vector2f(offsetWidth, SCREEN_HEIGHT / 2), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7, "Play Game", true));
	m_buttons.push_back(new Button(Vector2f(offsetWidth, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7, "Options", true));
	m_buttons.push_back(new Button(Vector2f(offsetWidth, SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7, "Exit Game", true));
	m_buttons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.1, SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)), SCREEN_WIDTH / 6, SCREEN_HEIGHT / 8, "Map Editor", true));

	m_optionsButtons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.50, SCREEN_HEIGHT / 2), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7,
		"Music: " + std::to_string(g_bgmVolume) + "%", false));
	m_optionsButtons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.35, SCREEN_HEIGHT / 2), SCREEN_WIDTH / 20, SCREEN_HEIGHT / 7, "-", true));
	m_optionsButtons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.65, SCREEN_HEIGHT / 2), SCREEN_WIDTH / 20, SCREEN_HEIGHT / 7, "+", true));

	m_optionsButtons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.50, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7,
		"Sound: " + std::to_string(g_sfxVolume) + "%", false));
	m_optionsButtons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.35, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), SCREEN_WIDTH / 20, SCREEN_HEIGHT / 7, "-", true));
	m_optionsButtons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.65, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), SCREEN_WIDTH / 20, SCREEN_HEIGHT / 7, "+", true));

	m_optionsButtons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.10, SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)), SCREEN_WIDTH / 6, SCREEN_HEIGHT / 10, "Reset Save", true));
	m_optionsButtons[6]->setColor(color(255, 0, 0, 255));

	m_optionsButtons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.50, SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)), SCREEN_WIDTH / 6, SCREEN_HEIGHT / 7, "Home", true));
	
	m_optionsButtons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.80, SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)), SCREEN_WIDTH / 6, SCREEN_HEIGHT / 10, "Credits", true));
	m_optionsButtons[8]->setColor(color(0, 0, 255, 255));

	changeFontSize(16);

	m_backgroundAlpha = 0.5f;
	m_alphaChange = 0.05f;
	
	g_isPlayerDead = false;
}

Menu::~Menu()
{
	delete m_cursor;

	for (Button* button : m_buttons) delete button;
	for (Button* button : m_optionsButtons) delete button;
}

void Menu::render()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	m_background.render(m_backgroundX, 0, NULL, (GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);
	m_background.render(m_backgroundX - SCREEN_WIDTH, 0, NULL, (GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);

	glLoadIdentity();
	
	glTranslatef(0.f, 0.f, 0.f);
	glColor4f(0.75f, 0.5f, 1.f, m_backgroundAlpha);
	
	glBegin(GL_QUADS);
		glVertex2f(0.f, 0.f);
		glVertex2f((GLfloat)SCREEN_WIDTH, 0);
		glVertex2f((GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);
		glVertex2f(0.f, (GLfloat)SCREEN_HEIGHT);
	glEnd();
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_backgroundLogo.render((SCREEN_WIDTH / 2) - 250, SCREEN_HEIGHT / 8);

	if (m_menuState == MAIN_MENU)
	{
		for (Button* button : m_buttons) button->render();
	}
	else if (m_menuState == OPTIONS_MENU)
	{
		for (Button* button : m_optionsButtons) button->render();
	}

	m_cursor->render();
}

void Menu::update(float deltaTime)
{
	m_backgroundX += 50 * deltaTime;
	if (m_backgroundX >= SCREEN_WIDTH) m_backgroundX = 0;
	if ((m_alphaChange == 0.05f && m_backgroundAlpha >= 0.8f) || (m_alphaChange == -0.05f && m_backgroundAlpha <= 0.2f)) m_alphaChange = -m_alphaChange;

	m_backgroundAlpha += m_alphaChange * deltaTime;

	m_cursor->update(deltaTime, Vector2f(0, 0));
}

void Menu::handleEvents(SDL_Event* event)
{
	while(SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
		{
			if (m_menuState == MAIN_MENU)
			{
				Application::getInstance().setAppState(APP_EXITING);
			}
			if (m_menuState == OPTIONS_MENU)
			{
				m_menuState = MAIN_MENU;
				m_optionsButtons[7]->setClick(false);
			}
		}
	}

	if (m_menuState == MAIN_MENU)
	{
		for (Button* button : m_buttons) button->handleEvents(event, m_cursor->getCollisionBox());

		//Play Game
		if (m_buttons[0]->isClicked())
		{
			g_gameState = new SelectionState(false);
			g_showCollisionBox = false;
		}
		//Options State
		if (m_buttons[1]->isClicked())
		{
			m_menuState = OPTIONS_MENU;
		}
		//Exit Game
		if (m_buttons[2]->isClicked())
		{
			Application::getInstance().setAppState(APP_EXITING);
		}
		//Editor State
		if (m_buttons[3]->isClicked())
		{
			g_gameState = new SelectionState(true);
			g_showCollisionBox = true;
		}
	}
	else if (m_menuState == OPTIONS_MENU)
	{
		for (Button* button : m_optionsButtons) button->handleEvents(event, m_cursor->getCollisionBox());

		changeFontSize(64);

		//Bgm decrement
		if (m_optionsButtons[1]->isClicked())
		{
			g_bgmVolume -= 5;
			if (g_bgmVolume < 0) g_bgmVolume = 0;
			Mix_VolumeMusic((int)(MIX_MAX_VOLUME * (g_bgmVolume / 100.f)));

			delete m_optionsButtons[0];
			m_optionsButtons[0] = new Button(Vector2f(SCREEN_WIDTH * 0.50, SCREEN_HEIGHT / 2), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7,
				"Music: " + std::to_string(g_bgmVolume) + "%", false);
		}
		//Bgm Increment
		if (m_optionsButtons[2]->isClicked())
		{
			g_bgmVolume += 5;
			if (g_bgmVolume > 100) g_bgmVolume = 100;
			Mix_VolumeMusic((int)(MIX_MAX_VOLUME * (g_bgmVolume / 100.f)));

			delete m_optionsButtons[0];
			m_optionsButtons[0] = new Button(Vector2f(SCREEN_WIDTH * 0.50, SCREEN_HEIGHT / 2), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7,
				"Music: " + std::to_string(g_bgmVolume) + "%", false);
		}

		//Sfx decrement
		if (m_optionsButtons[4]->isClicked())
		{
			g_sfxVolume -= 5;
			if (g_sfxVolume < 0) g_sfxVolume = 0;

			delete m_optionsButtons[3];
			m_optionsButtons[3] = new Button(Vector2f(SCREEN_WIDTH * 0.50, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7,
				"Sound: " + std::to_string(g_sfxVolume) + "%", false);
		}
		//Sfx Increment
		if (m_optionsButtons[5]->isClicked())
		{
			g_sfxVolume += 5;
			if (g_sfxVolume > 100) g_sfxVolume = 100;

			delete m_optionsButtons[3];
			m_optionsButtons[3] = new Button(Vector2f(SCREEN_WIDTH * 0.50, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7,
				"Sound: " + std::to_string(g_sfxVolume) + "%", false);
		}

		//Reset Save
		if (m_optionsButtons[6]->isClicked())
		{

		}

		//Home
		if (m_optionsButtons[7]->isClicked())
		{
			m_menuState = MAIN_MENU;
		}

		//Credits
		if (m_optionsButtons[8]->isClicked())
		{
			g_gameState = new Credits();
		}

		changeFontSize(16);

		if (event->type == SDL_KEYDOWN)
		{
			if (event->key.keysym.sym == SDLK_ESCAPE) m_menuState = MAIN_MENU;
		}
	}

	m_cursor->handleEvents(event);
}
