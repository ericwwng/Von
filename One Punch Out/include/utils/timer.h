#pragma once

#include <SDL.h>

class Timer
{
    public:
		Timer();

		void start();
		void stop();

		//Gets the timer's time in milliseconds
		unsigned int getTicks() const;

    private:
		unsigned int startTicks;

		bool started;
};
