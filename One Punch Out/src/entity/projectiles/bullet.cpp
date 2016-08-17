#include "entity/projectiles/bullet.h"

Bullet::Bullet()
{
	m_texture.loadFromFile("res/Projectile/bullet.png");
	m_active = false;
}

Bullet::~Bullet()
{

}

void Bullet::render() const
{
	Vector2f _rotationPoint = { m_texture.getWidth() / 2.f, m_texture.getHeight() / 2.f };
	m_texture.render(m_position.x, m_position.y, NULL, m_angle, &_rotationPoint);
	Rectf _box = { m_position.x, m_position.y, (GLfloat)m_texture.getWidth(), (GLfloat)m_texture.getHeight() };
	if (g_showCollisionBox) renderEmptyBox(_box, color(0, 255, 0, 255));
}

void Bullet::update(
	float deltaTime)
{
	m_position = m_position + (m_velocity * deltaTime);
	if (m_position.x - Camera::getInstance().m_collisionBox.position.x < 0 || m_position.x - Camera::getInstance().m_collisionBox.position.x > SCREEN_WIDTH)
		setActive(false);
	if (m_position.y - Camera::getInstance().m_collisionBox.position.y < 0 || m_position.y - Camera::getInstance().m_collisionBox.position.y > SCREEN_HEIGHT)
		setActive(false);

	m_collisionBox = { m_position, 8, 8 };
}