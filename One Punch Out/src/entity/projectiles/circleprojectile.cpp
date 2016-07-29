#include "entity\projectiles\circleprojectile.h"

CircleProjectile::CircleProjectile()
{
	tex.loadFromFile("res/Projectile/circleNormal.png");
	active = false;
}

CircleProjectile::CircleProjectile(GLubyte tileID)
{
	/*
		0 = 
		1 = 
		2 = 
		3 = 
	*/
	switch (tileID)
	{
		case 0:

		break;
		case 1:
		break;
	}
	tex.loadFromFile("res/Projectile/bullet.png");
	active = false;
}

CircleProjectile::~CircleProjectile()
{
}

void CircleProjectile::Update(float deltaTime)
{
}

void CircleProjectile::Render() const
{
}
