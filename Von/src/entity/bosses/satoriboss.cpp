#include "entity\bosses\satoriboss.h"

Satori::Satori()
{
	m_texture.loadFromFile("res/Enemy/Boss/Satori/SatoriImage.png", 444, 456);
	m_Projectiles = new Projectile[MAX_PROJECTILE_AMOUNT];

	m_songTimer.start();
	m_healthDecreaseTimer.start();
	m_collisionTimer.start();

	m_health = 1280.f; //Width of the screen

	m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f), 0.f };

	m_bulletSpawnPosition = { m_position.x + (m_texture.getWidth() / 2.f), m_texture.getHeight() / 4.f};
	m_spawnCircle.setPosition(m_bulletSpawnPosition);
	m_spawnCircle.setColor(color(255, 105, 180, 255));

	//Set particle properties
	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		m_Projectiles[i].setCenteredBox(true);
	}

	//For first attack
	m_shootingRate = 180;
	m_shootingSpeed = 120;
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
	if (m_health > 0) attack(player);

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
					player->hit();
					if (player->getPlayerHealth() <= 0) g_isPlayerDead = true;
				}
			}
		}
	}
}

void Satori::attack(Player* player)
{
	static bool _endAttack1 = false;
	if (!_endAttack1) _endAttack1 = attack1();

	static bool _endAttack2 = false;
	if (_endAttack1 && !_endAttack2) _endAttack2 = attack2(player);

	if(m_healthDecreaseTimer.getTicks() >= 500)
	{
		m_health -= (1280.f / 155.f / 2.f);
		m_healthDecreaseTimer.start();
	}
}

bool Satori::attack1()
{
	//Attack 1: randomly shoot
	bool _endAttackOne = false;
	if (m_songTimer.getTicks() > 3000 && m_songTimer.getTicks() < 25800)
	{
		random360(m_bulletSpawnPosition);
	}
	else if (m_songTimer.getTicks() > 25800)
	{
		//Transition to next attack
		for (int i = 0; i <= 300; i++) m_Projectiles[i].setVelocity(
			Vector2f(m_Projectiles[i].getVelocity().x * 8, m_Projectiles[i].getVelocity().y * 8));

		return true;
	}

	return false;
}

bool Satori::attack2(Player* player)
{
	static Timer repeatedTimer(true);

	if (m_songTimer.getTicks() > 26000)
	{
		if (repeatedTimer.getTicks() >= 1000)
		{
			m_shootingSpeed = 500;
			aimedShot(player->getPosition());
			repeatedTimer.start();
		}
	}

	return false;
}

void Satori::random360(Vector2f position)
{
	static int _index = 0;
	static Timer _repeatedTimer(true);

	if (_repeatedTimer.getTicks() >= m_shootingRate / 5)
	{
		_repeatedTimer.start();
		if (_index >= 300) _index = 0;
		Vector2f _direction = { randFloat(-1.f, 1.f), randFloat(-1.f, 1.f) };
		_direction = _direction.normalized();
		m_Projectiles[_index].reload(position, _direction, 0, (float)m_shootingSpeed);
		m_Projectiles[_index].setActive(true);
		_index++;
	}
}

void Satori::explodeAttack(Vector2f position)
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

void Satori::aimedShot(Vector2f position)
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