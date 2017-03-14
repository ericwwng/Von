#pragma once

#include <SDL.h>

class Timer
{
public:
	Timer();
    Timer(bool initialStart);

	void start();
	void stop();

	void setTicks(unsigned int ticks);
	//Gets the timer's time in milliseconds
	unsigned int getTicks() const;

private:
	unsigned int m_startTicks;
	unsigned int m_initialTicks;

	bool m_started;
	bool m_ticksSet;
};
