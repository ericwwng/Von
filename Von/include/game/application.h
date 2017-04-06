/*
Game application interface that loops through the game state
Displays FPS counter
*/

#pragma once

#include <iostream>
#include <sstream>
#include <ctime>

#include "game/level.h"

#include "utils/singleton.h"
#include "utils/timer.h"

enum AppState {
	APP_INITIALZING,
	APP_RUNNING,
	APP_EXITING
};

class Application : public Singleton<Application>
{
public:
    Application();
    ~Application();

    void run();
    void loop();

    void setAppState(AppState appState);
private:
	SDL_GLContext m_context;
	SDL_Event m_event;

	AppState m_appState;

    Timer m_deltaTimer;
    float deltaTime;

	//For FPS counter
    Texture m_fpsTex;
    std::stringstream m_fpsTime;
    int m_countedFrames;
};
