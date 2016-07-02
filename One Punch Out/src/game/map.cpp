#include "game/map.h"

Map::Map(std::string filename) 
{
	loadMap(filename);

	return;
	m_filename = filename;
    std::ifstream file(filename, std::ifstream::binary);
	if (file)
    {
		long index = 6; //start at end of attributes

        //Finding file length
        file.seekg(0, file.end);
        int length = (int)file.tellg();
        file.seekg(0, file.beg);

        std::cout << "Reading Map " << filename << std::endl;
        //File Attributes
		GLubyte* data = new GLubyte[length];
		file.read((char*)data, length);

		std::string bgfilename = "res/Backgrounds/";
		m_backgroundNumber = data[4];
		bgfilename.append(1, data[4] + '0');
		bgfilename.append(".png");
		printf("Background number: %d  // Background: %s\n", m_backgroundNumber, bgfilename.c_str());
		m_background.loadFromFile(bgfilename.c_str(), 1280, 1080);

		m_width = SCREEN_WIDTH / 16;
		m_height = SCREEN_HEIGHT / 16;
		//width = m_background.getWidth() / 16;
        //height = m_background.getHeight() / 16;
        printf("Tile Width: %d\nTile Height: %d\n", m_width, m_height);

		m_playerSpawnPosition.x = (float)(data[7] * 16);
		m_playerSpawnPosition.y = (float)(data[8] * 16);

		printf("Player Spawn(%d, %d)\n", (int)m_playerSpawnPosition.x, (int)m_playerSpawnPosition.y);

		Camera::getInstance().setCoords(Vector2f(m_playerSpawnPosition.x - SCREEN_WIDTH / 2, m_playerSpawnPosition.y - SCREEN_HEIGHT / 2));
		if (Camera::getInstance().collisionBox.position.x < 0) Camera::getInstance().collisionBox.position.x = 0;
		if (Camera::getInstance().collisionBox.position.y < 0) Camera::getInstance().collisionBox.position.y = 0;
		if (Camera::getInstance().collisionBox.position.x + Camera::getInstance().collisionBox.width > m_width * 16) Camera::getInstance().collisionBox.position.x = m_width * 16 - Camera::getInstance().collisionBox.width;
		if (Camera::getInstance().collisionBox.position.y + Camera::getInstance().collisionBox.height > m_height * 16) Camera::getInstance().collisionBox.position.y = m_height * 16 - Camera::getInstance().collisionBox.height;

		int tilecounter = 0;
		Vector2f tilePosition;
		Rectf Clip = {0.f, 0.f, 16.f, 16.f};

		tilePosition = { 0.f, 0.f };
		tilecounter = 0;
        //Tile types
		m_solidTiles = new Tile[m_width * m_height];
		/*while (tilecounter < m_width * m_height) {
			for (int i = 0; i < data[index] + 1; i++)
			{
				m_solidTiles[tilecounter] = Tile(tilePosition, data[index + 1]);
				tilecounter++;
				tilePosition.x = tilePosition.x + 16;
				if (tilecounter % m_width == 0)
				{
					tilePosition.x = 0;
					tilePosition.y = tilePosition.y + 16;
				}
			}
			index += 2;
		}
		*/
		for (int i = 0; i < m_height; i++)
			for (int ii = 0; ii < m_width; ii++)
				m_solidTiles[i * m_width + ii] = Tile(Vector2f((GLfloat)ii * 16, (GLfloat)i * 16), 0);
		
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
    for(int i = 0; i < m_height; i++)
		for (int ii = 0; ii < m_width; ii++)
		{
			//Rectf box = { ii * 16, i * 16, 16.f, 16.f};
			//renderEmptyBox(box);
			m_solidTiles[i * m_width + ii].Render();
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

void writeInt(std::ofstream& p_fileStream, GLuint p_uint)
{
	p_fileStream << GLubyte((p_uint & 0xFF000000) >> 24);
	p_fileStream << GLubyte((p_uint & 0xFF0000) >> 16);
	p_fileStream << GLubyte((p_uint & 0xFF00) >> 8);
	p_fileStream << GLubyte((p_uint & 0xFF));
}
void writeShort(std::ofstream& p_fileStream, GLushort p_ushort)
{
	p_fileStream << GLubyte((p_ushort & 0xFF00) >> 8);
	p_fileStream << GLubyte((p_ushort & 0xFF));
}
void writeChar(std::ofstream& p_fileStream, GLubyte p_uchar)
{
	p_fileStream << p_uchar;
}

void Map::saveMap()
{
	std::cout << "Saving map " << m_filename << std::endl;

	std::ofstream _file;

	//_mkdir(std::string("Levels\\").c_str());
	_file.open(m_filename, std::ios::binary);

	_file << ".OPO";
	writeChar(_file, m_backgroundNumber);
	writeChar(_file, m_width);
	writeChar(_file, m_height);

	writeShort(_file, round(m_playerSpawnPosition.x));
	writeShort(_file, round(m_playerSpawnPosition.y));

	GLubyte _lastId = 0;
	GLubyte _count = 0;
	for (int i = 0; i < m_width * m_height; i++)
	{
		if (_lastId == m_solidTiles[i].id && _count < 255)
		{
			_count++;
		}
		else
		{
			_lastId = m_solidTiles[i].id;
			if (_count > 0)
			{
				writeChar(_file, _count);
				writeChar(_file, _lastId);
				_count = 0;
			}
		}
	}
	if (_count > 0)
	{
		writeChar(_file, _count);
		writeChar(_file, _lastId);
		_count = 0;
	}

	_file.close();

	std::cout << "Map saved." << std::endl;
}

GLubyte readChar(char* p_file, long& p_index)
{
	p_index = p_index + 1;
	return GLubyte(p_file[p_index - 1]);
}

GLushort readShort(char* p_file, long& p_index)
{
	GLushort _value;
	_value = readChar(p_file, p_index) << 8;
	_value += readChar(p_file, p_index);
	return _value;
}

GLuint readInt(char* p_file, long& p_index)
{
	GLushort _value;
	_value = readShort(p_file, p_index) << 16;
	_value += readShort(p_file, p_index);
	return _value;
}

void Map::loadMap(std::string p_filename)
{
	std::ifstream _file;
	_file.open(p_filename.c_str(), std::ios::binary);

	if(!_file.good())
	{
		std::cout << "Error finding file " << p_filename << "." << std::endl;
		return;
	}

	_file.seekg(0, _file.end);
	long _len = _file.tellg();
	_file.seekg(0, _file.beg);

	long _index = 0;
	char* _data = new char[_len];
	_file.read(_data, _len);

	if(_data[0] != '.' || _data[1] != 'O' || _data[2] != 'P' || _data[3] != 'O')
	{
		std::cout << "First 4 characters are not .OPO, file not trusted." << std::endl;
		return;
	}
	_index = 4;

	m_backgroundNumber = readChar(_data, _index);
	m_background.loadFromFile(std::string("res\\Backgrounds\\").append(1, m_backgroundNumber + '0').append(".png").c_str(), 1280, 1080);
	printf("Background number: %d  // Background: %s\n", m_backgroundNumber, std::string("res\\Backgrounds\\").append(1, m_backgroundNumber + '0').append(".png"));

	m_width = readChar(_data, _index);
	m_height = readChar(_data, _index);
	m_playerSpawnPosition = Vector2f(readShort(_data, _index), readShort(_data, _index));

	int i = 0;
	GLubyte _amt = 0;
	GLubyte _id = 0;
	delete[] m_solidTiles;
	m_solidTiles = new Tile[m_width * m_height];
	while(i < m_width * m_height)
	{
		_amt = readShort(_data, _index);
		_id = readShort(_data, _index);
		for(int j = 0; j < _amt; j++)
		{
			m_solidTiles[i].id = _id;
			i++;
		}
	}
	
	_file.close();
	delete[] _data;
}

Tile::Tile() :
id(0)
{
	collisionBox = { Vector2f(0, 0), 32, 32 };
}
Tile::Tile(Vector2f pos, GLubyte tileType) :
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