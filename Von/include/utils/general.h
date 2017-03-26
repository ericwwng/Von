#pragma once

#define GLEW_STATIC // This must be defined when statically linking to GLEW
#include <GL/glew.h>

#pragma comment (lib, "glew32s.lib") // Link to glew32s.lib

#include <GL/GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "game/gamestate.h"

constexpr unsigned SCREEN_WIDTH  = 1280;
constexpr unsigned SCREEN_HEIGHT = 720;

constexpr float PI = 3.14159265f;

extern SDL_Window* g_window;
extern TTF_Font* g_font;

extern bool g_showCollisionBox;
extern bool g_isPlayerDead;

extern int g_bgmVolume;
extern int g_sfxVolume;

extern GameState* g_gameState;