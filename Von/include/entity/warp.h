/*
Warp entity for moving between different levels
Singleton for now unless a better solution is proposed and tested
*/
#pragma once

#include "entity/entity.h"

#include "game/level.h"

#include "utils/singleton.h"

class Warp : public Singleton<Warp>, public Entity
{
public:
	Warp();
	~Warp() = default;

	void render() const;
	void update(float deltaTime);

	void goToCertainLevel(int level);

	void goToNextLevel();
private:
	int currentLevel;
};
