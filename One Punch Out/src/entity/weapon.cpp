#include "entity/weapon.h"

Vector2f Weapon::UpdateRotation()
{
	//To store the new x amd y values
	Vector2f tempPosition;

	//origin, center of player texture
	float cx = position.x + 36;
	float cy = position.y + 36;

	//Position of left hand
	position.x += 64;
	position.y += 0;

	position.x -= cx;
	position.y -= cy;

	//Calcluate coordinates after rotation
	tempPosition.x = (float)(position.x * cos(angle * PI / 180) - position.y * sin(angle * PI / 180));
	tempPosition.y = (float)(position.x * sin(angle * PI / 180) + position.y * cos(angle * PI / 180));

	position.x = tempPosition.x + cx;
	position.y = tempPosition.y + cy;
	//printf("position(%f, %f)\n", position.x, position.y);
	
	return position;
}