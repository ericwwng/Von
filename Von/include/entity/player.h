#pragma once

#include "utils/general.h"
#include "utils/timer.h"

#include "game/camera.h"
#include "game/map.h"

#include "physics/physics.h"
#include "physics/AABB.h"

#include "gfx/texture.h"

#include "sfx/music.h"

#include "entity/entity.h"
#include "entity/gun.h"
#include "entity/cursor.h"

class Player : public Entity
{
public:
	Player();
	~Player() = default;

    void render();
	void renderUI(Camera* m_camera);
	void update(
		float deltaTime,
		Tile* tileTypes,
		int dimW,
		int dimH,
		Vector2f cursorPosition,
		Camera* m_camera);
    void handleEvents(SDL_Event* event);

	void setSpawnPosition(Vector2f pos) { m_position = pos; }

	void checkCollisionTypes(
		Tile* tileTypes,
		int dimW,
		int dimH);

	void setHit();
    void setPlayerHealth(int health) { m_health = health; }
    int getPlayerHealth() { return m_health; }
	int getMaxHealth() { return m_maxHealth; }

	Gun* getWeapon() { return m_weapon; }
private:
    int m_playerSpeed;
	int m_slipAmount;

    bool m_isCollided;
	bool m_isClickPressed;

	Sfx m_hitSfx;

	int m_maxHealth = 3; //3 default
	int m_health;

	Gun* m_weapon;

	Projectile m_collisionCircle;
};
