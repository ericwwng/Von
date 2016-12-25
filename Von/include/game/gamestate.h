#pragma once

class GameState
{
    public:
        virtual ~GameState(){};

		virtual void render() {};
		virtual void update(float deltaTime) {};
		virtual void handleEvents() {};
};
