#include "gfx/particle.h"

Particle::Particle(Vector2f& position, Vector2f& velocity, SDL_Color color, float lifeDuration, GLfloat radius) :
	m_position(position),
	m_velocity(velocity),
	m_color(color),
	m_lifeDuration(lifeDuration),
	m_radius(radius)
{
	m_alive = true;
	m_alpha = 255;
	m_alphaChange = (1 / m_lifeDuration) * 250;
}

bool Particle::update(float deltaTime)
{
	m_position = m_position + (m_velocity * deltaTime * 100);

	m_lifeDuration -= deltaTime;

	if(m_alpha > 0) m_alpha -= m_alphaChange * deltaTime;

	if (m_lifeDuration <= 0 || m_alpha <= 0 || m_position.x + (m_radius * 2) < 0 || m_position.x > SCREEN_WIDTH)
	{
		m_alive = false;
		return false;
	}
	return true;
}

void Particle::render() const
{
	glLoadIdentity();

	glTranslatef(m_position.x, m_position.y, 0.f);
	glColor4f(m_color.r / 255.f, m_color.g / 255.f, m_color.b / 255.f, m_alpha / 255.f);

	glBegin(GL_QUADS);
		glVertex2f(0.f, 0.f);
		glVertex2f(m_radius * 2, 0);
		glVertex2f(m_radius * 2, m_radius * 2);
		glVertex2f(0.f, m_radius * 2);
	glEnd();
}

ParticleEmitter::ParticleEmitter(int totalParticles, Vector2f& position, SDL_Color color, float lifeDuration, float lifeVariance, GLfloat radius) :
	m_totalParticles(totalParticles),
	m_color(color),
	m_lifeDuration(lifeDuration),
	m_lifeVariance(lifeVariance),
	m_radius(radius)
{
	m_particles = new Particle[m_totalParticles];

	m_position = position;
	
	m_emissionRate = m_totalParticles / lifeDuration;
	m_emissionTimer.start();

	m_particleCount = 0;
}

ParticleEmitter::~ParticleEmitter()
{
	delete[] m_particles;
}

void ParticleEmitter::update(float deltaTime, Vector2f& velocity)
{
	for (int i = 0; i < m_particleCount; i++)
	{
		if (m_particles[i].isAlive())
		{
			if (!m_particles[i].update(deltaTime)) //If particle just died
			{
				Particle temp = m_particles[i];
				m_particles[i] = m_particles[m_particleCount - 1];
				m_particles[m_particleCount - 1] = temp;
				m_particleCount--;
			}
		}
	}

	if (m_emissionTimer.getTicks() >= (1 / (m_emissionRate / 1000.f)) && m_particleCount < m_totalParticles)
	{
		float _age = randFloat(m_lifeDuration, m_lifeDuration - m_lifeVariance);
		Particle temp(m_position, velocity, m_color, _age, m_radius * (_age / m_lifeDuration));
		m_particles[m_particleCount++] = temp;
		m_emissionTimer.start();
	}
}

void ParticleEmitter::updateParticlesOnly(float deltaTime)
{
	for (int i = 0; i < m_particleCount; i++)
	{
		if (m_particles[i].isAlive())
		{
			if (!m_particles[i].update(deltaTime)) //If particle just died
			{
				Particle temp = m_particles[i];
				m_particles[i] = m_particles[m_particleCount - 1];
				m_particles[m_particleCount - 1] = temp;
				m_particleCount--;
			}
		}
	}
}

void ParticleEmitter::render() const
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	for (int i = 0; i < m_particleCount; i++)
	{
		if (m_particles[i].isAlive()) m_particles[i].render();
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleEmitter::setPosition(Vector2f& position)
{
	m_position = position;
}

void ParticleEmitter::setLifeDuration(float lifeDuration) 
{
	m_lifeDuration = lifeDuration; 
	//m_emissionRate = m_totalParticles / lifeDuration;
}
