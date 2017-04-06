/*
For map file information
*/

#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <direct.h>

#include "gfx/texture.h"

#include "physics/AABB.h"

class Tile
{
public:
	Tile();
	Tile(Vector2f pos, GLubyte tileType);

	void render();

	AABB m_collisionBox;
	GLubyte m_id; //For tile specific properties
};

class Map
{
public:
	Map(std::string filename, std::string worldName);
    ~Map();

	void render();
    void renderSolidTiles();
	void updateScroll(float deltaTime); //For background scrolling

	void saveMap();
	int loadMap(std::string p_filename);
	void newMap(GLubyte width, GLubyte height, GLubyte backgroundNumber, std::string filename);

	std::string getFilename() { return m_filename; }

	Tile* getSolids() { return m_solidTiles; }

	GLubyte getDimW() const { return m_width; }
	GLubyte getDimH() const { return m_height; }

	Vector2f getPlayerSpawn() { return m_playerSpawnPosition; }
	inline void setPlayerSpawn(Vector2f spawnPos) { m_playerSpawnPosition = spawnPos; }

	inline void setSolid(GLuint tileIndex, Uint8 id) { m_solidTiles[tileIndex].m_id = id; }

	Texture getBackground() { return m_background; }
private:
	std::string m_filename;
	std::string m_worldname;

	Tile* m_solidTiles;

	Vector2f m_playerSpawnPosition;

	GLubyte m_backgroundNumber;
	Texture m_background;
	GLfloat m_backgroundXScroll;

    GLubyte m_width, m_height; //should be image res / 8
};
