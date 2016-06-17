#pragma once

#include "utils/general.h"
#include <stdio.h>

#include "game/camera.h"

#include "gfx/texutils.h"

#include "utils/general.h"

class Texture
{
    public:
        Texture();
        virtual ~Texture();

		void loadFromFile(const char* path, GLuint w = 32, GLuint h = 32);
		void loadFromText(const char* text, SDL_Color color);

        void free();
		void freeVBO();

        void setAlpha(Uint8 a);

		void Render(GLfloat x, GLfloat y, Rectf* clip = NULL, float angle = 0.0, SDL_Point* center = NULL) const;

		GLuint getTextureID() const;
		GLuint getWidth() const;
		GLuint getHeight() const;

    private:
        //texture id
		GLuint texture;
		GLenum textureFormat;
		GLuint width, height;
		GLuint VBOid, IBOid; //Vertex Buffer Object id, Index Buffer Object id

		Uint8 alpha;
};
