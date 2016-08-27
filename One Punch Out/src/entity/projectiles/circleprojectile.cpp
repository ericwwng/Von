#include "entity\projectiles\circleprojectile.h"

CircleProjectile::CircleProjectile()
{
	m_texture.loadFromFile("res/Projectile/circleNormal.png", 64, 64, false);
	m_color = color(0, 0, 0, 255);
	m_position = Vector2f(0, 0);
	m_scale = 1.f;
	m_active = false;
}

void CircleProjectile::loadCircleTexture(char* filename)
{
	m_texture.loadFromFile(filename, 64, 64, false);
}

void CircleProjectile::setCircleType(GLubyte circleType)
{
	/*
	0 = circleNormal
	1 = circleClean
	2 = circleShiny
	3 = circleGlow
	4 = circleFat
	*/
	switch (circleType)
	{
	case 0:
		m_texture.loadFromFile("res/Projectile/circleNormal.png", 64, 64, false);
		break;
	case 1:
		m_texture.loadFromFile("res/Projectile/circleClean.png", 64, 64, false);
		break;
	case 2:
		m_texture.loadFromFile("res/Projectile/circleShiny.png", 64, 64, false);
		break;
	case 3:
		m_texture.loadFromFile("res/Projectile/circleGlow.png", 64, 64, false);
		break;
	case 4:
		m_texture.loadFromFile("res/Projectile/circleFat.png", 64, 64, false);
		break;
	}
}

void CircleProjectile::update(float deltaTime)
{
	m_position = m_position + (m_velocity * deltaTime);
	if (m_position.x - Camera::getInstance().m_collisionBox.position.x < 0 || m_position.x - Camera::getInstance().m_collisionBox.position.x > SCREEN_WIDTH)
		setActive(false);
	if (m_position.y - Camera::getInstance().m_collisionBox.position.y < 0 || m_position.y - Camera::getInstance().m_collisionBox.position.y > SCREEN_HEIGHT)
		setActive(false);

	auto _center = (32.f * m_scale) / 2;

	m_collisionCircle = { m_position, (GLuint)(16 * m_scale) };
	m_collisionBox = { Vector2f(m_position.x + _center, m_position.y + _center), 32 * (GLuint)m_scale, 32 * (GLuint)m_scale };
}

void CircleProjectile::render() const
{
	m_texture.render(m_position.x, m_position.y, NULL, 64 * m_scale, 64 * m_scale, 0.f, NULL, m_color);
	GLfloat _center = (32 * m_scale) / 2;

	Rectf box = { m_collisionBox.position.x, m_collisionBox.position.y, 32 * m_scale, 32 * m_scale };
	if (g_showCollisionBox) renderEmptyBox(box, m_color);
}
