#pragma once

#include <GL/glew.h>
#include <GL/GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "game/gamestate.h"

#define SCREEN_WIDTH            1280
#define SCREEN_HEIGHT           720
#define SCREEN_FPS              60
#define SCREEN_TICKS_PER_FRAME  1000 / SCREEN_FPS

#define PI 3.14159265

extern SDL_Window* gWindow;
extern SDL_Event event;
extern TTF_Font* Font;

extern bool showCollisionBox;

extern GameState* gameState;
