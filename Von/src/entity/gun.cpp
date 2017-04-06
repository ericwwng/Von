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
	if (m_rateOfFire.getTicks() >= 500)
	{
		//Calculate position to shoot bullet from the edge of the gun
		//Look at updateRotation()

		Vector2f _tempPosition;

		m_initialPosition.x += 96;
		m_initialPosition = m_initialPosition - m_playerCenter;

		_tempPosition.x = (GLfloat)(m_initialPosition.x * cos(m_angle * PI / 180) - m_initialPosition.y * sin(m_angle * PI / 180));
		_tempPosition.y = (GLfloat)(m_initialPosition.x * sin(m_angle * PI / 180) + m_initialPosition.y * cos(m_angle * PI / 180));

		m_initialPosition = _tempPosition + m_playerCenter;

		//Shoot the bullet
		m_bullet->reload(m_initialPosition, m_weaponDirection, m_angle, 2500);
		m_bullet->setActive(true);
		m_rateOfFire.start();
		m_shootSfx.playSound();
	}
}

void Gun::render() const
{
	Vector2f _rotationPoint = { 0.f, 0.f };
	m_texture.render(m_position.x, m_position.y, NULL, NULL, NULL, m_angle, &_rotationPoint);
	if (m_bullet->isActive()) m_bullet->render();
}

void Gun::update(Vector2f& position, float angle, float deltaTime)
{
	m_position = m_initialPosition = position;
	m_playerCenter = Vector2f(m_position.x + 32.f, m_position.y + 32.f);
	m_angle = angle;

	m_position = updateRotation();

	m_collisionBox = { m_position, 32, 16 };

	m_bullet->update(deltaTime);
}

Vector2f Gun::updateRotation()
{
	//To store the new x and y values
	Vector2f _tempPosition;

	//Position of left hand
	m_position.x += 58;
	m_position = m_position - m_playerCenter;

	//Calcluate coordinates after rotation
	_tempPosition.x = (GLfloat)(m_position.x * cos(m_angle * PI / 180) - m_position.y * sin(m_angle * PI / 180));
	_tempPosition.y = (GLfloat)(m_position.x * sin(m_angle * PI / 180) + m_position.y * cos(m_angle * PI / 180));

	m_position = _tempPosition + m_playerCenter;

	return m_position;
}