#include "entity/weapons/gun.h"

Gun::Gun()
{
	angle = 0.f;
	tex.loadFromFile("res/Weapon/gun.png");

	bullet = new Bullet[3];

	rateOfFire.start();
}

Gun::~Gun()
{
	delete bullet;
}

void Gun::Action()
{
	for (int i = 0; i < 3; i++)
		if (!bullet[i].isActive() && rateOfFire.getTicks() >= 200)
		{
			bullet[i].Reload(position, weaponDirection, angle, 1000);
			bullet[i].setActive(true);
			rateOfFire.start();
		}
}

void Gun::Render() const
{
	Vector2f rotationPoint = { 0.f, 0.f };
	tex.Render(position.x, position.y, NULL, angle, &rotationPoint);
	for (int i = 0; i < 3; i++)
		if (bullet[i].isActive())
			bullet[i].Render();
}

void Gun::Update(Vector2f& pos, float angle, float deltaTime)
{
	position = pos;
	this->angle = angle;

	position = UpdateRotation();

	collisionBox = { position, 32, 16 };

	for (int i = 0; i < 3; i++)
	{
		if (bullet[i].isActive())
			bullet[i].Update(deltaTime);
	}
}
