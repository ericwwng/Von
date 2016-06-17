#include "utils/timer.h"

Timer::Timer()
{
    startTicks = 0;
}

void Timer::start()
{
    started = true;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    started = false;

	startTicks = 0;
}

unsigned int Timer::getTicks() const
{
    if(started)
        return SDL_GetTicks() - startTicks;

    return SDL_GetTicks();
}
