#include "utils/gui/selectionitem.h"

SelectionItem::SelectionItem(
	std::string path,
	Vector2f pos, 
	GLuint w,
	GLuint h)
{
	m_worldName = path;
	if (path == "0.opo")
	{
		m_messageTex.loadFromText("Boss 1", color(0, 0, 0, 255));
	}
	else
		m_messageTex.loadFromText(path.c_str(), color(0, 0, 0, 255));
	
	
	m_buttonTex.loadFromFile("res/GUI/button.png");
	m_collisionBox = { Vector2f(pos.x, pos.y), w, h };

	m_menuHover.loadSoundFile("res/Music/sfx/menuhover.wav");
	m_menuClick.loadSoundFile("res/Music/sfx/menuhit.wav");
	Mix_VolumeChunk(m_menuClick.m_Chunk, 80);

	m_click = false;
}

SelectionItem::~SelectionItem()
{

}

void SelectionItem::render() const
{
	glPopMatrix();
		glTranslatef(m_collisionBox.position.x, m_collisionBox.position.y, 0.f);
		glScalef((GLfloat)m_collisionBox.width / (GLfloat)m_buttonTex.getWidth(), (GLfloat)m_collisionBox.height / (GLfloat)m_buttonTex.getHeight(), 1.f);
		glTranslatef(-m_collisionBox.position.x, -m_collisionBox.position.y, 0.f);
	glPushMatrix();
	m_buttonTex.render(m_collisionBox.position.x, m_collisionBox.position.y);
	glPopMatrix();
		glTranslatef(m_collisionBox.position.x, m_collisionBox.position.y, 0.f);
		glScalef((GLfloat)m_buttonTex.getWidth() / (GLfloat)m_collisionBox.width, (GLfloat)m_buttonTex.getHeight() / (GLfloat)m_collisionBox.height, 1.f);
		glTranslatef(-m_collisionBox.position.x, -m_collisionBox.position.y, 0.f);
	glPushMatrix();

	glPopMatrix();
		glTranslatef(m_collisionBox.position.x + (m_collisionBox.width / 6), m_collisionBox.position.y + (m_collisionBox.height / 6), 0.f);
		glScalef((GLfloat)m_collisionBox.width / (GLfloat)m_messageTex.getWidth() / 1.5f, (GLfloat)m_collisionBox.height / (GLfloat)m_messageTex.getHeight() / 1.5f, 1.f);
		glTranslatef(-m_collisionBox.position.x - (m_collisionBox.width / 6), -m_collisionBox.position.y - (m_collisionBox.height / 6), 0.f);
	glPushMatrix();
	m_messageTex.render(m_collisionBox.position.x + (m_collisionBox.width / 6), m_collisionBox.position.y + (m_collisionBox.height / 6));
	glPopMatrix();
		glTranslatef(m_collisionBox.position.x + (m_collisionBox.width / 6), m_collisionBox.position.y + (m_collisionBox.height / 6), 0.f);
		glScalef((GLfloat)m_messageTex.getWidth() / (GLfloat)m_collisionBox.width * 1.5f, (GLfloat)m_messageTex.getHeight() / (GLfloat)m_collisionBox.height * 1.5f, 1.f);
		glTranslatef(-m_collisionBox.position.x - (m_collisionBox.width / 6), -m_collisionBox.position.y - (m_collisionBox.height / 6), 0.f);
	glPushMatrix();

	if (m_hover)
	{
		glColor4f(1.f, 1.f, 1.f, 0.5f);
		glTranslatef(m_collisionBox.position.x, m_collisionBox.position.y, 0.f);
		glBegin(GL_QUADS);
			glVertex2f(0.f, 0.f);
			glVertex2f((GLfloat)m_collisionBox.width, 0.f);
			glVertex2f((GLfloat)m_collisionBox.width, (GLfloat)m_collisionBox.height);
			glVertex2f(0.f, (GLfloat)m_collisionBox.height);
		glEnd();
		glTranslatef(-m_collisionBox.position.x, -m_collisionBox.position.y, 0.f);
	}
}

void SelectionItem::handleEvents()
{
	static bool _up;

	if (Collision(Cursor::getInstance().getCollisionBox(), m_collisionBox))
	{
		m_hover = true;
		if (g_event.type == SDL_MOUSEMOTION)
			m_menuHover.playSound();

		if (g_event.type == SDL_MOUSEBUTTONUP)
		{
			_up = true;
			m_click = false;
		}
		if (g_event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (_up)
			{
				m_click = true;
				m_hover = false;
				_up = false;
				m_menuClick.playSound();
			}
		}
	}
	else
		m_hover = false;
}

void SelectionItem::updatePosition()
{

}

void addSelectionItem(
	std::vector<SelectionItem*> &selectionItems,
	char* path,
	Vector2f pos,
	int w, 
	int h)
{
	SelectionItem* _selectionItem = new SelectionItem(path, pos, w, h);
	selectionItems.push_back(_selectionItem);
}