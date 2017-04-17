/*
Base class for all game states to inherit from
*/

#pragma once

#include <SDL.h>

class GameState
{
public:
	virtual void render() {};
	virtual void update(float deltaTime) {};
	virtual void handleEvents(SDL_Event* event) {};
};