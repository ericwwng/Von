#include "utils/timer.h"

Timer::Timer()
{
	m_startTicks = 0;
	m_ticksSet = false;
}

Timer::Timer(bool initialStart)
{
	m_startTicks = 0;
	m_ticksSet = false;
	if(initialStart) start();
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

void Timer::setTicks(unsigned int ticks)
{
	m_initialTicks = ticks;
	m_startTicks = m_initialTicks + SDL_GetTicks();
	m_ticksSet = true;
}

unsigned int Timer::getTicks() const
{
	if (m_started)
		return SDL_GetTicks() - m_startTicks;

    return SDL_GetTicks();
}
