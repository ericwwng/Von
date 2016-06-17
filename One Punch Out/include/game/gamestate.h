#pragma once

class GameState
{
    public:
        virtual ~GameState(){};

        virtual void Render() const = 0 ;
        virtual void Update(float deltaTime) = 0;
		virtual void HandleEvents() = 0;
};
