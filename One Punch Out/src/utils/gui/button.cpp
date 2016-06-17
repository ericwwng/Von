#include "utils/gui/button.h"

Button::Button(Vector2f pos, GLuint w, GLuint h, char* msg)
{
	buttonTex.loadFromFile("res/GUI/button.png");
	messageTex.loadFromText(msg, color(0, 0, 0, 255));
	collisionBox = { Vector2f(pos.x - (w / 2), pos.y - (h / 2)), w, h }; //To center the button

	click = false;
}

Button::~Button()
{

}

void Button::Render() const
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

	if(hover)
	{
		glColor4f(1.f, 1.f, 1.f, 0.2f);
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

void Button::HandleEvents()
{
	static bool up;

	if (Collision(Cursor::getInstance().getCollisionBox(), collisionBox))
	{
		hover = true;
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
			}
		}
	}
	else
		hover = false;
}

void AddButton(std::vector<Button*> &buttons, char* message, Vector2f pos, GLuint w, GLuint h)
{
	Button* temp = new Button(pos, w, h, message);
	buttons.push_back(temp);
}