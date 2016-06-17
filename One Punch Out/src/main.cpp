#include "game/application.h"

SDL_Window* gWindow = NULL;
TTF_Font* Font = NULL;
SDL_Event event;
bool showCollisionBox   = true;
GameState* gameState = NULL;

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

int main(int argc, char* args[])
{
    Application::getInstance().run();
    return 0;
}
