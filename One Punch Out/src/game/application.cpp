#include "game/application.h"

Application::Application() :
    m_appState(appRunning),
    countedFrames(0)
{
    printf("Application Initializing... \n");
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
		printf("SDL could not initialize!\n");
	else
	{
		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	
		gWindow = SDL_CreateWindow("One Punch Out", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		if(gWindow == NULL)
			printf("Window could not be created!\n");
		else
		{
			m_context = SDL_GL_CreateContext(gWindow);
			if (m_context == NULL)
				printf("OpenGL Context could not be created!\n");
			else
			{
				int flags = IMG_INIT_PNG;
				if(!(IMG_Init(flags) & flags))
					printf("SDL_image could not initialize!\n");
				if (TTF_Init() == -1)
					printf("SDL_ttf could not initialize!\n");
			}
			//Initialize SDL_mixer
			if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				printf("SDL_mixer could not initialize! %s\n", Mix_GetError());
		}
	}
    SDL_ShowCursor(SDL_DISABLE);

	///Initialize OpenGL
	//Initialize GLEW
	if (glewInit() != GLEW_OK)
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewInit()));

	if (!GLEW_VERSION_2_1)
		printf("OpenGL 2.1 not supported!\n");

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//Initiailize Matrices
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.f, 1.f, -1.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();

	glClearColor(0.f, 0.f, 0.f, 1.f);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (glGetError() != GL_NO_ERROR)
		printf("Error initializing OpenGL! %s\n", gluErrorString(glGetError()));

	changeFontSize(64);

	gameState = new Menu();
	//gameState = new Level("Dungeons/World/World.dmm");
	
	changeFontSize(16);
}

Application::~Application()
{
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(gWindow);

	delete gameState;

	SDL_Quit();
    IMG_Quit();
	TTF_Quit();
}

void Application::run()
{
	m_appState = appRunning;
    printf("Application State: Running \n");
	while (m_appState != appExiting)
    {
		m_fpsTimer.start();
		deltaTime_f = m_deltaTimer.getTicks() / 1000.f;
        //Delta may get too large due to a game freeze
        if(deltaTime_f >= 0.15)
            deltaTime_f = 0.15f;
		m_deltaTimer.start();

        loop();
    }

    printf("Application State: Exiting \n");
}

void Application::loop()
{
	static Uint32 lastTime;
	if (SDL_GetTicks() - lastTime >= 1000)
	{
		m_fpsTime.str("");
		m_fpsTime << "FPS: " << countedFrames;
		m_fpsTex.loadFromText(m_fpsTime.str().c_str(), color(255, 255, 255, 255));
		lastTime = SDL_GetTicks();
		countedFrames = 0;
	}
    ///----------------
    /// Game Update
    ///----------------
	while (SDL_PollEvent(&event) != 0)
	{
		if (event.type == SDL_QUIT)
			m_appState = appExiting;
		else if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
				m_appState = appExiting;
		}
		else if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.sym == SDLK_LSHIFT)
				showCollisionBox = !showCollisionBox;
		}
		gameState->HandleEvents();
	}
    //Update current game state
    gameState->Update(deltaTime_f);

    ///----------------
    /// Game Rendering
    ///----------------
	glClear(GL_COLOR_BUFFER_BIT);

    //Render current game state
    gameState->Render();
	m_fpsTex.Render(Camera::getInstance().collisionBox.position.x, Camera::getInstance().collisionBox.position.y);

	SDL_GL_SwapWindow(gWindow);

	countedFrames++;
	//Cap fps to SCREEN_FPS
	int frameTicks = m_fpsTimer.getTicks();
	//if (frameTicks < SCREEN_TICKS_PER_FRAME) { SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks); }
}
