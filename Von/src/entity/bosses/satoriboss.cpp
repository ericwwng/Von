#include "entity\bosses\satoriboss.h"

Satori::Satori():
	m_shootingRate(200),
	m_shootingSpeed(100)
{
	m_texture.loadFromFile("res/Enemy/Boss/Satori/SatoriImage.png", 444, 456);
	m_Projectiles = new Projectile[MAX_PROJECTILE_AMOUNT];

	m_songTimer.start();
	m_healthDecreaseTimer.start();
	m_speedUpTimer.start();
	m_collisionTimer.start();

	m_phaseNumber = 1;
	//152,000 milliseconds in the song
	m_health = 1280.f;

	m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f), 0.f };

	m_bulletSpawnPosition = { m_position.x + (m_texture.getWidth() / 2.f), m_texture.getHeight() / 4.f};
	m_spawnCircle.setPosition(m_bulletSpawnPosition);
	m_spawnCircle.setCircleType(2);
	m_spawnCircle.setColor(color(255, 105, 180, 255));
	//m_spawnCircle.setActive(true);

	//random360
	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		m_Projectiles[i].setCircleType(4);
	}
}

Satori::~Satori()
{
	delete m_Projectiles;
}

void Satori::render()
{
	m_texture.render(m_position.x, m_position.y);

	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		if (m_Projectiles[i].isActive())
		{
			m_Projectiles[i].render();
		}
	}

	m_spawnCircle.render();

	Rectf _box = { 0, 0, (GLfloat)m_health, 64 };
	renderFillRect(_box, color(255, 0, 0, 128));
	renderEmptyBox(_box, color(0, 0, 0, 255));
}

void Satori::update(float deltaTime, Player* player)
{
	if (m_phaseNumber == 1)
		phaseOne();
	else if (m_phaseNumber == 2)
		phaseTwo();

	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		if (m_Projectiles[i].isActive())
		{
			m_Projectiles[i].update(deltaTime);
		}
	}

	//Check for collision with the projectiles for bosses
	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		if (m_Projectiles[i].isActive())
		{
			if (Collision(m_Projectiles[i].getCollisionBox(), player->getCollisionBox()) &&
				m_collisionTimer.getTicks() > 2000)
			{
				m_collisionTimer.start();
				if (player->getPlayerHealth() > 0)
				{
					player->setHit();
					if (player->getPlayerHealth() <= 0) g_isPlayerDead = true;
				}
			}


			if(player->getWeapon()->getProjectile()->isActive())
				if (Collision(m_Projectiles[i].getCollisionBox(), player->getWeapon()->getProjectile()->getCollisionBox()))
				{
					m_Projectiles[i].reload(Vector2f(0, 0), Vector2f(0, 0), 0, 0);
					player->getWeapon()->getProjectile()->reload(Vector2f(0, 0), Vector2f(0, 0), 0, 0);
				}
		}
	}
}

void Satori::phaseOne()
{
	if(m_songTimer.getTicks() > 3000 && m_songTimer.getTicks() < 25800)
		random360();
	else if(m_songTimer.getTicks() > 25800)
	{
		m_phaseNumber = 2;

		//transition
		for (int i = 0; i <= 300; i++)
			m_Projectiles[i].multiplyVelocity(8.f);
	}

	if(m_healthDecreaseTimer.getTicks() >= 500)
	{
		m_health -= (1280.f / 155.f / 2.f);
		m_healthDecreaseTimer.start();
	}
}

void Satori::phaseTwo()
{
	static Timer repeatedTimer(true);

	if (m_songTimer.getTicks() > 26000)
	{
		if (repeatedTimer.getTicks() >= 1000)
		{
			m_shootingSpeed = 500;
			//aimedShot();
			repeatedTimer.start();
		}
	}

	if (m_healthDecreaseTimer.getTicks() >= 500)
	{
		m_health -= (1280.f / 155.f / 2.f);
		m_healthDecreaseTimer.start();
	}
}

void Satori::randomPhase()
{
	Vector2f _randomPosition = { (GLfloat)(rand() % 1280), (GLfloat)(rand() % 720) };
	if (m_songTimer.getTicks() > 3000 && m_songTimer.getTicks() < 155500)
	{
		explodeAttack(m_bulletSpawnPosition);
		random360();
	}

	if(m_healthDecreaseTimer.getTicks() >= 500)
	{
		m_health -= (1280.f / 155.f / 2.f);
		m_healthDecreaseTimer.start();
	}

	if(m_speedUpTimer.getTicks() >= 3000)
	{
		if(m_shootingRate >= 16)
			m_shootingRate -= 2;
		if(m_shootingSpeed < 300)
			m_shootingSpeed += 5;
		printf("rate: %d\n speed: %d\n\n", m_shootingRate, m_shootingSpeed);
		m_speedUpTimer.start();
	}
}

void Satori::random360()
{
	static int _index = 0;
	static Timer repeatedTimer(true);

	if(repeatedTimer.getTicks() >= m_shootingRate / 5)
	{
		repeatedTimer.start();
		if(_index >= 300)
			_index = 0;
		float r1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2.f);
		float r2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX/2.f);
		Vector2f _direction = { r1 - 1.f, r2 - 1.f };
		_direction = _direction.normalized();
		m_Projectiles[_index].reload(m_bulletSpawnPosition, _direction, 0, (float)m_shootingSpeed);
		m_Projectiles[_index].setActive(true);
		m_Projectiles[_index].setColor(color(rand() % 255, rand() % 255, rand() % 255, 255));
		_index++;
	}
}

void Satori::explodeAttack(Vector2f position)
{
	static int _index = 300;
	static Timer repeatedTimer(true);

	if(repeatedTimer.getTicks() >= m_shootingRate)
	{
		repeatedTimer.start();
		SDL_Color col = color(rand() % 255, rand() % 255, rand() % 255, 255);
		int randomOffset = rand() % 360;
		for(int i = 0; i < 12; i++)
		{
			if(_index >= 500)
				_index = 300;
			int angle = (i * 30) + randomOffset;
			Vector2f _aimedPosition = { position.x + (GLfloat)cos(angle * PI / 180), position.y + (GLfloat)sin(angle * PI / 180) };
			Vector2f _direction = _aimedPosition - position;
			_direction = _direction.normalized();
			m_Projectiles[_index].reload(position, _direction, 0, (float)m_shootingSpeed);
			m_Projectiles[_index].setActive(true);
			m_Projectiles[_index].setColor(col);
			_index++;
		}
	}
}

void Satori::aimedShot(Vector2f position)
{
	static int _index = 500;

	if (_index >= 600)
		_index = 500;

	Vector2f _direction = position - m_bulletSpawnPosition;
	_direction = _direction.normalized();
	m_Projectiles[_index].reload(m_bulletSpawnPosition, _direction, 0, (float)m_shootingSpeed);
	m_Projectiles[_index].setActive(true);
	m_Projectiles[_index].setColor(color(rand() % 255, rand() % 255, rand() % 255, 255));

	_index++;
}
