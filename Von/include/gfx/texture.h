/*
Abstraction to hold all texture information
render is a beast that should be somehow optimized however I am very unsure of how to do
*/

#pragma once

#include <iostream>

#include "game/camera.h"

#include "gfx/texutils.h"

class Texture
{
public:
    Texture();
    virtual ~Texture();

	void loadFromFile(const char* path, GLuint w = 32, GLuint h = 32, bool printLoaded = true);
	void loadFromText(const char* text, SDL_Color color);

	//Deallocate the textures from the GPU
    void free();
	void freeVBO();

	inline void setAlpha(GLubyte alpha) { m_alpha = alpha; }
	
	void render(
		GLfloat x,
		GLfloat y,
		Rectf* clip = NULL,
		GLfloat stretchWidth = NULL,
        GLfloat stretchHeight = NULL,
		float angle = 0.f,
		Vector2f* center = NULL,
		SDL_Color color = color(255, 255, 255, 255),
		GLfloat alpha = 255.f) const;

	GLuint getWidth() const { return m_width; }
	GLuint getHeight() const { return m_height; }
	
	GLuint getTextureID() const { return m_texture; }
private:
	GLuint m_texture;
	GLenum m_textureFormat;
	GLuint m_width, m_height;
	GLuint m_VBOid, m_IBOid; //Vertex Buffer Object id, Index Buffer Object id

	GLubyte m_alpha;
};
