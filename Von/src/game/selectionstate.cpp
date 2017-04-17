#include "game/selectionstate.h"

SelectionState::SelectionState(bool goToEditor) :
	m_goToEditorState(goToEditor)
{
	m_cursor = new Cursor();
	m_camera = new Camera();

	changeFontSize(64);

	m_previewBackground.loadFromFile("res/GUI/menu-backgroundscroll.png", 1280, 720);

	int _index = 0;
	int _skip = 0; //skip . and ..

	printf("Loading levels...\n");

	m_dir = opendir("./Levels");
	if (m_dir)
	{
		while (m_ent = readdir(m_dir))
		{
			if (_skip < 2)
			{
				_skip++;
				continue;
			}
			m_selectionItems.push_back(new SelectionItem(m_ent->d_name, 
				Vector2f(SCREEN_WIDTH * 0.66f, (GLfloat)_index * SCREEN_HEIGHT / 8), SCREEN_WIDTH / 3, SCREEN_HEIGHT / 8));

			_index++;
		}
		closedir(m_dir);
	}
	else
		printf("Error: Opening directory \n");

	m_camera->setCoords(Vector2f(0, 0));

	changeFontSize(16);
}

SelectionState::~SelectionState()
{
	delete m_camera;
	for (unsigned int i = 0; i < m_selectionItems.size(); i++)
		delete m_selectionItems[i];
}

void SelectionState::render()
{
	m_previewBackground.render(0, 0, NULL, (GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);

	for (unsigned int i = 0; i < m_selectionItems.size(); i++)
		if (Collision(m_camera->m_collisionBox, m_selectionItems[i]->getCollisionBox()))
			m_selectionItems[i]->render();

	m_cursor->render();
}

void SelectionState::update(float deltaTime)
{
	for (unsigned int i = 0; i < m_selectionItems.size(); i++)
	{
		if (m_selectionItems[i]->isClicked())
		{
			std::string filepath = "Levels/" + m_selectionItems[i]->getWorldName();

			if (m_goToEditorState) g_gameState = new EditorState(filepath, m_selectionItems[i]->getWorldName());
			else g_gameState = new Level(filepath, m_selectionItems[i]->getWorldName());
		}
	}

	m_cursor->update(deltaTime, m_camera->getPosition());
}

void SelectionState::handleEvents(SDL_Event* event)
{
	for (unsigned int i = 0; i < m_selectionItems.size(); i++)
		m_selectionItems[i]->handleEvents(event, m_cursor->getCollisionBox());

	while(SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
		{
			g_gameState = new Menu(false);
		}
	}

	m_cursor->handleEvents(event);

	if (event->type == SDL_KEYDOWN)
		if (event->key.keysym.sym == SDLK_ESCAPE)
		{
			delete g_gameState;
			g_gameState = new Menu(false);
		}
}
