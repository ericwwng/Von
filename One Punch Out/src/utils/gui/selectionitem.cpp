#include "utils/gui/selectionitem.h"

SelectionItem::SelectionItem(std::string path, Vector2f pos, GLuint w, GLuint h)
{
	worldName = path;
	if (path == "0.opo")
	{
		messageTex.loadFromText("Boss 1", color(0, 0, 0, 255));
	}
	else
		messageTex.loadFromText(path.c_str(), color(0, 0, 0, 255));
	
	
	buttonTex.loadFromFile("res/GUI/button.png");
	collisionBox = { Vector2f(pos.x, pos.y), w, h };

	menuHover.loadSoundFile("res/Music/sfx/menuhover.wav");
	menuClick.loadSoundFile("res/Music/sfx/menuhit.wav");
	Mix_VolumeChunk(menuClick.m_Chunk, 80);

	click = false;
}

SelectionItem::~SelectionItem()
{

}

void SelectionItem::Render() const
{
	glPopMatrix();
		glTranslatef(collisionBox.position.x, collisionBox.position.y, 0.f);
		glScalef((GLfloat)collisionBox.width / (GLfloat)buttonTex.getWidth(), (GLfloat)collisionBox.height / (GLfloat)buttonTex.getHeight(), 1.f);
		glTranslatef(-collisionBox.position.x, -collisionBox.position.y, 0.f);
	glPushMatrix();
	buttonTex.Render(collisionBox.position.x, collisionBox.position.y);
	glPopMatrix();
		glTranslatef(collisionBox.position.x, collisionBox.position.y, 0.f);
		glScalef((GLfloat)buttonTex.getWidth() / (GLfloat)collisionBox.width, (GLfloat)buttonTex.getHeight() / (GLfloat)collisionBox.height, 1.f);
		glTranslatef(-collisionBox.position.x, -collisionBox.position.y, 0.f);
	glPushMatrix();

	glPopMatrix();
		glTranslatef(collisionBox.position.x + (collisionBox.width / 6), collisionBox.position.y + (collisionBox.height / 6), 0.f);
		glScalef((GLfloat)collisionBox.width / (GLfloat)messageTex.getWidth() / 1.5f, (GLfloat)collisionBox.height / (GLfloat)messageTex.getHeight() / 1.5f, 1.f);
		glTranslatef(-collisionBox.position.x - (collisionBox.width / 6), -collisionBox.position.y - (collisionBox.height / 6), 0.f);
	glPushMatrix();
	messageTex.Render(collisionBox.position.x + (collisionBox.width / 6), collisionBox.position.y + (collisionBox.height / 6));
	glPopMatrix();
		glTranslatef(collisionBox.position.x + (collisionBox.width / 6), collisionBox.position.y + (collisionBox.height / 6), 0.f);
		glScalef((GLfloat)messageTex.getWidth() / (GLfloat)collisionBox.width * 1.5f, (GLfloat)messageTex.getHeight() / (GLfloat)collisionBox.height * 1.5f, 1.f);
		glTranslatef(-collisionBox.position.x - (collisionBox.width / 6), -collisionBox.position.y - (collisionBox.height / 6), 0.f);
	glPushMatrix();

	if (hover)
	{
		glColor4f(1.f, 1.f, 1.f, 0.5f);
		glTranslatef(collisionBox.position.x, collisionBox.position.y, 0.f);
		glBegin(GL_QUADS);
			glVertex2f(0.f, 0.f);
			glVertex2f((GLfloat)collisionBox.width, 0.f);
			glVertex2f((GLfloat)collisionBox.width, (GLfloat)collisionBox.height);
			glVertex2f(0.f, (GLfloat)collisionBox.height);
		glEnd();
		glTranslatef(-collisionBox.position.x, -collisionBox.position.y, 0.f);
	}
}

void SelectionItem::HandleEvents()
{
	static bool up;

	if (Collision(Cursor::getInstance().getCollisionBox(), collisionBox))
	{
		hover = true;
		if (event.type == SDL_MOUSEMOTION)
			menuHover.playSound();

		if (event.type == SDL_MOUSEBUTTONUP)
		{
			up = true;
			click = false;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (up)
			{
				click = true;
				hover = false;
				up = false;
				menuClick.playSound();
			}
		}
	}
	else
		hover = false;
}

void SelectionItem::updatePosition()
{

}

void AddSelectionItem(std::vector<SelectionItem*> &selectionItems, char* path, Vector2f pos, int w, int h)
{
	SelectionItem* selectionItem = new SelectionItem(path, pos, w, h);
	selectionItems.push_back(selectionItem);
}