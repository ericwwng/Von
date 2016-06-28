#include "gfx/texture.h"

Texture::Texture()
{
	texture = NULL;
	width = 0;
	height = 0;

	VBOid = 0;
	IBOid = 0;
}

Texture::~Texture()
{
	free();
	freeVBO();
}

void Texture::loadFromFile(const char* path, GLuint w, GLuint h)
{
	free();

	SDL_Surface* surface = IMG_Load(path);
	GLuint* pixels = NULL;
	if (surface == NULL)
		printf("Unable to load image %s!\n", path);

	if (surface != NULL)
	{
		width = surface->w;
		height = surface->h;
	}
	else
	{
		width = w;
		height = h;
		printf("creating texture %d, %d", w, h);
		textureFormat = GL_RGBA;
		pixels = new GLuint[w * h];
		for (GLuint i = 0; i < (w * h); i++)
		{
			GLubyte* colors = (GLubyte*)&pixels[i];
			colors[0] = 0xFF; colors[1] = 0xFF; colors[2] = 0xFF; colors[3] = 0xFF;
		}
	}

	if (surface != NULL)
	{
		if (surface->format->BytesPerPixel == 4)
			textureFormat = GL_RGBA;
		else if (surface->format->BytesPerPixel == 3)
			textureFormat = GL_RGB;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	if (surface != NULL)
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, NULL);

	if (glGetError() != GL_NO_ERROR)
		printf("Error loading texture! %s\n", gluErrorString(glGetError()));

	///Initialize VBO
	if (texture != 0 && VBOid == 0)
	{
		//Vertex data
		VertexData2f vData[4]; //To be set later in the render function
		GLuint iData[4];

		//Set rendering indices
		iData[0] = 0;
		iData[1] = 1;
		iData[2] = 2;
		iData[3] = 3;

		//VBO
		glGenBuffers(1, &VBOid);
		glBindBuffer(GL_ARRAY_BUFFER, VBOid);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData2f), vData, GL_DYNAMIC_DRAW);

		//IBO
		glGenBuffers(1, &IBOid);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOid);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), iData, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	}

	SDL_FreeSurface(surface);
}

void Texture::loadFromText(const char* text, SDL_Color color)
{
	free();

	SDL_Surface* surface = TTF_RenderText_Blended(Font, text, color);
	if (surface == NULL)
		printf("Unable to load image %s!\n", text);

	width = surface->w;
	height = surface->h;

	if (surface->format->BytesPerPixel == 4)
		textureFormat = GL_RGBA;
	else if (surface->format->BytesPerPixel == 3)
		textureFormat = GL_RGB;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, width, height, 0, textureFormat, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, NULL);

	if (glGetError() != GL_NO_ERROR)
		printf("Error loading texture! %s\n", gluErrorString(glGetError()));

	///Initialize VBO
	if (texture != 0 && VBOid == 0)
	{
		//Vertex data
		VertexData2f vData[4]; //To be set later in the render function
		GLuint iData[4];

		//Set rendering indices
		iData[0] = 0;
		iData[1] = 1;
		iData[2] = 2;
		iData[3] = 3;

		//VBO
		glGenBuffers(1, &VBOid);
		glBindBuffer(GL_ARRAY_BUFFER, VBOid);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(VertexData2f), vData, GL_DYNAMIC_DRAW);

		//IBO
		glGenBuffers(1, &IBOid);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOid);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), iData, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
	}

	SDL_FreeSurface(surface);
}

void Texture::free()
{
	if (texture != 0)
	{
		glDeleteTextures(1, &texture);
		texture = 0;
	}
	width = 0;
	height = 0;
}

void Texture::freeVBO()
{
	if(VBOid != 0)
	{
		glDeleteBuffers(1, &VBOid);
		glDeleteBuffers(1, &IBOid);
	}
}

void Texture::setAlpha(Uint8 a)
{
	alpha = a;
}

void Texture::Render(GLfloat x, GLfloat y, Rectf* clip, float angle, SDL_Point* center) const
{
	if (texture != 0)
	{
		Rectf texCoords = { 0.f, 1.f, 0.f, 1.f }; //Top Bottom Left Right

		GLfloat quadWidth = (GLfloat)width;
		GLfloat quadHeight = (GLfloat)height;

		if (clip != NULL)
		{
			//Convert to texture coordinates
			texCoords = { clip->y / height, (clip->y + clip->h) / height, clip->x / width, (clip->x + clip->w) / width };

			quadWidth = clip->w;
			quadHeight = clip->h;
		}

		glPopMatrix();
		glPushMatrix();

		glColor3f(1.f, 1.f, 1.f);

		glTranslatef(x, y, 0.f);

		if (center != NULL)
		{
			glTranslatef((GLfloat)center->x, (GLfloat)center->y, 0.f);
			glRotatef(angle, 0.f, 0.f, 1.f);
			glTranslatef((GLfloat)-center->x, (GLfloat)-center->y, 0.f);
		}

		VertexData2f vData[4]; //Vertex Data

		vData[0].texCoord.s = texCoords.w; vData[0].texCoord.t = texCoords.x;
		vData[1].texCoord.s = texCoords.h; vData[1].texCoord.t = texCoords.x;
		vData[2].texCoord.s = texCoords.h; vData[2].texCoord.t = texCoords.y;
		vData[3].texCoord.s = texCoords.w; vData[3].texCoord.t = texCoords.y;

		vData[0].position.x = 0.f;			vData[0].position.y = 0.f;
		vData[1].position.x = quadWidth;	vData[1].position.y = 0.f;
		vData[2].position.x = quadWidth;	vData[2].position.y = quadHeight;
		vData[3].position.x = 0.f;			vData[3].position.y = quadHeight;
	
		glBindTexture(GL_TEXTURE_2D, texture);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, VBOid);

			//Update vertex buffer data
			glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(VertexData2f), vData);

			glTexCoordPointer(2, GL_FLOAT, sizeof(VertexData2f), (GLvoid*)offsetof(VertexData2f, texCoord));
			glVertexPointer(2, GL_FLOAT, sizeof(VertexData2f), (GLvoid*)offsetof(VertexData2f, position));

			//Draw quad using vertex data and index data
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOid);
			glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, NULL);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		glBindTexture(GL_TEXTURE_2D, NULL);
	}
}

GLuint Texture::getTextureID() const
{
	return texture;
}

GLuint Texture::getWidth() const
{
	return width;
}

GLuint Texture::getHeight() const
{
	return height;
}
