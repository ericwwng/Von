#include "game/application.h"

SDL_Window* g_window = NULL;
TTF_Font* g_font = NULL;
bool g_showCollisionBox = false;
bool g_isPlayerDead = false;
int g_bgmVolume = 10;
int g_sfxVolume = 25;
GameState* g_gameState = NULL;

int main(int argc, char* args[])
{
	Application::getInstance().run();
    return 0;
}
