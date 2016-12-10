#include "entity\bosses\moneyboss.h"

BigMoney::BigMoney() :
	m_shootingRate(200),
	m_shootingSpeed(100)
{
	m_texture.loadFromFile("res/Enemy/Boss/BigMoney/BigMoneyImage450.png", 450, 450);
	m_Projectiles = new Projectile[MAX_PROJECTILE_AMOUNT];

	m_collisionTimer.start();

	m_phaseNumber = 1;
	m_health = 100.f;

	m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f), 0.f };
	m_collisionBox = {m_position, 450, 450};

	//random360
	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		m_Projectiles[i].setCircleType(4);
	}
}

BigMoney::~BigMoney()
{
	delete m_Projectiles;
}

void BigMoney::render()
{
	m_texture.render(m_position.x, m_position.y);

	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		if (m_Projectiles[i].isActive())
		{
			m_Projectiles[i].render();
		}
	}

	Rectf _box = { 0, 0, (GLfloat)m_health * 12.8f, 64 };
	renderFillRect(_box, color(255, 0, 0, 128));
	renderEmptyBox(_box, color(0, 0, 0, 255));
}

void BigMoney::update(float deltaTime)
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

	if (Player::getInstance().getWeapon()->getProjectile()->isActive())
		if (Collision(m_collisionBox, Player::getInstance().getWeapon()->getProjectile()->getCollisionBox()))
		{
			if (m_collisionTimer.getTicks() > 2000)
			{
				m_collisionTimer.start();
				m_health--;
			}
			Player::getInstance().getWeapon()->getProjectile()->reload(Vector2f(0, 0), Vector2f(0, 0), 0, 0);
		}
}

void BigMoney::phaseOne()
{
	//random360();

}

void BigMoney::phaseTwo()
{
	
}

void BigMoney::random360()
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
		m_Projectiles[_index].setColor(color(rand() % 255, rand() % 255, rand() % 255, 255));
		_index++;
	}
}

void BigMoney::explodeAttack(Vector2f position)
{
	static int _index = 300;
	static Timer repeatedTimer(true);

	if (repeatedTimer.getTicks() >= m_shootingRate)
	{
		repeatedTimer.start();
		SDL_Color col = color(rand() % 255, rand() % 255, rand() % 255, 255);
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
			m_Projectiles[_index].setColor(col);
			_index++;
		}
	}
}

void BigMoney::aimedShot()
{
	static int _index = 500;

	if (_index >= 600)
		_index = 500;

	Vector2f _aimedPosition = Player::getInstance().getPosition();
	Vector2f _direction = _aimedPosition - m_position;
	_direction = _direction.normalized();
	m_Projectiles[_index].reload(m_position, _direction, 0, (float)m_shootingSpeed);
	m_Projectiles[_index].setActive(true);
	m_Projectiles[_index].setColor(color(rand() % 255, rand() % 255, rand() % 255, 255));

	_index++;
}
