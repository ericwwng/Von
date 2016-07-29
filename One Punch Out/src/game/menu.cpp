#include "game/menu.h"

Menu::Menu()
{
	m_background.loadFromFile("res/GUI/menu-background.png", 1280, 720);
	menuTheme.loadMusicFile("res/Music/bgm/Firestorm.ogg");
	Mix_VolumeMusic(80);
	menuTheme.playMusic();

	GLfloat offsetWidth = SCREEN_WIDTH * 0.75;

	AddButton(m_buttons, "Play Game", Vector2f(offsetWidth, SCREEN_HEIGHT / 6), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7);
	AddButton(m_buttons, "Map Editor", Vector2f(offsetWidth, SCREEN_HEIGHT / 3), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7);
	AddButton(m_buttons, "How to Play", Vector2f(offsetWidth, SCREEN_HEIGHT / 2), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7);
	AddButton(m_buttons, "Options", Vector2f(offsetWidth, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7);
	AddButton(m_buttons, "Exit Game", Vector2f(offsetWidth, SCREEN_HEIGHT - (SCREEN_HEIGHT / 6)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7);
}

Menu::~Menu()
{
	for (unsigned int i = 0; i < m_buttons.size(); i++)
		delete m_buttons[i];
}

void Menu::Render() const
{
	glPopMatrix();
		glLoadIdentity();
		glScalef((GLfloat)SCREEN_WIDTH / (GLfloat)m_background.getWidth(), (GLfloat)SCREEN_HEIGHT / (GLfloat)m_background.getHeight(), 1.f);
	glPushMatrix();
	m_background.Render(0, 0);
	glPopMatrix();
		glScalef((GLfloat)m_background.getWidth() / (GLfloat)SCREEN_WIDTH, (GLfloat)m_background.getHeight() / (GLfloat)SCREEN_HEIGHT, 1.f);
	glPushMatrix();

	for (unsigned int i = 0; i < m_buttons.size(); i++)
		m_buttons[i]->Render();

	Cursor::getInstance().Render();
}

void Menu::Update(float deltaTime)
{
	static SDL_Event exit;
	exit.type = SDL_QUIT;
	//Play Game
	if (m_buttons[0]->getClicked())
	{
		gameState = new SelectionState(false);
		showCollisionBox = false;
	}
	//Editor State
	if (m_buttons[1]->getClicked())
	{
		gameState = new SelectionState(true);
		showCollisionBox = true;
	}
	//Exit Game
	if (m_buttons[4]->getClicked())
		SDL_PushEvent(&exit);

	Cursor::getInstance().Update(deltaTime);
}

void Menu::HandleEvents()
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	for (unsigned int i = 0; i < m_buttons.size(); i++)
		m_buttons[i]->HandleEvents();
}