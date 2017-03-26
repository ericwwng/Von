#include "utils/gui/button.h"

Button::Button(Vector2f pos, GLuint w, GLuint h, std::string msg, bool clickable) :
	m_message(msg)
{
	m_messageTex.loadFromText(m_message.c_str(), color(0, 0, 0, 255));
	m_collisionBox = { Vector2f(pos.x - (w / 2), pos.y - (h / 2)), w, h }; //To center the button

	m_menuHover.loadSoundFile("res/Music/sfx/menuhover.wav");
	m_menuClick.loadSoundFile("res/Music/sfx/menuhit.wav");

	m_click = false;

	m_clickable = clickable;
}

Button::~Button()
{

}

void Button::render() const
{
	Rectf _renderRect = { m_collisionBox.position.x, m_collisionBox.position.y,(GLfloat)m_collisionBox.width,(GLfloat)m_collisionBox.height };
	renderFillRect(_renderRect, color(255, 255, 255, 128));
	renderEmptyBox(_renderRect, color(0, 0, 0, 255));

	m_messageTex.render(m_collisionBox.position.x + (m_collisionBox.width / 6),
		m_collisionBox.position.y + (m_collisionBox.height / 6), NULL, (GLfloat)m_collisionBox.width / 1.5f, (GLfloat)m_collisionBox.height / 1.5f);

	if(m_clickable)
		if (m_hover)
		{
			glPopMatrix();
			glPushMatrix();

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

void Button::handleEvents(SDL_Event* event, AABB cursorCollisionBox)
{
	static bool _up = true;

	if (m_clickable)
	{
		if (Collision(cursorCollisionBox, m_collisionBox))
		{
			m_hover = true;
			if (event->type == SDL_MOUSEMOTION)
				m_menuHover.playSound();

			if (event->type == SDL_MOUSEBUTTONUP)
			{
				_up = true;
				m_click = false;
			}
			if (event->type == SDL_MOUSEBUTTONDOWN)
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
		{
			m_hover = false;
		}
	}
}

bool Button::isClicked()
{
	if (m_click)
	{
		m_click = false;
		return true;
	}

	return false;
}