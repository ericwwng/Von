#pragma once

#include "utils/Vector2f.h"

#include "physics/AABB.h"

#include "gfx/texture.h"
#include "gfx/spritesheet.h"

class Entity
{
    public:
        void Render() {};
        void Update() {};
        void HandleEvents() {};

        Vector2f& getPosition() {return position;}

        AABB& getCollisionBox() {return collisionBox;}
    protected:
        Vector2f position;
        Vector2f velocity;
        Vector2f direction;

        float angle;

        AABB collisionBox;

        Texture tex;
		SpriteSheet spriteSheet;
};