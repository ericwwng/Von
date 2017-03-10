#include "entity\bosses\moneyboss.h"

BigMoney::BigMoney() :
	m_shootingRate(1500),
	m_shootingSpeed(250)
{
	m_texture.loadFromFile("res/Enemy/Boss/BigMoney/BigMoneyImage350.png", 350, 350);
	m_Projectiles = new Projectile[MAX_PROJECTILE_AMOUNT];

	//set a third of the random360 textures to use coin sprite
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

	//set the coin sprite for the implode and explode projectiles
	for (int i = 300; i < 700; i++)
	{
		m_Projectiles[i].loadTexture("res/Projectile/coin.png");
		m_Projectiles[i].setCollisionBox(32, 32);
	}

	//set the rest to bill
	for (int i = 700; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		m_Projectiles[i].loadTexture("res/Projectile/bill.png");
		m_Projectiles[i].setCollisionBox(64, 32);
	}

	m_collisionTimer.start();
	m_bossCollisionTimer.start();
	m_blinkTimer.start();

	m_phaseNumber = 1; //1 default
	m_health = 100.f; //100 default

	m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f) + 175.f, 175.f };

	m_healthColor = color(0, 255, 0, 128);

	Warp::getInstance().setPosition(Vector2f(-64, -64));

}

BigMoney::~BigMoney()
{
	delete m_Projectiles;
}

void BigMoney::render()
{
	if (m_phaseNumber != 0)
	{
		if (m_bossCollisionTimer.getTicks() > 1500)
			m_texture.render(m_position.x - 175.f, m_position.y - 175.f);
		else
		{
			if (m_blinkTimer.getTicks() < 150)
				m_texture.render(m_position.x - 175.f, m_position.y - 175.f);
			else if (m_blinkTimer.getTicks() > 300)
				m_blinkTimer.start();
		}
	}

	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		if (m_Projectiles[i].isActive())
		{
			m_Projectiles[i].render();
		}
	}

	Rectf _box = { 0, 0, m_health * 12.8f, 64 };
	renderFillRect(_box, m_healthColor);
	renderEmptyBox(_box, color(0, 0, 0, 255));

	if (m_phaseNumber == 3 && m_health == 0)
		Warp::getInstance().render();
	
}

void BigMoney::update(float deltaTime, Player* player)
{
	m_collisionBox = { Vector2f(m_position.x - 175 + 100, m_position.y - 175), 250, 250 };

	if (m_phaseNumber == 1)
		phaseOne();
	else if (m_phaseNumber == 2)
		phaseTwo(deltaTime, player);
	else if (m_phaseNumber == 3)
		phaseThree(deltaTime);

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
				if (player->getPlayerHealth() > 0)
					player->setHit();
			}
	
	//Special case for implosion attack
	for (int i = 500; i < 700; i++)
	{
		if (m_Projectiles[i].isActive() && m_phaseNumber != 2)
			if (Collision(m_Projectiles[i].getCollisionBox(), m_collisionBox))
				m_Projectiles[i].setActive(false);
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
						m_health -= 4;
						break;
					}
					case 2:
					{
						m_health -= 3;
						break;
					}
					case 3:
					{
						m_health -= 5;
						break;
					}
				}
			}
			player->getWeapon()->getProjectile()->reload(Vector2f(0, 0), Vector2f(0, 0), 0, 0);
		}

	if (m_phaseNumber == 3 && m_health == 0)
	{
		Warp::getInstance().setPosition(m_position);
		Warp::getInstance().update(deltaTime);
	}
}

void BigMoney::phaseOne()
{
	random360();
	implodeAttack();
	if (m_health <= 0)
	{
		m_health = 100;
		m_healthColor = color(0, 0, 255, 128);
		m_phaseNumber = 2;
		m_velocity.x = 1000.f;
	}
}

void BigMoney::phaseTwo(float deltaTime, Player* player)
{
	static Timer _miniPhaseTimer(true);
	static int _attackNumber = 0;

	//boss movement
	m_position.x += m_velocity.x * deltaTime;
	if (m_position.x + m_texture.getWidth() - 225 > SCREEN_WIDTH || m_position.x - 225 < 0)
	{
		m_position.x -= m_velocity.x * deltaTime;
		m_velocity.x = -m_velocity.x;
	}

	if (_attackNumber == 0)
	{
		m_shootingRate = 300;
		m_shootingSpeed = 400;
		random360();
	}
	else if (_attackNumber == 1)
	{
		m_shootingRate = 600;
		m_shootingSpeed = 400;
		explodeAttack(m_position);

		m_shootingRate = 1000;
		m_shootingSpeed = 600;
		aimedShot(player->getPosition());
	}
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

				if (_index >= 660)
					_index = 300;

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

	if (_miniPhaseTimer.getTicks() > 10000)
	{
		_miniPhaseTimer.start();
		_attackNumber++;
		if (_attackNumber == 3)
			_attackNumber = 0;

		if (_attackNumber == 0)
		{
			m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f) + 175.f, 175.f };
			m_velocity.x = 1000.f;
		}
		else if (_attackNumber == 2)
		{
			m_velocity.x = 0.f;
			m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f) + 175.f, SCREEN_HEIGHT / 2 - (m_texture.getHeight() / 4.f) };

			m_shootingRate = 100;
			m_shootingSpeed = 600;
		}
	}
	
	if (m_health <= 0)
	{
		m_health = 100;
		m_healthColor = color(255, 0, 0, 128);
		m_phaseNumber = 3;
		m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f) + 175.f, SCREEN_HEIGHT / 2 - (m_texture.getHeight() / 4.f) };
		m_shootingRate = 300;
		m_shootingSpeed = 200;
	}
}

void BigMoney::phaseThree(float deltaTime)
{
	m_shootingRate -= 8 * deltaTime;
	m_shootingSpeed += 8 * deltaTime;
	random360();

	if (m_health <= 0)
	{
		m_phaseNumber = 0;
	}
}

void BigMoney::random360()
{
	static int _index = 0;
	static Timer _repeatedTimer(true);

	if (_repeatedTimer.getTicks() >= m_shootingRate / 5)
	{
		_repeatedTimer.start();
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

void BigMoney::aimedShot(Vector2f position)
{
	static Timer _repeatedTimer(true);
	if (_repeatedTimer.getTicks() >= m_shootingRate)
	{
		_repeatedTimer.start();
		static int _index = 700;

		if (_index >= 800)
			_index = 700;

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