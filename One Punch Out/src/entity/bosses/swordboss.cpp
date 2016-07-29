#include "entity\bosses\swordboss.h"

SwordBoss::SwordBoss()
{
	m_tex.loadFromFile("res/Enemy/Boss/Youmu/YoumuImage.png", 300, 342);

}

SwordBoss::~SwordBoss()
{

}

void SwordBoss::Render()
{
	m_tex.Render(0, 0);
}

void SwordBoss::Update()
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
