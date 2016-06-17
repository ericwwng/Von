#pragma once

#include "utils/general.h"
#include "utils/singleton.h"
#include "utils/timer.h"

#include "game/camera.h"
#include "game/map.h"

#include "physics/physics.h"
#include "physics/AABB.h"

#include "gfx/texture.h"

#include "utils/Vector2f.h"

#include "entity/entity.h"
#include "entity/weapon.h"
#include "entity/weapons/gun.h"

class Player : public Entity
{
    public:
        Player(Vector2f pos);
        ~Player();

        void Render() const;
		void RenderUI() const;
		void Update(float deltaTime, Tile* tileTypes, int dimW, int dimH);
        void HandleEvents();

        //Weapon action on activation of space key.
        void Action();
    private:
		void CheckCollisionTypes(Tile* tileTypes, int dimW);

        Vector2f velocityGoal;
        int playerSpeed;
		int slipAmount;

        bool isCollided;

		//Attributes
		Texture staminaBar;
		int stamina;

		Weapon* weapon;
};

class Cursor : public Singleton<Cursor>, public Entity
{
    public:
        Cursor() {tex.loadFromFile("res/cursor.png");}

        void Render() const;
        void Update(float deltaTime);
};
