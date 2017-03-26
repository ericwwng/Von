#include "entity/cursor.h"

Cursor::Cursor()
{
	m_texture.loadFromFile("res/Entity/cursor.png");

	m_particleEmitter = new ParticleEmitter(40, m_position, color(192, 128, 192, 255), 0.5, 0.25, 4);
}

void Cursor::render() const
{
	m_particleEmitter->render();

	Vector2f rotationPoint = { m_texture.getWidth() / 2.f, m_texture.getHeight() / 2.f };
	m_texture.render(m_position.x - m_texture.getWidth() / 2.f, m_position.y - m_texture.getHeight() / 2.f,
		NULL, NULL, NULL, m_angle, &rotationPoint);

}

void Cursor::update(float deltaTime, Vector2f cameraPosition)
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	m_position.x = (GLfloat)x + cameraPosition.x;
	m_position.y = (GLfloat)y + cameraPosition.y;
	m_angle += 100 * deltaTime;
	m_collisionBox = { Vector2f(m_position.x, m_position.y), 1, 1 };

	m_particleEmitter->setPosition(Vector2f((GLfloat)x, (GLfloat)y));
	Vector2f _particleVelocity(randFloat(0.1f, -0.1f), randFloat(0.1f, -0.1f));
	m_particleEmitter->update(deltaTime, _particleVelocity);
}