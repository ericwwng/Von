/*
Timer class using sdl's functions
Some code used from lazyfoo
Should be revamped to use <chrono> for more accuracy
*/

#pragma once

#include "utils/general.h"

class Timer
{
public:
	Timer();
	Timer(bool isStarted);

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
