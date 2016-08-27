#include "entity\bosses\satoriboss.h"

Satori::Satori():
	m_shootingRate(100),
	m_shootingSpeed(100)
{
	m_texture.loadFromFile("res/Enemy/Boss/Satori/SatoriImage.png", 444, 456);
	m_Projectiles = new CircleProjectile[MAX_PROJECTILE_AMOUNT];

	m_songTimer.start();
	m_healthDecreaseTimer.start();
	m_speedUpTimer.start();
	m_repeatedTimer.start();

	m_phaseNumber = 1;
	//152,000 milliseconds in the song
	m_health = 1280.f;

	m_position = { SCREEN_WIDTH / 2 - (m_texture.getWidth() / 2.f), 0.f };

	m_bulletSpawnPosition = { m_position.x + (m_texture.getWidth() / 2.f), m_texture.getHeight() / 4.f};
	m_spawnCircle.setPosition(m_bulletSpawnPosition);
	m_spawnCircle.setCircleType(2);
	m_spawnCircle.setColor(color(255, 105, 180, 255));
	m_spawnCircle.setActive(true);

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
}

void Satori::update(float deltaTime)
{
	if (m_phaseNumber == 1)
		phaseOne();
	else if (m_phaseNumber == 2)
		phaseTwo();
	else if (m_phaseNumber == 3)
		phaseThree();

	for (int i = 0; i < MAX_PROJECTILE_AMOUNT; i++)
	{
		if (m_Projectiles[i].isActive())
		{
			m_Projectiles[i].update(deltaTime);
		}
	}
}

void Satori::phaseOne()
{
	if(m_songTimer.getTicks() > 3000 && m_songTimer.getTicks() < 155500)
		random360();

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

void Satori::phaseTwo()
{

}

void Satori::phaseThree()
{

}


void Satori::random360()
{
	static int _index = 10;

	if(m_repeatedTimer.getTicks() >= m_shootingRate)
	{
		m_repeatedTimer.start();
		if(_index >= MAX_PROJECTILE_AMOUNT)
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
