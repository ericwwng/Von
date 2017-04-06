/*
Helper functions for drawing primitves and Vertex buffer objects
*/
#pragma once

#include "utils/general.h"
#include "utils/vector2f.h"

SDL_Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

void renderEmptyBox(Rectf box, SDL_Color color);
void renderFillRect(Rectf box, SDL_Color color);
void renderFillCircle(Vector2f position, GLfloat radius, SDL_Color color, int numSegments = 32);

void changeFontSize(int ptsize);

struct VertexPos2f
{
	GLfloat x;
	GLfloat y;
};

struct TexCoord2f
{
	GLfloat s;
	GLfloat t;
};

struct VertexData2f
{
	VertexPos2f position;
	TexCoord2f texCoord;
};