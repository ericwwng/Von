#include "entity\bosses\swordboss.h"

SwordBoss::SwordBoss()
{
	m_texture.loadFromFile("res/Enemy/Boss/Youmu/YoumuImage.png", 300, 342);

	m_Projectiles = new CircleProjectile[200];
}

SwordBoss::~SwordBoss()
{

}

void SwordBoss::render()
{
	m_texture.render(0, 0);
}

void SwordBoss::update()
{
	if (m_phaseNumber == 1)
		phaseOne();
	else if (m_phaseNumber == 2)
		phaseTwo();
	else if (m_phaseNumber == 3)
		phaseThree();

}

void SwordBoss::phaseOne()
{
	
}

void SwordBoss::phaseTwo()
{

}

void SwordBoss::phaseThree()
{

}
