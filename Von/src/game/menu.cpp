#include "game/menu.h"

Menu::Menu(bool playBgm)
{
	changeFontSize(64);

	m_cursor = new Cursor();
	m_background.loadFromFile("res/GUI/menu-background.png", 1280, 720);
	if(playBgm)
	{
		m_menuTheme.loadMusicFile("res/Music/bgm/Shiver.ogg");
		m_menuTheme.repeatMusic();
	}

	GLfloat offsetWidth = SCREEN_WIDTH * 0.75;

	m_buttons.push_back(new Button(Vector2f(offsetWidth, SCREEN_HEIGHT / 6), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7, "Play Game", true));
	m_buttons.push_back(new Button(Vector2f(offsetWidth, SCREEN_HEIGHT / 3), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7, "Map Editor", true));
	m_buttons.push_back(new Button(Vector2f(offsetWidth, SCREEN_HEIGHT / 2), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7, "Options", true));
	m_buttons.push_back(new Button(Vector2f(offsetWidth, SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7, "Exit Game", true));

	g_isPlayerDead = false;
}

Menu::~Menu()
{
	delete m_cursor;

	for (unsigned int i = 0; i < m_buttons.size(); i++)
		delete m_buttons[i];
}

void Menu::render()
{
	m_background.render(0, 0, NULL, (GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);

	for (unsigned int i = 0; i < m_buttons.size(); i++)
		m_buttons[i]->render();

	m_cursor->render();
}

void Menu::update(float deltaTime)
{
	m_cursor->update(deltaTime, Vector2f(0, 0));
}

void Menu::handleEvents()
{
	while(SDL_PollEvent(&g_event))
	{
		if (g_event.type == SDL_QUIT)
			Application::getInstance().setAppState(APP_EXITING);
	}

	for (unsigned int i = 0; i < m_buttons.size(); i++)
		m_buttons[i]->handleEvents(m_cursor->getCollisionBox());

	//Play Game
	if (m_buttons[0]->isClicked())
	{
		g_gameState = new SelectionState(false);
		g_showCollisionBox = false;
	}
	//Editor State
	if (m_buttons[1]->isClicked())
	{
		g_gameState = new SelectionState(true);
		g_showCollisionBox = true;
	}
	//Options State
	if (m_buttons[2]->isClicked())
	{
		g_gameState = new OptionsState();
		g_showCollisionBox = false;
	}
	//Exit Game
	if (m_buttons[3]->isClicked())
		Application::getInstance().setAppState(APP_EXITING);
}
