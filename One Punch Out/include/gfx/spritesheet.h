#pragma once

#include <vector>
#include "gfx/texture.h"

class SpriteSheet : public Texture
{
	public:
		SpriteSheet();
		~SpriteSheet();

		void free();
		void freeSheet();

		int addClipSprite(Rectf& newClip); //returns index
		Rectf getClip(int index);

		void generateDataBuffer();

		void RenderSprite(GLfloat x, GLfloat y, int index);
	private:
		std::vector<Rectf> Clips;

		GLuint vertexDataBuffer;
		GLuint* indexBuffers;

};