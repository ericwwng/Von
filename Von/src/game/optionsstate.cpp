#include "game/optionsstate.h"

OptionsState::OptionsState()
{
	changeFontSize(64);

	m_cursor = new Cursor();
	m_background.loadFromFile("res/GUI/menu-background.png", 1280, 720);

	m_buttons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT / 6), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7,
		"BGM: " + std::to_string((int)g_bgmVolume), false));
	m_buttons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.60, SCREEN_HEIGHT / 6), SCREEN_WIDTH / 20, SCREEN_HEIGHT / 6, "-", true));
	m_buttons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.90, SCREEN_HEIGHT / 6), SCREEN_WIDTH / 20, SCREEN_HEIGHT / 6, "+", true));
	m_buttons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT / 3), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7,
		"SFX: " + std::to_string((int)g_bgmVolume), false));
	m_buttons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.60, SCREEN_HEIGHT / 3), SCREEN_WIDTH / 20, SCREEN_HEIGHT / 6, "-", true));
	m_buttons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.90, SCREEN_HEIGHT / 3), SCREEN_WIDTH / 20, SCREEN_HEIGHT / 6, "+", true));
	m_buttons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT / 2), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7, "Reset Save", true));
	m_buttons.push_back(new Button(Vector2f(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT - (SCREEN_HEIGHT / 3)), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7, "Home", true));
}

OptionsState::~OptionsState()
{
	delete m_cursor;

	for (unsigned int i = 0; i < m_buttons.size(); i++)
		delete m_buttons[i];
}

void OptionsState::render()
{
	m_background.render(0, 0, NULL, (GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);

	for (unsigned int i = 0; i < m_buttons.size(); i++)
		m_buttons[i]->render();

	m_cursor->render();
}

void OptionsState::update(float deltaTime)
{
	//Bgm decrement
	if (m_buttons[1]->isClicked())
	{
		g_bgmVolume -= 20 * deltaTime;
		if (g_bgmVolume < 0) g_bgmVolume = 0;
		Mix_VolumeMusic((int)(MIX_MAX_VOLUME * (g_bgmVolume / 100.f)));

		delete m_buttons[0];
		m_buttons[0] = new Button(Vector2f(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT / 6), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7,
			"BGM: " + std::to_string((int)g_bgmVolume), false);
	}
	//Bgm Increment
	if (m_buttons[2]->isClicked())
	{
		g_bgmVolume += 20 * deltaTime;
		if (g_bgmVolume > 100) g_bgmVolume = 100;
		Mix_VolumeMusic((int)(MIX_MAX_VOLUME * (g_bgmVolume / 100.f)));

		delete m_buttons[0];
		m_buttons[0] = new Button(Vector2f(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT / 6), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7,
			"BGM: " + std::to_string((int)g_bgmVolume), false);
	}

	//Sfx decrement
	if (m_buttons[4]->isClicked())
	{
		g_sfxVolume -= 20 * deltaTime;
		if (g_sfxVolume < 0) g_sfxVolume = 0;
		Mix_VolumeMusic((int)(MIX_MAX_VOLUME * (g_sfxVolume / 100.f)));

		delete m_buttons[3];
		m_buttons[3] = new Button(Vector2f(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT / 3), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7,
			"SFX: " + std::to_string((int)g_sfxVolume), false);
	}
	//Sfx Increment
	if (m_buttons[5]->isClicked())
	{
		g_sfxVolume += 20 * deltaTime;
		if (g_sfxVolume > 100) g_sfxVolume = 100;
		Mix_VolumeMusic((int)(MIX_MAX_VOLUME * (g_sfxVolume / 100.f)));

		delete m_buttons[3];
		m_buttons[3] = new Button(Vector2f(SCREEN_WIDTH * 0.75, SCREEN_HEIGHT / 3), SCREEN_WIDTH / 4, SCREEN_HEIGHT / 7,
			"SFX: " + std::to_string((int)g_sfxVolume), false);
	}

	//Reset Save
	if (m_buttons[7]->isClicked())
	{
		
	}

	//Exit Game
	if (m_buttons[7]->isClicked())
	{
		delete g_gameState;
		g_gameState = new Menu(false);
	}

	m_cursor->update(deltaTime, Vector2f(0, 0));
}

void OptionsState::handleEvents()
{
	for (unsigned int i = 0; i < m_buttons.size(); i++)
		m_buttons[i]->handleEvents(m_cursor->getCollisionBox());

	if (g_event.type == SDL_KEYDOWN)
		if (g_event.key.keysym.sym == SDLK_ESCAPE)
		{
			delete g_gameState;
			g_gameState = new Menu(false);
		}

	while (SDL_PollEvent(&g_event))
	{
		if (g_event.type == SDL_QUIT)
		{
			delete g_gameState;
			g_gameState = new Menu(false);
		}
	}
}
