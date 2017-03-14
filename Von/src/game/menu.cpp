#include "game/menu.h"

Menu::Menu(bool playBgm, GLfloat initialScroll) :
	m_backgroundX(initialScroll)
{
	changeFontSize(64);

	m_cursor = new Cursor();

	m_background.loadFromFile("res/GUI/menu-backgroundscroll.png", 1280, 720);
	m_backgroundLogo.loadFromFile("res/GUI/menu-logo.png", 500, 180);

	if(playBgm)
	{
		m_menuTheme.loadMusicFile("res/Music/bgm/Lifeline.ogg");
		m_menuTheme.repeatMusic();
	}

	GLfloat offsetWidth = SCREEN_WIDTH * 0.50;

	m_buttons.push_back(new Button(Vector2f(offsetWidth, SCREEN_HEIGHT / 2), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7, "Play Game", true));
	m_buttons.push_back(new Button(Vector2f(offsetWidth, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7, "Options", true));
	m_buttons.push_back(new Button(Vector2f(offsetWidth, SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7, "Exit Game", true));
	m_buttons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.1, SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)), SCREEN_WIDTH / 6, SCREEN_HEIGHT / 8, "Map Editor", true));
	changeFontSize(16);

	m_backgroundAlpha = 0.2f;
	m_alphaChange = 0.05f;
	
	g_isPlayerDead = false;
}

Menu::~Menu()
{
	delete m_cursor;

	for (Button* button : m_buttons) delete button;
}

void Menu::render()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	m_background.render(m_backgroundX, 0, NULL, (GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);
	m_background.render(m_backgroundX - SCREEN_WIDTH, 0, NULL, (GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);

	glLoadIdentity();

	glTranslatef(0.f, 0.f, 0.f);
	glColor4f(0.5f, 0.5f, 1.f, m_backgroundAlpha);

	glBegin(GL_QUADS);
		glVertex2f(0.f, 0.f);
		glVertex2f((GLfloat)SCREEN_WIDTH, 0);
		glVertex2f((GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);
		glVertex2f(0.f, (GLfloat)SCREEN_HEIGHT);
	glEnd();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	m_backgroundLogo.render((SCREEN_WIDTH / 2) - 250, SCREEN_HEIGHT / 8);

	for (Button* button : m_buttons) button->render();

	m_cursor->render();
}

void Menu::update(float deltaTime)
{
	m_backgroundX += 50 * deltaTime;
	if (m_backgroundX >= SCREEN_WIDTH)
		m_backgroundX = 0;

	if ((m_alphaChange == 0.05f && m_backgroundAlpha >= 0.9f) || (m_alphaChange == -0.05f && m_backgroundAlpha <= 0.2f))
		m_alphaChange = -m_alphaChange;

	m_backgroundAlpha += m_alphaChange * deltaTime;

	m_cursor->update(deltaTime, Vector2f(0, 0));
}

void Menu::handleEvents()
{
	while(SDL_PollEvent(&g_event))
	{
		if (g_event.type == SDL_QUIT)
			Application::getInstance().setAppState(APP_EXITING);
	}

	for (Button* button : m_buttons) button->handleEvents(m_cursor->getCollisionBox());

	//Play Game
	if (m_buttons[0]->isClicked())
	{
		g_gameState = new SelectionState(false);
		g_showCollisionBox = false;
	}
	//Options State
	if (m_buttons[1]->isClicked())
	{
		g_gameState = new OptionsState(m_backgroundX);
		g_showCollisionBox = false;
	}
	//Exit Game
	if (m_buttons[2]->isClicked())
		Application::getInstance().setAppState(APP_EXITING);
	//Editor State
	if (m_buttons[3]->isClicked())
	{
		g_gameState = new SelectionState(true);
		g_showCollisionBox = true;
	}
}
