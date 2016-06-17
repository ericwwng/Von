#include "game/map.h"

Map::Map(std::string filename)
{
    std::ifstream file(filename, std::ifstream::binary);
	if (file)
    {
		long index = 8; //start at end of attributes

        //Finding file length
        file.seekg(0, file.end);
        int length = (int)file.tellg();
        file.seekg(0, file.beg);

        std::cout << "Reading Map " << filename << std::endl;
        //File Attributes
		unsigned char* data = new unsigned char[length];
		file.read((char*)data, length);

		std::string bgfilename = "res/Backgrounds/";
		bgfilename.append(1, data[4] + '0');
		bgfilename.append(".png");
		printf("Background: %s\n", bgfilename.c_str());
		m_background.loadFromFile(bgfilename.c_str());

		width = SCREEN_WIDTH / 16;
		height = SCREEN_HEIGHT / 16;
		//width = m_background.getWidth() / 16;
        //height = m_background.getHeight() / 16;
        printf("Tile Width: %d\nTile Height: %d\n", width, height);

		//m_playerSpawnPosition.x = (float)((data[5] * 16 * 16) + (data[6] * 16));
		//m_playerSpawnPosition.y = (float)((data[7] * 16 * 16) + (data[8] * 16));
		m_playerSpawnPosition.x = 0;
		m_playerSpawnPosition.y = 0;
		printf("Player Spawn(%d, %d)\n", (int)m_playerSpawnPosition.x, (int)m_playerSpawnPosition.y);

		Camera::getInstance().setCoords(Vector2f(m_playerSpawnPosition.x - SCREEN_WIDTH / 2, m_playerSpawnPosition.y - SCREEN_HEIGHT / 2));
		if (Camera::getInstance().collisionBox.position.x < 0) Camera::getInstance().collisionBox.position.x = 0;
		if (Camera::getInstance().collisionBox.position.y < 0) Camera::getInstance().collisionBox.position.y = 0;
		if (Camera::getInstance().collisionBox.position.x + Camera::getInstance().collisionBox.width > width * 16) Camera::getInstance().collisionBox.position.x = width * 16 - Camera::getInstance().collisionBox.width;
		if (Camera::getInstance().collisionBox.position.y + Camera::getInstance().collisionBox.height > height * 16) Camera::getInstance().collisionBox.position.y = height * 16 - Camera::getInstance().collisionBox.height;

		int tilecounter = 0;
		Vector2f tilePosition;
		Rectf Clip = {0.f, 0.f, 32.f, 32.f};

		tilePosition = { 0.f, 0.f };
		tilecounter = 0;
        //Tile types
		m_solidTiles = new Tile[width * height];
		/*while (tilecounter < width * height) {
			for (int i = 0; i < data[index] + 1; i++)
			{
				m_solidTiles[i] = Tile(tilePosition, data[index + 1]);
				tilecounter++;
				tilePosition.x = tilePosition.x + 16;
				if (tilecounter % width == 0)
				{
					tilePosition.x = 0;
					tilePosition.y = tilePosition.y + 16;
				}
			}
			index += 2;
		}*/
		for (int i = 0; i < height; i++)
			for (int ii = 0; ii < width; ii++)
				m_solidTiles[i * width + ii] = Tile(Vector2f((GLfloat)ii * 16, (GLfloat)i * 16), 0);

		tilePosition = { 0, 0 };
		tilecounter = 0;

		printf("Successfully Read!\n");
		file.close();
		delete data;
    }
	else
	{
		std::cout << "Map " << filename << "not found" << std::endl;
		file.close();
	}

	//enemyEntities.push_back(e);
	
}

Map::~Map()
{
	delete[] m_solidTiles;
}

void Map::Render()
{
	m_background.Render(0, 0);
}

void Map::renderSolidTiles()
{
    for(int i = 0; i < height; i++)
		for (int ii = 0; ii < width; ii++)
		{
			//Rectf box = { ii * 16, i * 16, 16.f, 16.f};
			//renderEmptyBox(box);
			m_solidTiles[i * width + ii].Render();
		}
}

/*void Map::renderObjectTiles() 
{
	for (int i = 0; i < SCREEN_HEIGHT / 32 + 2; i++)
		for (int ii = 0; ii < SCREEN_WIDTH / 32 + 2; ii++)
		{
			int index = (i + (int)(floor(Camera::getInstance().collisionBox.position.y / 32))) * width + (ii + (int)(floor(Camera::getInstance().collisionBox.position.x / 32)));

			if (m_tileTypes[index].id != 0)
				m_tileTypes[index].Render();
		}

	for (unsigned int i = 0; i < m_enemyEntities.size(); i++)
		m_enemyEntities[i]->Render();
}
*/

Tile::Tile() :
id(0)
{
	collisionBox = { Vector2f(0, 0), 32, 32 };
}
Tile::Tile(Vector2f pos, Uint8 tileType) :
id(tileType)
{
	collisionBox = { pos, 32, 32 };
}

void Tile::Render()
{
	Rectf box = { collisionBox.position.x, collisionBox.position.y, 16.f, 16.f };
	if (showCollisionBox) renderEmptyBox(box, color(64, 64, 64, 128));
	if (showCollisionBox)
	{
		switch(id)
		{
			case 0: //No collision - transparent
			
			break;
			case 1: //Solid - GRAY
				renderFillRect(box, color(200, 200, 200, 128));
			break;
			case 2: //Slow - GREEN
				renderFillRect(box, color(0, 255, 0, 128));
			break;
			case 3: //Slippery - BLUE
				renderFillRect(box, color(0, 0, 255, 128));
			break;
			case 4: //DEATH - RED
				renderFillRect(box, color(255, 0, 0, 128));
			break;
		}
	}
}

///Tile Colors///
/*
Solid Layer Types:
0 - No Collision - Transparent
1 - Solid - RED
2 - Slow - GREEN
3 - Slippery - BLUE
*/