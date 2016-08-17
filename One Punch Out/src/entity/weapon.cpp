#include "entity/weapon.h"

Vector2f Weapon::updateRotation()
{
	//To store the new x amd y values
	Vector2f _tempPosition;

	//origin, center of player texture
	float cx = m_position.x + 36;
	float cy = m_position.y + 36;

	//Position of left hand
	m_position.x += 64;
	m_position.y += 0;

	m_position.x -= cx;
	m_position.y -= cy;

	//Calcluate coordinates after rotation
	_tempPosition.x = (float)(m_position.x * cos(m_angle * PI / 180) - m_position.y * sin(m_angle * PI / 180));
	_tempPosition.y = (float)(m_position.x * sin(m_angle * PI / 180) + m_position.y * cos(m_angle * PI / 180));

	m_position.x = _tempPosition.x + cx;
	m_position.y = _tempPosition.y + cy;
	
	return m_position;
}