#include "entity\bosses\sunboss.h"

Sun::Sun()
{
	m_texture.loadFromFile("res/Enemy/Boss/Sun/SunImage300.png", 300, 300);
	m_Projectiles = new Projectile[MAX_PROJECTILE_AMOUNT];

	m_collisionTimer.start();
	m_bossCollisionTimer.start();
	m_blinkTimer.start();

	m_phaseNumber = 1;
	m_health = 100.f;

	m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f) + 200.f, 200 };

	m_healthColor = color(255, 165, 0, 128);
	m_healthBarParticleEmitter = new ParticleEmitter(100, Vector2f((GLfloat)SCREEN_WIDTH, 32), color(255, 255, 255, 255), 5.f, 2.f, 4.f);

	m_velocityGoal.x = 5000.f;
	m_shootingRate = 1000;
	m_shootingSpeed = 300;

	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		int random = rand() % 5 + 1;
		if (random == 1) m_Projectiles[i].loadTexture("res/Projectile/stone-1.png");
		if (random == 2) m_Projectiles[i].loadTexture("res/Projectile/stone-2.png");
		if (random == 3) m_Projectiles[i].loadTexture("res/Projectile/stone-3.png");
		if (random == 4) m_Projectiles[i].loadTexture("res/Projectile/stone-4.png");
		if (random == 5) m_Projectiles[i].loadTexture("res/Projectile/stone-5.png");

		m_Projectiles[i].setCenteredBox(true);
		m_Projectiles[i].setCollisionBox(40, 40);
	}
}

Sun::~Sun()
{
	delete m_Projectiles;
	delete m_healthBarParticleEmitter;
}

void Sun::render()
{
	if (m_phaseNumber != 0)
	{
		if (m_bossCollisionTimer.getTicks() > 1500) m_texture.render(m_position.x - 150.f, m_position.y - 150.f);
		else
		{
			if (m_blinkTimer.getTicks() < 150) m_texture.render(m_position.x - 150.f, m_position.y - 150.f);
			else if (m_blinkTimer.getTicks() > 200) m_blinkTimer.start();
		}
	}

	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		if (m_Projectiles[i].isActive()) m_Projectiles[i].render();
	}

	m_healthBarParticleEmitter->render();

	Rectf _box = { 0, 0, m_health * 12.8f, 64 };
	renderFillRect(_box, m_healthColor);
	renderEmptyBox(_box, color(0, 0, 0, 255));
}

void Sun::update(float deltaTime, Player* player)
{
	m_collisionBox = { Vector2f(m_position.x - 50, m_position.y - 150), 250, 250 };

	switch (m_phaseNumber)
	{
		case 1:
		{
			phaseOne(deltaTime);
		} break;
		case 2:
		{
			phaseTwo(deltaTime, player);
		} break;
		case 3:
		{
			phaseThree(deltaTime);
		} break;
	}

	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		if (m_Projectiles[i].isActive())
		{
			m_Projectiles[i].update(deltaTime);
		}
	}

	//Check for collision with the projectiles for bosses
	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
		if (m_Projectiles[i].isActive())
			if (Collision(m_Projectiles[i].getCollisionBox(), player->getCollisionBox()) &&
				m_collisionTimer.getTicks() > 2000)
			{
				m_collisionTimer.start();
				if (player->getPlayerHealth() > 0) player->setHit();
			}

	//Special case for implosion attack
	for (int i = 500; i < 700; i++)
	{
		if (m_Projectiles[i].isActive() && m_phaseNumber != 2)
		{
			if (Collision(m_Projectiles[i].getCollisionBox(), m_collisionBox)) m_Projectiles[i].setActive(false);
		}
	}

	//Player hits boss
	if (player->getWeapon()->getProjectile()->isActive())
		if (Collision(m_collisionBox, player->getWeapon()->getProjectile()->getCollisionBox()))
		{
			if (m_bossCollisionTimer.getTicks() > 1500)
			{
				m_bossCollisionTimer.start();
				switch (m_phaseNumber)
				{
					case 1:
					{
						m_health -= 1;
					} break;
					case 2:
					{
						m_health -= 3;
					} break;
					case 3:
					{
						m_health -= 5;
					} break;
				}
			}
			player->getWeapon()->getProjectile()->reload(Vector2f(0, 0), Vector2f(0, 0), 0, 0);
		}

	m_healthBarParticleEmitter->setPosition(Vector2f(m_health * 12.8f - 5, 32));
	m_healthBarParticleEmitter->update(deltaTime, Vector2f(randFloat(-2.f, -4.f), randFloat(0.05f, -0.05f)));

	if (m_phaseNumber == 3 && m_health == 0)
	{
		//Death
	}
}

void Sun::phaseOne(float deltaTime)
{
	static Timer _miniPhaseTimer(true);
	static Vector2f _tempVelocity;
	static int _attackNumber = 0;

	//boss movement
	_tempVelocity.x = lerpApproach(m_velocityGoal.x, _tempVelocity.x, deltaTime * 500);
	_tempVelocity.y = lerpApproach(m_velocityGoal.y, _tempVelocity.y, deltaTime * 500);
	m_velocity = _tempVelocity * deltaTime * 100;
	m_velocity.normalized();

	//Attack 1: go in a square and shoot towards the center
	if (_attackNumber == 0)
	{
		m_position.x += m_velocity.x * deltaTime;
		if (m_position.x - 50 < 0 || m_position.x + m_texture.getWidth() - 200 > SCREEN_WIDTH)
		{
			m_position.x -= m_velocity.x * deltaTime;
			_tempVelocity.y = _tempVelocity.x;
			_tempVelocity.x = 0;
			m_velocityGoal.y = m_velocityGoal.x;
			m_velocityGoal.x = 0;
		}

		m_position.y += m_velocity.y * deltaTime;
		if (m_position.y - 50 < 0 || m_position.y + m_texture.getHeight() - 200 > SCREEN_HEIGHT)
		{
			m_position.y -= m_velocity.y * deltaTime;
			_tempVelocity.x = -_tempVelocity.y;
			_tempVelocity.y = 0;
			m_velocityGoal.x = -m_velocityGoal.y;
			m_velocityGoal.y = 0;
		}
		aimedShot(Vector2f((SCREEN_WIDTH / 2) + randFloat(-50.f, 50.f), SCREEN_HEIGHT / 2 + randFloat(-50.f, 50.f)));
		m_shootingRate -= 40 * deltaTime;
		if (m_shootingRate <= 150) m_shootingRate = 150;

	}

	if (_miniPhaseTimer.getTicks() > 100000)
	{
		_miniPhaseTimer.start();
		
		m_velocityGoal.x = 5000.f;

		/*_attackNumber++;
		if (_attackNumber == 3) _attackNumber = 0;

		if (_attackNumber == 0)
		{
			m_velocityGoal.x = 1000.f;
			m_velocity.x = 0.f;
			_tempVelocity.x = 0.f;
			m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f) + 175.f, 175.f };
		}
		else if (_attackNumber == 2)
		{
			m_velocity.x = 0.f;
			m_velocityGoal.x = 0.f;
			_tempVelocity.x = 0.f;
			m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f) + 175.f, SCREEN_HEIGHT / 2 - (m_texture.getHeight() / 4.f) };

			m_shootingRate = 100;
			m_shootingSpeed = 600;
		}*/
	}

	if (m_health <= 0)
	{
		m_health = 100;
		m_healthColor = color(0, 0, 255, 128);
		m_phaseNumber = 2;
		m_velocity.x = 1000.f;
	}
}

void Sun::phaseTwo(float deltaTime, Player* player)
{
	static Timer miniPhaseTimer(true);

	//boss movement
	m_position.x += m_velocity.x * deltaTime;
	if (m_position.x + m_texture.getWidth() - 150 > SCREEN_WIDTH || m_position.x - 150 < 0)
	{
		m_position.x -= m_velocity.x * deltaTime;
		m_velocity.x = -m_velocity.x;
	}

	if (miniPhaseTimer.getTicks() > 20000)
	{
		m_shootingRate = 600;
		m_shootingSpeed = 400;
		explodeAttack(m_position);

		m_shootingRate = 1000;
		m_shootingSpeed = 600;
		aimedShot(player->getPosition());
	}
	else if (miniPhaseTimer.getTicks() > 10000)
	{
		m_shootingRate = 300;
		m_shootingSpeed = 400;
		random360();
	}
	else
	{

	}

	if (miniPhaseTimer.getTicks() > 30000)
		miniPhaseTimer.start();

	if (m_health <= 0)
	{
		m_health = 100;
		m_healthColor = color(255, 0, 0, 128);
		m_phaseNumber = 3;
		m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f) + 150.f, 150.f };
		m_shootingRate = 300;
		m_shootingSpeed = 200;
	}
}

void Sun::phaseThree(float deltaTime)
{
	m_shootingRate -= 8 * deltaTime;
	m_shootingSpeed += 8 * deltaTime;
	random360();

	if (m_health <= 0)
	{
		m_phaseNumber = 0;
	}
}

void Sun::random360()
{
	static int _index = 0;
	static Timer repeatedTimer(true);

	if (repeatedTimer.getTicks() >= m_shootingRate / 5)
	{
		repeatedTimer.start();
		if (_index >= 300)
			_index = 0;
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2.f);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX / 2.f);
		Vector2f _direction = { r1 - 1.f, r2 - 1.f };
		_direction = _direction.normalized();
		m_Projectiles[_index].reload(m_position, _direction, 0, (float)m_shootingSpeed);
		m_Projectiles[_index].setActive(true);
		_index++;
	}
}

void Sun::explodeAttack(Vector2f position)
{
	static int _index = 300;
	static Timer repeatedTimer(true);

	if (repeatedTimer.getTicks() >= m_shootingRate)
	{
		repeatedTimer.start();
		int randomOffset = rand() % 360;
		for (int i = 0; i < 12; i++)
		{
			if (_index >= 500)
				_index = 300;
			int angle = (i * 30) + randomOffset;
			Vector2f _aimedPosition = { position.x + (GLfloat)cos(angle * PI / 180), position.y + (GLfloat)sin(angle * PI / 180) };
			Vector2f _direction = _aimedPosition - position;
			_direction = _direction.normalized();
			m_Projectiles[_index].reload(position, _direction, 0, (float)m_shootingSpeed);
			m_Projectiles[_index].setActive(true);
			_index++;
		}
	}
}

void Sun::aimedShot(Vector2f position)
{
	static Timer repeatedTimer(true);
	if (repeatedTimer.getTicks() >= m_shootingRate)
	{
		repeatedTimer.start();
		static int _index = 700;

		if (_index >= 800)
			_index = 700;

		Vector2f _direction = position - m_position;
		_direction = _direction.normalized();
		m_Projectiles[_index].reload(m_position, _direction, 0, (float)m_shootingSpeed);
		m_Projectiles[_index].setActive(true);
		m_Projectiles[_index].setParticleEmitter(new ParticleEmitter(20, Vector2f(0, 0), color(255, 180, 0, 255), 0.5f, 0.1f, 6.f));

		_index++;
	}
}


void Sun::implodeAttack()
{
	static int _index = 500;
	static Timer repeatedTimer(true);

	if (repeatedTimer.getTicks() >= m_shootingRate)
	{
		repeatedTimer.start();
		int randomOffset = rand() % 360;
		for (int i = 0; i < 12; i++)
		{
			if (_index >= 700)
				_index = 500;
			int angle = (i * 30) + randomOffset;
			Vector2f _startingPosition = { m_position.x + ((GLfloat)cos(angle * PI / 180) * 800), m_position.y + ((GLfloat)sin(angle * PI / 180) * 800) };
			Vector2f _direction = m_position - _startingPosition;
			_direction = _direction.normalized();
			m_Projectiles[_index].reload(_startingPosition, _direction, 0, (float)m_shootingSpeed);
			m_Projectiles[_index].setActive(true);
			_index++;
		}
	}
}