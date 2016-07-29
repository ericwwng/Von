#include "entity/projectiles/bullet.h"

Bullet::Bullet()
{
    tex.loadFromFile("res/Projectile/bullet.png");
    active = false;
}

Bullet::~Bullet()
{

}

void Bullet::Render() const
{
	Vector2f rotationPoint = { tex.getWidth() / 2.f, tex.getHeight() / 2.f };
	tex.Render(position.x, position.y, NULL, angle, &rotationPoint);
	Rectf box = { position.x, position.y, (GLfloat)tex.getWidth(), (GLfloat)tex.getHeight() };
	if (showCollisionBox) renderEmptyBox(box, color(0, 255, 0, 255));
}

void Bullet::Update(float deltaTime)
{
	position = position + (velocity * deltaTime);
	if (position.x - Camera::getInstance().collisionBox.position.x < 0 || position.x - Camera::getInstance().collisionBox.position.x > SCREEN_WIDTH)
		setActive(false);
	if (position.y - Camera::getInstance().collisionBox.position.y < 0 || position.y - Camera::getInstance().collisionBox.position.y > SCREEN_HEIGHT)
		setActive(false);

	collisionBox = { position, 8, 8 };
}