#include "entity/player.h"

Player::Player()
{
	m_angle = 0.f;
	m_position = {0, 0};

	m_playerSpeed = 4; //4 default
	m_slipAmount = 40;

	m_isClickPressed = false;

	m_texture.loadFromFile("res/Entity/player.png", 64, 64);

	m_health = m_maxHealth; //3 default

	m_hitSfx.loadSoundFile("res/Music/sfx/hit.wav");

	m_weapon = new Gun();

	m_collisionCircle.setColor(color(255, 32, 32, 64));
	m_collisionCircle.setScale(0.5, 0.5);
	m_collisionCircle.setActive(true);
	m_collisionCircle.setCenteredBox(true);
}

void Player::setHit()
{
	m_health--;
	m_hitSfx.playSound();
}

void Player::render()
{
	Vector2f _rotationPoint = { m_texture.getWidth() / 2.f, m_texture.getHeight() / 2.f };
	m_texture.render(m_position.x, m_position.y, NULL, NULL, NULL, m_angle, &_rotationPoint);

	m_weapon->render();
	m_collisionCircle.render();
}

void Player::renderUI(Camera* camera)
{
	if(m_health >= 1)
	{
		Rectf leftClip = { camera->m_collisionBox.position.x + 32.f, camera->m_collisionBox.position.y + 64.f, 85.f, 32.f};
		renderFillRect(leftClip, color(255, 0, 0, 128));
	}
	if (m_health >= 2)
	{
		Rectf middleClip = { camera->m_collisionBox.position.x + 117.f, camera->m_collisionBox.position.y + 64.f, 85.f, 32.f };
		renderFillRect(middleClip, color(0, 255, 0, 128));
	}
	if (m_health == 3)
	{
		Rectf rightClip = { camera->m_collisionBox.position.x + 202.f, camera->m_collisionBox.position.y + 64.f, 85.f, 32.f };
		renderFillRect(rightClip, color(0, 0, 255, 128));
	}

	Rectf _box = { camera->m_collisionBox.position.x + 32, camera->m_collisionBox.position.y + 64, 256.f, 32.f };
	renderEmptyBox(_box, color(0, 0, 0, 255));
}

void Player::handleEvents(SDL_Event* event)
{
	m_velocityGoal = { 0, 0 };
    const Uint8* _currentKeyStates = SDL_GetKeyboardState(NULL);
	
	if (!g_isPlayerDead)
	{
		if (_currentKeyStates[SDL_SCANCODE_W])        m_velocityGoal.y = (float)-m_playerSpeed;
		if (_currentKeyStates[SDL_SCANCODE_A])        m_velocityGoal.x = (float)-m_playerSpeed;
		if (_currentKeyStates[SDL_SCANCODE_S])        m_velocityGoal.y = (float)m_playerSpeed;
		if (_currentKeyStates[SDL_SCANCODE_D])        m_velocityGoal.x = (float)m_playerSpeed;

		if (event->type == SDL_MOUSEBUTTONDOWN) m_isClickPressed = true;
		else if (event->type == SDL_MOUSEBUTTONUP) m_isClickPressed = false;
	}
}

//Collision for player to tile handling
void Player::checkCollisionTypes(
	Tile* tileTypes,
	int dimW,
	int dimH)
{
	for (int i = 0; i < 5; i++)
		for (int ii = 0; ii < 5; ii++)
			if ((i + (int)floor(m_collisionBox.position.y / 16)) * dimW + (ii + (int)floor(m_collisionBox.position.x / 16)) > 0 && 
				(i + (int)floor(m_collisionBox.position.y / 16)) * dimW + (ii + (int)floor(m_collisionBox.position.x / 16)) < dimW * dimH)
				if (Collision(m_collisionBox, tileTypes[(i + (int)floor(m_collisionBox.position.y / 16)) * dimW +
					(ii + (int)floor(m_collisionBox.position.x / 16))].m_collisionBox))
				{
					switch (tileTypes[(i + (int)floor(m_collisionBox.position.y / 16)) * dimW +
						(ii + (int)floor(m_collisionBox.position.x / 16))].m_id)
					{
						case 1: //Solid
						{
							m_isCollided = true;
						} break;

						case 2: //Slow
						{
							m_playerSpeed = 2;
						} break;

						case 3: //Slippery
						{
							m_slipAmount = 10;
							m_playerSpeed = 6;
						} break;
					}
				}
}

void Player::update(
	float deltaTime,
	Tile* tileTypes,
	int dimW,
	int dimH,
	Vector2f cursorPosition,
	Camera* camera)
{
	static Vector2f _tempVelocity;

	//Linear Interpolate the player's velocity
	_tempVelocity.x = lerpApproach(m_velocityGoal.x, _tempVelocity.x, deltaTime * m_slipAmount);
	_tempVelocity.y = lerpApproach(m_velocityGoal.y, _tempVelocity.y, deltaTime * m_slipAmount);
	m_velocity = _tempVelocity * deltaTime * 100;
	m_velocity.normalized();

	//Update x values
	m_isCollided = false;
	m_position.x = m_position.x + m_velocity.x;
	m_collisionBox = { Vector2f(m_position.x + 24, m_position.y + 24), 16, 16 };
	checkCollisionTypes(tileTypes, dimW, dimH);
	if (m_isCollided || m_collisionBox.position.x < 0 || m_collisionBox.position.x + m_collisionBox.width > dimW * 16)
		m_position.x = m_position.x - m_velocity.x;

	//Update y values
	m_isCollided = false;
	m_position.y = m_position.y + m_velocity.y;
	m_collisionBox = { Vector2f(m_position.x + 24, m_position.y + 24), 16, 16 };
	checkCollisionTypes(tileTypes, dimW, dimH);
	if (m_isCollided || m_collisionBox.position.y < 0 || m_collisionBox.position.y + m_collisionBox.height > dimH * 16)
		m_position.y = m_position.y - m_velocity.y;

	m_collisionCircle.setPosition(Vector2f(m_position.x + 16, m_position.y + 16));
	m_collisionCircle.update(deltaTime);

	//Moves the camera and makes sure the camera doesen't scroll past the map.
	if (dimW * 16 >= SCREEN_WIDTH || dimH * 16 >= SCREEN_HEIGHT)
	{
		camera->m_collisionBox.position.x = m_position.x - SCREEN_WIDTH / 2;
		camera->m_collisionBox.position.y = m_position.y - SCREEN_HEIGHT / 2;
		if (camera->m_collisionBox.position.x < 0) camera->m_collisionBox.position.x = 0;
		if (camera->m_collisionBox.position.y < 0) camera->m_collisionBox.position.y = 0;
		if (camera->m_collisionBox.position.x + camera->m_collisionBox.width > dimW * 16)
			camera->m_collisionBox.position.x = dimW * 16.f - camera->m_collisionBox.width;
		if (camera->m_collisionBox.position.y + camera->m_collisionBox.height > dimH * 16)
			camera->m_collisionBox.position.y = dimH * 16.f - camera->m_collisionBox.height;
	}

	m_direction = cursorPosition - Vector2f(m_position.x + m_collisionBox.width / 2, m_position.y + m_collisionBox.height / 2);
	m_direction = m_direction.normalized();
	m_angle = (float)(atan2(m_direction.y, m_direction.x) * (180.f / PI));
	m_weapon->setDirection(m_direction);
	m_weapon->update(m_position, m_angle, deltaTime);

	if (m_isClickPressed) m_weapon->action();
	if (m_health <= 0) g_isPlayerDead = true;
}
