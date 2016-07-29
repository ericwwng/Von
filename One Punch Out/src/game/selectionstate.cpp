#include "game/selectionstate.h"

SelectionState::SelectionState(bool goToEditor) :
	goToEditorState(goToEditor)
{
	changeFontSize(64);

	previewBackground.loadFromFile("res/GUI/menu-background.png");

	int index = 0;
	int skip = 0; //skip . and ..

	printf("Loading levels...\n");

	dir = opendir("./Levels");
	if (dir)
	{
		while (ent = readdir(dir))
		{
			if (skip < 2)
			{
				skip++;
				continue;
			}
			AddSelectionItem(selectionItems, ent->d_name, Vector2f(SCREEN_WIDTH * 0.66f, (GLfloat)index * SCREEN_HEIGHT / 8), SCREEN_WIDTH / 3, SCREEN_HEIGHT / 8);
			
			index++;
		}
		closedir(dir);
	}
	else
		printf("Error: Opening directory \n");

	Camera::getInstance().setCoords(Vector2f(0, 0));

	changeFontSize(16);
}

SelectionState::~SelectionState()
{
	for (unsigned int i = 0; i < selectionItems.size(); i++)
		delete selectionItems[i];
}

void SelectionState::Render() const
{
	Camera::getInstance().Update();

	glPopMatrix();
		glTranslatef(Camera::getInstance().collisionBox.position.x, Camera::getInstance().collisionBox.position.y, 0.f);
		glScalef((GLfloat)SCREEN_WIDTH / (GLfloat)previewBackground.getWidth(), (GLfloat)SCREEN_HEIGHT / (GLfloat)previewBackground.getHeight(), 1.f);
		glTranslatef(-Camera::getInstance().collisionBox.position.x, -Camera::getInstance().collisionBox.position.y, 0.f);
	glPushMatrix();
	previewBackground.Render(Camera::getInstance().collisionBox.position.x, Camera::getInstance().collisionBox.position.y);
	glPopMatrix();
		glTranslatef(Camera::getInstance().collisionBox.position.x, Camera::getInstance().collisionBox.position.y, 0.f);
		glScalef((GLfloat)previewBackground.getWidth() / (GLfloat)SCREEN_WIDTH, (GLfloat)previewBackground.getHeight() / (GLfloat)SCREEN_HEIGHT, 1.f);
		glTranslatef(-Camera::getInstance().collisionBox.position.x, -Camera::getInstance().collisionBox.position.y, 0.f);
	glPushMatrix();

	for (unsigned int i = 0; i < selectionItems.size(); i++)
		if (Collision(Camera::getInstance().collisionBox, selectionItems[i]->getCollisionBox()))
			selectionItems[i]->Render();

	Cursor::getInstance().Render();
}

void SelectionState::Update(float deltaTime)
{
	if (Camera::getInstance().collisionBox.position.y + Camera::getInstance().collisionBox.height >(SCREEN_HEIGHT / 8 * selectionItems.size())) Camera::getInstance().collisionBox.position.y = (SCREEN_HEIGHT / 8 * selectionItems.size()) - (float)Camera::getInstance().collisionBox.height;
	if (Camera::getInstance().collisionBox.position.y < 0) Camera::getInstance().collisionBox.position.y = 0;
	
	for (unsigned int i = 0; i < selectionItems.size(); i++)
	{
		if (selectionItems[i]->getClicked())
		{
			std::string filepath = "Levels/" + selectionItems[i]->getWorldName();

			std::cout << "Loading " << filepath << std::endl;
			if (goToEditorState)
				gameState = new EditorState(filepath);
			else
				gameState = new Level(filepath);
		}
	}

	Cursor::getInstance().Update(deltaTime);
}

void SelectionState::HandleEvents()
{
	if (event.type == SDL_MOUSEWHEEL)
		Camera::getInstance().addCoords(Vector2f(0.f, -event.wheel.y * 10.f));

	if (event.type == SDL_KEYDOWN)
		if (event.key.keysym.sym == SDLK_ESCAPE)
		{
			changeFontSize(64);
			gameState = new Menu();
		}
	for (unsigned int i = 0; i < selectionItems.size(); i++)
		selectionItems[i]->HandleEvents();
}