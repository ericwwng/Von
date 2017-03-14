#pragma once

#include "utils/general.h"
#include "utils/singleton.h"

#include "game/level.h"

#include "sfx/music.h"

#include "entity/entity.h"


class Warp : public Singleton<Warp>, public Entity
{
public:
	Warp();
	~Warp() = default;

	void render() const;
	void update(float deltaTime);

	void goToCertainLevel(int level);

	void goToNextLevel();

	void setPosition(Vector2f pos) { m_position = pos; }
	AABB getCollisionBox() { return m_collisionBox; }
private:
	int currentLevel;

	AABB m_collisionBox;
};
