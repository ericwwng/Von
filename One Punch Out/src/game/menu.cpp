#include "game/menu.h"

Menu::Menu()
{
	m_background.loadFromFile("res/GUI/menu-background.png", 1280, 720);
	m_menuTheme.loadMusicFile("res/Music/bgm/Firestorm.ogg");
	Mix_VolumeMusic(80);
	m_menuTheme.playMusic();

	GLfloat offsetWidth = SCREEN_WIDTH * 0.75;

	addButton(m_buttons, "Play Game", Vector2f(offsetWidth, SCREEN_HEIGHT / 6), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7);
	addButton(m_buttons, "Map Editor", Vector2f(offsetWidth, SCREEN_HEIGHT / 3), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7);
	addButton(m_buttons, "How to Play", Vector2f(offsetWidth, SCREEN_HEIGHT / 2), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7);
	addButton(m_buttons, "Options", Vector2f(offsetWidth, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7);
	addButton(m_buttons, "Exit Game", Vector2f(offsetWidth, SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7);
}

Menu::~Menu()
{
	for (unsigned int i = 0; i < m_buttons.size(); i++)
		delete m_buttons[i];
}

void Menu::render() const
{
	glPopMatrix();
		glLoadIdentity();
		glScalef((GLfloat)SCREEN_WIDTH / (GLfloat)m_background.getWidth(), (GLfloat)SCREEN_HEIGHT / (GLfloat)m_background.getHeight(), 1.f);
	glPushMatrix();
	m_background.render(0, 0);
	glPopMatrix();
		glScalef((GLfloat)m_background.getWidth() / (GLfloat)SCREEN_WIDTH, (GLfloat)m_background.getHeight() / (GLfloat)SCREEN_HEIGHT, 1.f);
	glPushMatrix();

	for (unsigned int i = 0; i < m_buttons.size(); i++)
		m_buttons[i]->render();

	Cursor::getInstance().render();
}

void Menu::update(
	float deltaTime)
{
	static SDL_Event _exit;
	_exit.type = SDL_QUIT;
	//Play Game
	if (m_buttons[0]->getClicked())
	{
		g_gameState = new SelectionState(false);
		g_showCollisionBox = false;
	}
	//Editor State
	if (m_buttons[1]->getClicked())
	{
		g_gameState = new SelectionState(true);
		g_showCollisionBox = true;
	}
	//Exit Game
	if (m_buttons[4]->getClicked())
		SDL_PushEvent(&_exit);

	Cursor::getInstance().update(deltaTime);
}

void Menu::handleEvents()
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	for (unsigned int i = 0; i < m_buttons.size(); i++)
		m_buttons[i]->handleEvents();
}