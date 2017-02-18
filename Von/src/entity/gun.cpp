#include "entity/gun.h"

Gun::Gun()
{
	m_angle = 0.f;
	m_texture.loadFromFile("res/Entity/gun.png");

	m_bullet = new Projectile();

	m_shootSfx.loadSoundFile("res/Music/sfx/shoot.wav");

	m_bullet->loadTexture("res/Projectile/bullet.png");
	m_bullet->setCollisionBox(8, 8);
	m_bullet->setCenteredBox(false);

	m_rateOfFire.start();
}

Gun::~Gun()
{
	
}

void Gun::action()
{
	if (m_rateOfFire.getTicks() >= 400)
	{
		m_bullet->reload(m_position, m_weaponDirection, m_angle, 2500);
		m_bullet->setActive(true);
		m_rateOfFire.start();
		m_shootSfx.playSound();
	}
}

void Gun::render() const
{
	Vector2f _rotationPoint = { 0.f, 0.f };
	m_texture.render(m_position.x, m_position.y, NULL, NULL, NULL, m_angle, &_rotationPoint);
	if (m_bullet->isActive())
		m_bullet->render();
}

void Gun::update(
	Vector2f& pos,
	float angle,
	float deltaTime)
{
	m_position = pos;
	m_angle = angle;

	m_position = updateRotation();

	m_collisionBox = { m_position, 32, 16 };

	m_bullet->update(deltaTime);
}

Vector2f Gun::updateRotation()
{
	//To store the new x amd y values
	Vector2f _tempPosition;

	//origin, center of player texture
	auto _cx = m_position.x + 32.f;
	auto _cy = m_position.y + 32.f;

	//Position of left hand
	m_position.x += 58;
	m_position.y += 0;

	m_position.x -= _cx;
	m_position.y -= _cy;

	//Calcluate coordinates after rotation
	_tempPosition.x = (float)(m_position.x * cos(m_angle * PI / 180) -
		m_position.y * sin(m_angle * PI / 180));
	_tempPosition.y = (float)(m_position.x * sin(m_angle * PI / 180) +
		m_position.y * cos(m_angle * PI / 180));

	m_position.x = _tempPosition.x + _cx;
	m_position.y = _tempPosition.y + _cy;

	return m_position;
}