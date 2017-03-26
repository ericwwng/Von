#include "entity/projectile.h"

Projectile::Projectile()
{
	m_texture.loadFromFile("res/Projectile/circleNormal.png", 64, 64, false);
	m_color = color(255, 255, 255, 255);
	m_position = Vector2f(0, 0);
	m_baseWidth = 32;
	m_baseHeight = 32;
	m_xScale = 1.f;
	m_yScale = 1.f;
	m_angle = 0.f;
	m_active = false;
	m_centeredBox = false;
}

Projectile::~Projectile()
{
	delete m_particleEmitter;
}

void Projectile::update(float deltaTime)
{
	m_position = m_position + (m_velocity * deltaTime);

	if (m_particleEmitter)
	{
		if (m_centeredBox)
		{
			m_particleEmitter->setPosition(Vector2f(m_position.x + ((m_texture.getWidth() * m_xScale) / 2),
				m_position.y + ((m_texture.getHeight() * m_yScale) / 2)));
		}
		else m_particleEmitter->setPosition(m_position);
		m_particleEmitter->update(deltaTime, Vector2f(randFloat(-1.f, 1.f), randFloat(-1.f, 1.f)));
		if (!m_active) delete m_particleEmitter;
	}

	if(m_centeredBox)
	{
		Vector2f _center = { m_position.x + (m_texture.getWidth() / 2) - ((m_baseWidth * m_xScale) / 2), 
			m_position.y + (m_texture.getHeight() / 2) - ((m_baseHeight * m_yScale) / 2)};
		m_collisionBox = { _center, m_baseWidth * (GLuint)m_xScale, m_baseHeight * (GLuint)m_yScale };
	}
	else m_collisionBox = { m_position, m_baseWidth * (GLuint)m_xScale, m_baseHeight * (GLuint)m_yScale };
}

void Projectile::render() const
{
	if (m_particleEmitter) m_particleEmitter->render();

	Vector2f _rotationPoint = { m_texture.getWidth() / 2.f, m_texture.getHeight() / 2.f };
	m_texture.render(m_position.x, m_position.y, NULL, m_texture.getWidth() * m_xScale, m_texture.getHeight() * m_yScale, m_angle, &_rotationPoint, m_color);

	if (g_showCollisionBox)
	{
		Rectf box = { m_collisionBox.position.x, m_collisionBox.position.y, m_baseWidth * m_xScale, m_baseHeight * m_yScale };
		renderEmptyBox(box, m_color);
	}
}

void Projectile::reload(
	Vector2f position,
	Vector2f velocity,
	float angle,
	float speed)
{
	m_position = position;
	m_velocity = velocity * speed;
	m_projectileSpeed = (int)speed;
	m_angle = angle;
	m_active = false;
}

void Projectile::setScale(GLfloat xscale, GLfloat yscale)
{
	m_xScale = xscale;
	m_yScale = yscale;
}

void Projectile::setCollisionBox(GLuint width, GLuint height)
{
	m_baseWidth = width;
	m_baseHeight = height;
}

void Projectile::setParticleEmitter(ParticleEmitter* particleEmitter)
{
	delete m_particleEmitter;
	m_particleEmitter = particleEmitter;
}