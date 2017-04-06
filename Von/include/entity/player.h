/*
Playable character of the game
Camera follows the player
Shoots projectiles out of the weapon
*/

#pragma once

#include "game/map.h"

#include "entity/entity.h"
#include "entity/gun.h"
#include "entity/cursor.h"

class Player : public Entity
{
public:
	Player();
	~Player() = default;

    void render();
	//Render health bar
	void renderUI(Camera* m_camera);
	void update(
		float deltaTime,
		Tile* tileTypes,
		int dimW,
		int dimH,
		Vector2f& cursorPosition,
		Camera* m_camera);
    void handleEvents(SDL_Event* event);

	void checkCollisionTypes(Tile* tileTypes, int dimW, int dimH);

	void hit(); //Plays a hit sound
    void setPlayerHealth(int health) { m_health = health; }
    int getPlayerHealth() { return m_health; }
	int getMaxHealth() { return m_maxHealth; }

	Gun* getWeapon() { return m_weapon; }
private:
    int m_playerSpeed;

    bool m_isCollided;
	bool m_isClickPressed;

	Sfx m_hitSfx;

	const int m_maxHealth = 3; //3 default
	int m_health;

	Gun* m_weapon;

	Projectile m_collisionCircle;
};
