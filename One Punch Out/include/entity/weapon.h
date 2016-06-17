#pragma once

#include "utils/general.h"
#include "utils/timer.h"

#include "gfx/texture.h"

#include "utils/Vector2f.h"

#include "entity/entity.h"

class Weapon : public Entity
{
    public:
        virtual ~Weapon() {}

        virtual void Render() const {}
		virtual void Update(Vector2f& pos, float angle, float deltaTime) {  }

        virtual void Action() {}

        void setDirection(Vector2f& dir) {weaponDirection = dir;}
    protected:
		Vector2f UpdateRotation();

        Vector2f weaponDirection;

        Timer rateOfFire;
};