#include "entity\bosses\moneyboss.h"

BigMoney::BigMoney()
{
	m_texture.loadFromFile("res/Enemy/Boss/BigMoney/BigMoneyImage350.png", 350, 350);
	m_Projectiles = new Projectile[MAX_PROJECTILE_AMOUNT];

	//Set a third of the random360 textures to use coin sprite
	for (int i = 0; i < 300; i++)
	{
		if (i % 3 != 0)
		{
			m_Projectiles[i].loadTexture("res/Projectile/bill.png");
			m_Projectiles[i].setCollisionBox(64, 32);
		}
		else
		{
			m_Projectiles[i].loadTexture("res/Projectile/coin.png");
			m_Projectiles[i].setCollisionBox(32, 32);
		}
	}

	//Set the coin sprite for the implode and explode projectiles
	for (int i = 300; i < 700; i++)
	{
		m_Projectiles[i].loadTexture("res/Projectile/coin.png");
		m_Projectiles[i].setCollisionBox(32, 32);
	}

	//Set the rest to bill
	for (int i = 700; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		m_Projectiles[i].loadTexture("res/Projectile/bill.png");
		m_Projectiles[i].setCollisionBox(64, 32);
	}

	m_collisionTimer.start();
	m_bossCollisionTimer.start();
	m_blinkTimer.start();

	///Set up for the first phase
	m_phaseNumber = 1; //1 default
	m_health = 100; //100 default
	m_shootingRate = 1500;
	m_shootingSpeed = 250;
	m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f) + m_texture.getWidth() / 2.f, m_texture.getHeight() / 2.f };
	m_healthColor = color(0, 255, 0, 128);
	m_healthBarParticleEmitter = new ParticleEmitter(100, Vector2f(0, 32), color(255, 255, 255, 255), 3.f, 1.f, 4.f);
	m_bossStartTimer.start();
}

BigMoney::~BigMoney()
{
	delete m_Projectiles;
	delete m_healthBarParticleEmitter;
}

void BigMoney::render()
{
	//If phase is 0 (dead) then perform actions pertaining to the rendering of the boss
	if (m_phaseNumber != 0)
	{
		if (m_bossCollisionTimer.getTicks() > 1500) m_texture.render(m_position.x - m_texture.getWidth() / 2, m_position.y - m_texture.getHeight() / 2);
		else
		{
			if (m_blinkTimer.getTicks() < 150) m_texture.render(m_position.x - m_texture.getWidth() / 2, m_position.y - m_texture.getHeight() / 2);
			else if (m_blinkTimer.getTicks() > 300) m_blinkTimer.start();
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

	//Render the warp if boss is dead
	if (m_phaseNumber == 0) Warp::getInstance().render();
}

void BigMoney::update(float deltaTime, Player* player)
{
	m_collisionBox = { Vector2f(m_position.x - 175 + 100, m_position.y - 175), 250, 250 };

	switch (m_phaseNumber)
	{
		case 1:
		{
			phaseOne();
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
		if (m_Projectiles[i].isActive()) m_Projectiles[i].update(deltaTime);
	}

	//Check for collision with the projectiles for bosses and the player
	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
		if (m_Projectiles[i].isActive())
			if (Collision(m_Projectiles[i].getCollisionBox(), player->getCollisionBox()) &&
				m_collisionTimer.getTicks() > 2000)
			{
				m_collisionTimer.start();
				if (player->getPlayerHealth() > 0) player->hit();
			}
	
	//Special collision case for implosion attack
	for (int i = 500; i < 700; i++)
	{
		if (m_Projectiles[i].isActive() && m_phaseNumber != 2)
		{
			if (Collision(m_Projectiles[i].getCollisionBox(), m_collisionBox))
			{
				GLfloat scale = m_Projectiles[i].getXScale() - 3 * deltaTime;
				m_Projectiles[i].setScale(scale, scale);
				if (scale <= 0.05f)
				{
					m_Projectiles[i].setScale(1.f, 1.f);
					m_Projectiles[i].setActive(false);
				}
			}
		}
	}
	
	//Player hits boss
	for (int i = 0; i < player->getWeapon()->getBulletAmount(); i++)
	{
		if (player->getWeapon()->getProjectiles()[i].isActive())
		{
			if (Collision(m_collisionBox, player->getWeapon()->getProjectiles()[i].getCollisionBox()))
			{
				if (m_bossCollisionTimer.getTicks() > 1500)
				{
					m_bossCollisionTimer.start();
					switch (m_phaseNumber)
					{
					case 1:
					{
						m_health -= 3.75;
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
				player->getWeapon()->getProjectiles()[i].reload(Vector2f(0, 0), Vector2f(0, 0), 0, 0);
			}
		}
	}

	m_healthBarParticleEmitter->setLifeDuration(3.f / (100 / m_health));
	m_healthBarParticleEmitter->setLifeVariance(1.f / (100 / m_health));
	m_healthBarParticleEmitter->update(deltaTime, Vector2f(randFloat(2.f, 4.f), randFloat(0.05f / (m_health / 100), -0.05f / (m_health / 100))));

	if (m_phaseNumber == 3 && m_health == 0)
	{
		Warp::getInstance().setPosition(m_position);
		Warp::getInstance().update(deltaTime);
	}
}

void BigMoney::phaseOne()
{
	if (m_bossStartTimer.getTicks() >= 2000)
	{
		random360();
		implodeAttack();
		if (m_health <= 0)
		{
			m_health = 100;
			m_healthColor = color(0, 0, 255, 128);
			m_phaseNumber = 2;
			m_velocityGoal.x = 1000.f;
		}
	}
}

void BigMoney::phaseTwo(float deltaTime, Player* player)
{
	static Timer _miniPhaseTimer(true);
	static Vector2f _tempVelocity;
	static int _attackNumber = 0;

	//Boss movement
	_tempVelocity.x = lerpApproach(m_velocityGoal.x, _tempVelocity.x, deltaTime * 1000);
	m_velocity = _tempVelocity * deltaTime * 100;
	m_velocity.normalized();

	m_position.x += m_velocity.x * deltaTime;
	if (m_position.x > SCREEN_WIDTH || m_position.x < 0)
	{
		m_position.x -= m_velocity.x * deltaTime;
		_tempVelocity.x = 0;
		m_velocityGoal.x = -m_velocityGoal.x;
	}

	if (_miniPhaseTimer.getTicks() < 10000)
	{
		//Attack 1: move and shoot randomly
		if (_attackNumber == 0)
		{
			m_shootingRate = 300;
			m_shootingSpeed = 400;
			random360();
		}
		//Attack 2: move while shoot circles of projectils and an aimed shot every so often
		else if (_attackNumber == 1)
		{
			m_shootingRate = 600;
			m_shootingSpeed = 400;
			explodeAttack(m_position);

			m_shootingRate = 1000;
			m_shootingSpeed = 600;
			aimedShot(player->getPosition());
		}
		//Attack 3: windmill of projectiles
		else
		{
			//Taken from aimedshot
			static Timer __repeatedTimer(true);
			if (__repeatedTimer.getTicks() >= m_shootingRate)
			{
				__repeatedTimer.start();
				for (int i = 0; i < 12; i++)
				{
					static int _index = 300;

					if (_index >= 660) _index = 300;

					int angle = (i * 30) + (_index - 300) / 4;
					Vector2f _aimedPosition = { m_position.x + (GLfloat)cos(angle * PI / 180), m_position.y + (GLfloat)sin(angle * PI / 180) };
					Vector2f _direction = _aimedPosition - m_position;
					_direction = _direction.normalized();
					m_Projectiles[_index].reload(m_position, _direction, 0, (float)m_shootingSpeed);
					m_Projectiles[_index].setActive(true);

					_index++;
				}
			}
		}
	}

	if (_miniPhaseTimer.getTicks() > 14000)
	{
		_miniPhaseTimer.start();
		_attackNumber++;

		if (_attackNumber == 3) _attackNumber = 0;

		if (_attackNumber == 0)
		{
			m_velocityGoal.x = 1000.f;
			m_velocity.x = 0.f;
			_tempVelocity.x = 0.f;
			m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f) + m_texture.getWidth() / 2.f, m_texture.getHeight() / 2.f };
		}
		else if (_attackNumber == 2)
		{
			m_velocity.x = 0.f;
			m_velocityGoal.x = 0.f;
			_tempVelocity.x = 0.f;
			m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f) + m_texture.getWidth() / 2.f, SCREEN_HEIGHT / 2.f - (m_texture.getHeight() / 4.f) };

			m_shootingRate = 100;
			m_shootingSpeed = 600;
		}
	}
	
	if (m_health <= 0)
	{
		m_health = 100;
		m_healthColor = color(255, 0, 0, 128);
		m_phaseNumber = 3;
		m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f) + m_texture.getWidth() / 2, SCREEN_HEIGHT / 2 - (m_texture.getHeight() / 4.f) };
		m_shootingRate = 300;
		m_shootingSpeed = 200;
	}
}

void BigMoney::phaseThree(float deltaTime)
{
	//Shoot increasingly faster projecties randomly
	m_shootingRate -= 9 * deltaTime;
	m_shootingSpeed += 10 * deltaTime;
	random360();

	if (m_health <= 0) m_phaseNumber = 0;
}

void BigMoney::random360()
{
	static int _index = 0;
	static Timer _repeatedTimer(true);

	if (_repeatedTimer.getTicks() >= m_shootingRate / 5)
	{
		_repeatedTimer.start();
		if (_index >= 300) _index = 0;
		Vector2f _direction = { randFloat(-1.f, 1.f), randFloat(-1.f, 1.f) };
		_direction = _direction.normalized();
		m_Projectiles[_index].reload(m_position, _direction, 0, (float)m_shootingSpeed);
		m_Projectiles[_index].setActive(true);
		_index++;
	}
}

void BigMoney::explodeAttack(Vector2f position)
{
	static int _index = 300;
	static Timer _repeatedTimer(true);

	if (_repeatedTimer.getTicks() >= m_shootingRate)
	{
		_repeatedTimer.start();
		int randomOffset = rand() % 360;
		for (int i = 0; i < 12; i++)
		{
			if (_index >= 500) _index = 300;
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

void BigMoney::aimedShot(Vector2f position)
{
	static Timer _repeatedTimer(true);
	if (_repeatedTimer.getTicks() >= m_shootingRate)
	{
		_repeatedTimer.start();
		static int _index = 700;
		if (_index >= 800) _index = 700;
		Vector2f _direction = position - m_position;
		_direction = _direction.normalized();
		m_Projectiles[_index].reload(m_position, _direction, 0, (float)m_shootingSpeed);
		m_Projectiles[_index].setActive(true);

		_index++;
	}
}

void BigMoney::implodeAttack()
{
	static int _index = 500;
	static Timer _repeatedTimer(true);

	if (_repeatedTimer.getTicks() >= m_shootingRate)
	{
		_repeatedTimer.start();
		int randomOffset = rand() % 360;
		for (int i = 0; i < 12; i++)
		{
			if (_index >= 700) _index = 500;
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