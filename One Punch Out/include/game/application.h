#pragma once

#include "utils/general.h"
#include <iostream>
#include <sstream>

#include "utils/general.h"
#include "utils/singleton.h"
#include "utils/timer.h"

#include "gfx/texture.h"

#include "game/gamestate.h"
#include "game/camera.h"
#include "game/menu.h"
#include "game/level.h"

enum AppState {
    appInitializing,
    appRunning,
    appExiting
};

class Application : public Singleton<Application>
{
    public:
        Application();
        ~Application();

        void run();
        void loop();

    private:
		SDL_GLContext m_context;

        AppState m_appState;

        Timer m_deltaTimer, m_fpsTimer;
        float deltaTime_f;

        Texture m_fpsTex;
        std::stringstream m_fpsTime;
        int countedFrames;
};
