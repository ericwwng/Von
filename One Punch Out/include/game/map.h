#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>

#include "game/camera.h"

#include "entity/enemy.h"

#include "gfx/spritesheet.h"
#include "gfx/texutils.h"

#include "utils/Vector2f.h"
#include "physics/AABB.h"

struct Tile
{
public:
	Tile();
	Tile(Vector2f pos, Uint8 tileType);

	void Render();

	AABB collisionBox;
	Uint8 id; //on the tilesheet
};

class Map
{
    public:
		Map(std::string filename);
        ~Map();

		void Render();
        void renderSolidTiles();

		std::string getFilename() { return m_filename; }

		Tile* getSolids() { return m_solidTiles; }

		GLuint getDimW() const { return width; }
		GLuint getDimH() const { return height; }

		Vector2f getPlayerSpawn() { return m_playerSpawnPosition; }
		void setPlayerSpawn(Vector2f spawnPos) { m_playerSpawnPosition = spawnPos; }

		void setSolid(GLuint tileIndex, Uint8 id) { m_solidTiles[tileIndex].id = id; }
    private:
		std::string m_filename;

		Tile* m_solidTiles;
		std::vector<Enemy*> m_enemyEntities;

		Vector2f m_playerSpawnPosition;

		Texture m_background;

        GLuint width, height; //should be image res / 8
};