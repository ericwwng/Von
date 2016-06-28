#include "entity/player.h"

void Cursor::Render() const
{
	SDL_Point rotationPoint = { tex.getWidth() / 2, tex.getHeight() / 2 };
	tex.Render(position.x - tex.getWidth() / 2, position.y - tex.getHeight() / 2, NULL, angle, &rotationPoint);
}

void Cursor::Update(float deltaTime)
{
    int x, y;
    SDL_GetMouseState(&x, &y);

	position.x = (GLfloat)x + Camera::getInstance().collisionBox.position.x;
	position.y = (GLfloat)y + Camera::getInstance().collisionBox.position.y;
    angle += 100 * deltaTime;
    collisionBox = {Vector2f(position.x, position.y), 1, 1};
}

Player::Player(Vector2f pos)
{
    angle = 0.f;
    position = pos;

    playerSpeed = 4; //Will become 4 * 100 = 400 afted deltatime inclusion
	slipAmount = 15; //Default

    tex.loadFromFile("res/player.png");

	staminaBar.loadFromFile("res/GUI/staminabar.png");
	stamina = 256;

	weapon = new Gun();
}

Player::~Player()
{
	delete weapon;
}

void Player::Render() const
{
	SDL_Point rotationPoint = { tex.getWidth() / 2, tex.getHeight() / 2 };
	tex.Render(position.x, position.y, NULL, angle, &rotationPoint);
	Rectf box = { position.x + 20, position.y + 20, 32, 32 };
	if (showCollisionBox) renderEmptyBox(box, color(0, 255, 0, 255));
	weapon->Render();
}

void Player::RenderUI() const
{
	Rectf staminaBarClip = { 0.f, 0.f, (GLfloat)stamina, 32.f };
	staminaBar.Render(Camera::getInstance().collisionBox.position.x + 32, Camera::getInstance().collisionBox.position.y + 64, &staminaBarClip);
}

void Player::HandleEvents()
{
    velocityGoal = { 0, 0 };
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );

    if (currentKeyStates[SDL_SCANCODE_W])        velocityGoal.y = (float)-playerSpeed;
    if (currentKeyStates[SDL_SCANCODE_A])        velocityGoal.x = (float)-playerSpeed;
    if (currentKeyStates[SDL_SCANCODE_S])        velocityGoal.y = (float)playerSpeed;
    if (currentKeyStates[SDL_SCANCODE_D])        velocityGoal.x = (float)playerSpeed;
	if (currentKeyStates[SDL_SCANCODE_SPACE])    weapon->Action();

	if (event.type == SDL_MOUSEBUTTONDOWN)
		weapon->Action();
}

//Collision for player to tile handling
void Player::CheckCollisionTypes(Tile* tileTypes, int dimW)
{
	for (int i = 0; i < 5; i++)
		for (int ii = 0; ii < 5; ii++)
			if((i + (int)floor(collisionBox.position.y / 16) - 2) * dimW + (ii + (int)floor(collisionBox.position.x / 16) - 2) > 0)
				if (Collision(collisionBox, tileTypes[(i + (int)floor(collisionBox.position.y / 16) - 2) * dimW + (ii + (int)floor(collisionBox.position.x / 16) - 2)].collisionBox))
				{
					switch (tileTypes[(i + (int)floor(collisionBox.position.y / 16) - 2) * dimW + (ii + (int)floor(collisionBox.position.x / 16) - 2)].id)
					{
					case 1: //Solid
						isCollided = true;
						break;

					case 2: //Slow
						playerSpeed = 2;
						break;

					case 3: //Slippery
						slipAmount = 5;
						playerSpeed = 6;
						break;

					case 4: //Solid
						isCollided = true;
						break;

					case 5: //Solid
						isCollided = true;
						break;
					}
				}
}

void Player::Update(float deltaTime, Tile* tileTypes, int dimW, int dimH)
{
	static Vector2f tempVelocity;
	playerSpeed = 4;

	tempVelocity.x = LerpApproach(velocityGoal.x, tempVelocity.x, deltaTime * slipAmount);
	tempVelocity.y = LerpApproach(velocityGoal.y, tempVelocity.y, deltaTime * slipAmount);
	velocity = tempVelocity * deltaTime * 100;

	slipAmount = 15;

	//Update x values to allow wall sliding
	isCollided = false;
	position.x = position.x + velocity.x;
	collisionBox = { Vector2f(position.x + 20, position.y + 20), 32, 32 };
	CheckCollisionTypes(tileTypes, dimW);
	if (isCollided || collisionBox.position.x < 0 || collisionBox.position.x + collisionBox.width > dimW * 16)
		position.x = position.x - velocity.x;

	//Update y values to allow wall sliding
	isCollided = false;
	position.y = position.y + velocity.y;
	collisionBox = { Vector2f(position.x + 20, position.y + 20), 32, 32 };
	CheckCollisionTypes(tileTypes, dimW);
	if (isCollided || collisionBox.position.y < 0 || collisionBox.position.y + collisionBox.height > dimH * 16)
		position.y = position.y - velocity.y;

	collisionBox = { position, tex.getWidth(), tex.getHeight() };

	Camera::getInstance().collisionBox.position.x = position.x - SCREEN_WIDTH / 2;
	Camera::getInstance().collisionBox.position.y = position.y - SCREEN_HEIGHT / 2;
	
	if (Camera::getInstance().collisionBox.position.x < 0) Camera::getInstance().collisionBox.position.x = 0;
	if (Camera::getInstance().collisionBox.position.y < 0) Camera::getInstance().collisionBox.position.y = 0;
	if (Camera::getInstance().collisionBox.position.x + Camera::getInstance().collisionBox.width > dimW * 16) Camera::getInstance().collisionBox.position.x = dimW * 16 - Camera::getInstance().collisionBox.width;
	if (Camera::getInstance().collisionBox.position.y + Camera::getInstance().collisionBox.height > dimH * 16) Camera::getInstance().collisionBox.position.y = dimH * 16 - Camera::getInstance().collisionBox.height;
  
	weapon->Update(position, angle, deltaTime);
	direction = Cursor::getInstance().getPosition() - Vector2f(position.x + collisionBox.width / 2, position.y + collisionBox.height / 2);
	direction = direction.Normalized();
	angle = (float)(atan2(direction.y, direction.x) * (180.f / PI));
	direction = Cursor::getInstance().getPosition() - weapon->getPosition();
	direction = direction.Normalized();
	weapon->setDirection(direction);
}
