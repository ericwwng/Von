#include "game/credits.h"

Credits::Credits()
{
	m_creditsTheme.loadMusicFile("res/Music/bgm/Von.ogg", false);
	m_creditsTheme.repeatMusic();

	m_creditsScroll = SCREEN_HEIGHT;

	changeFontSize(48);

	//Load up each line in the CREDITS text file into a string vector
	std::ifstream file("res/GUI/CREDITS");
	std::string line;
	while (std::getline(file, line))
	{
		Texture* texture = new Texture();
		texture->loadFromText(line.c_str(), color(255, 255, 255, 255));
		m_creditsText.push_back(texture);
	}
	
	file.close();
}

Credits::~Credits()
{
	for (Texture* texture : m_creditsText) delete texture;
}

void Credits::render()
{	
	GLfloat y = m_creditsScroll;
	for (Texture* texture : m_creditsText)
	{
		texture->render(SCREEN_WIDTH / 2.f - texture->getWidth() / 2.f, y);
		y += 48;
	}
}

void Credits::update(float deltaTime)
{
	if(m_creditsScroll > (int)(m_creditsText.size() - 5) * -48) m_creditsScroll -= 20 * deltaTime;
}

void Credits::handleEvents(SDL_Event* event)
{
	while (SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
		{
			g_gameState = new Menu(true);
		}
	}

	//TODO: Remove this when releasing game
	if (event->type == SDL_KEYDOWN)
		if (event->key.keysym.sym == SDLK_ESCAPE)
		{
			delete g_gameState;
			g_gameState = new Menu(true);
		}
}
