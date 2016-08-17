#include "utils/timer.h"

Timer::Timer()
{
	m_startTicks = 0;
}

void Timer::start()
{
	m_started = true;

    //Get the current clock time
	m_startTicks = SDL_GetTicks();
}

void Timer::stop()
{
	m_started = false;

	m_startTicks = 0;
}

unsigned int Timer::getTicks() const
{
    if(m_started)
        return SDL_GetTicks() - m_startTicks;

    return SDL_GetTicks();
}
