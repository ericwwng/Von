#include "entity\projectiles\circleprojectile.h"

CircleProjectile::CircleProjectile()
{
	m_texture.loadFromFile("res/Projectile/circleNormal.png");
	m_color = color(0, 0, 0, 255);
	m_scale = 1.f;
	m_active = false;
}

CircleProjectile::CircleProjectile(
	GLubyte circleType,
	SDL_Color col, 
	GLfloat scale) :
	m_color(col),
	m_scale(scale)
{
	/*
		0 = circleNormal
		1 = circleClean
		2 = circleShiny
		3 = circleGlow
		4 = circleFat
	*/
	m_texture.loadFromFile("res/Projectile/circleNormal.png");
	switch (circleType)
	{
		case 0:
			m_texture.loadFromFile("res/Projectile/circleNormal.png", 64, 64);
		break;
		case 1:
			m_texture.loadFromFile("res/Projectile/circleClean.png", 64, 64);
		break;
		case 2:
			m_texture.loadFromFile("res/Projectile/circleShiny.png", 64, 64);
		break;
		case 3:
			m_texture.loadFromFile("res/Projectile/circleGlow.png", 64, 64);
		break;
		case 4:
			m_texture.loadFromFile("res/Projectile/circleFat.png", 64, 64);
		break;
	}
	m_active = false;
}

CircleProjectile::~CircleProjectile()
{
}

void CircleProjectile::update(float deltaTime)
{
	m_position = m_position + (m_velocity * deltaTime);
	if (m_position.x - Camera::getInstance().m_collisionBox.position.x < 0 || m_position.x - Camera::getInstance().m_collisionBox.position.x > SCREEN_WIDTH)
		setActive(false);
	if (m_position.y - Camera::getInstance().m_collisionBox.position.y < 0 || m_position.y - Camera::getInstance().m_collisionBox.position.y > SCREEN_HEIGHT)
		setActive(false);

	m_collisionCircle = { m_position, (GLuint)(32 * m_scale) };
}

void CircleProjectile::render() const
{
	glPopMatrix();
		glLoadIdentity();
		glScalef(m_scale, m_scale, 0.f);
	glPushMatrix();
	m_texture.render(m_position.x, m_position.y, NULL, 0.f, NULL, m_color);
	Rectf box = { m_position.x, m_position.y, 64 * m_scale, 64 * m_scale };
	if (g_showCollisionBox) renderEmptyBox(box, m_color);
}
