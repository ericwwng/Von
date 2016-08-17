#include "game/selectionstate.h"

SelectionState::SelectionState(
	bool goToEditor) :
	m_goToEditorState(goToEditor)
{
	changeFontSize(64);

	m_previewBackground.loadFromFile("res/GUI/menu-background.png");

	int index = 0;
	int skip = 0; //skip . and ..

	printf("Loading levels...\n");

	m_dir = opendir("./Levels");
	if (m_dir)
	{
		while (m_ent = readdir(m_dir))
		{
			if (skip < 2)
			{
				skip++;
				continue;
			}
			addSelectionItem(m_selectionItems, m_ent->d_name, Vector2f(SCREEN_WIDTH * 0.66f, (GLfloat)index * SCREEN_HEIGHT / 8), SCREEN_WIDTH / 3, SCREEN_HEIGHT / 8);
			
			index++;
		}
		closedir(m_dir);
	}
	else
		printf("Error: Opening directory \n");

	Camera::getInstance().setCoords(Vector2f(0, 0));

	changeFontSize(16);
}

SelectionState::~SelectionState()
{
	for (unsigned int i = 0; i < m_selectionItems.size(); i++)
		delete m_selectionItems[i];
}

void SelectionState::render() const
{
	Camera::getInstance().update();

	glPopMatrix();
		glTranslatef(Camera::getInstance().m_collisionBox.position.x, Camera::getInstance().m_collisionBox.position.y, 0.f);
		glScalef((GLfloat)SCREEN_WIDTH / (GLfloat)m_previewBackground.getWidth(), (GLfloat)SCREEN_HEIGHT / (GLfloat)m_previewBackground.getHeight(), 1.f);
		glTranslatef(-Camera::getInstance().m_collisionBox.position.x, -Camera::getInstance().m_collisionBox.position.y, 0.f);
	glPushMatrix();
	m_previewBackground.render(Camera::getInstance().m_collisionBox.position.x, Camera::getInstance().m_collisionBox.position.y);
	glPopMatrix();
		glTranslatef(Camera::getInstance().m_collisionBox.position.x, Camera::getInstance().m_collisionBox.position.y, 0.f);
		glScalef((GLfloat)m_previewBackground.getWidth() / (GLfloat)SCREEN_WIDTH, (GLfloat)m_previewBackground.getHeight() / (GLfloat)SCREEN_HEIGHT, 1.f);
		glTranslatef(-Camera::getInstance().m_collisionBox.position.x, -Camera::getInstance().m_collisionBox.position.y, 0.f);
	glPushMatrix();

	for (unsigned int i = 0; i < m_selectionItems.size(); i++)
		if (Collision(Camera::getInstance().m_collisionBox, m_selectionItems[i]->getCollisionBox()))
			m_selectionItems[i]->render();

	Cursor::getInstance().render();
}

void SelectionState::update(
	float deltaTime)
{
	if (Camera::getInstance().m_collisionBox.position.y + Camera::getInstance().m_collisionBox.height > 
		(SCREEN_HEIGHT / 8 * m_selectionItems.size())) Camera::getInstance().m_collisionBox.position.y =
		(SCREEN_HEIGHT / 8 * m_selectionItems.size()) - (float)Camera::getInstance().m_collisionBox.height;
	if (Camera::getInstance().m_collisionBox.position.y < 0) Camera::getInstance().m_collisionBox.position.y = 0;
	
	for (unsigned int i = 0; i < m_selectionItems.size(); i++)
	{
		if (m_selectionItems[i]->getClicked())
		{
			std::string filepath = "Levels/" + m_selectionItems[i]->getWorldName();

			if (m_goToEditorState)
				g_gameState = new EditorState(filepath);
			else
				g_gameState = new Level(filepath, m_selectionItems[i]->getWorldName());
		}
	}

	Cursor::getInstance().update(deltaTime);
}

void SelectionState::handleEvents()
{
	if (g_event.type == SDL_MOUSEWHEEL)
		Camera::getInstance().addCoords(Vector2f(0.f, -g_event.wheel.y * 10.f));

	if (g_event.type == SDL_KEYDOWN)
		if (g_event.key.keysym.sym == SDLK_ESCAPE)
		{
			changeFontSize(64);
			delete g_gameState;
			g_gameState = new Menu();
		}
	for (unsigned int i = 0; i < m_selectionItems.size(); i++)
		m_selectionItems[i]->handleEvents();
}