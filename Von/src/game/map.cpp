#include "game/map.h"

inline void writeShort(std::ofstream& fileStream, GLushort ushort)
{
	fileStream << GLubyte((ushort & 0xFF00) >> 8);
	fileStream << GLubyte((ushort & 0xFF));
}

inline void writeByte(std::ofstream& fileStream, GLubyte ubyte)
{
	fileStream << ubyte;
}

inline GLubyte readByte(char* file, long& index)
{
	index = index + 1;
	return GLubyte(file[index - 1]);
}

inline GLushort readShort(char* file, long& index)
{
	GLushort _value;
	_value = readByte(file, index) << 8;
	_value += readByte(file, index);
	return _value;
}

Map::Map(std::string filename, std::string worldname) :
	m_filename(filename),
	m_worldname(worldname)
{
	m_solidTiles = NULL;

	loadMap(filename);
}

Map::~Map()
{
	delete[] m_solidTiles;
}

void Map::render()
{
	if (m_worldname == "Sun")
	{
		m_background.render(m_backgroundXScroll, 0, NULL, (GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);
		m_background.render(m_backgroundXScroll - SCREEN_WIDTH, 0, NULL, (GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT);
	}
	else m_background.render(0.f, 0.f);
}

void Map::updateScroll(float deltaTime)
{
	m_backgroundXScroll += 300 * deltaTime;
	if (m_backgroundXScroll >= SCREEN_WIDTH) m_backgroundXScroll = 0;
}

void Map::renderSolidTiles()
{
	for (int i = 0; i < m_height; i++)
	{
		for (int ii = 0; ii < m_width; ii++) m_solidTiles[i * m_width + ii].render();
	}
}

void Map::newMap(GLubyte width, GLubyte height, GLubyte backgroundNumber, std::string filename)
{
	m_width = width;
	m_height = height;
	m_backgroundNumber = backgroundNumber;
	m_filename = filename;

	printf("Creating new file: %s\n", m_filename.c_str());

	std::cout << "Saving map " << m_filename << std::endl;

	_mkdir(std::string("Levels\\").c_str());
	std::ofstream _file("Levels\\" + m_filename, std::ios::binary);

	_file << ".OPO";
	writeByte(_file, m_backgroundNumber);
	writeByte(_file, m_width);
	writeByte(_file, m_height);

	writeShort(_file, 0);
	writeShort(_file, 0);

	for (int i = 0; i < m_width * m_height; i++) m_solidTiles[i].m_id = 0;

	GLubyte _lastId = 0;
	GLubyte _count = 0;
	for (int i = 0; i < m_width * m_height; i++)
	{
		if (_count >= 255 || (int)_lastId != (int)m_solidTiles[i].m_id)
		{
			if (_count > 0)
			{
				writeByte(_file, _count);
				writeByte(_file, _lastId);
				_count = 0;
			}
			_lastId = m_solidTiles[i].m_id;
		}
		_count++;
	}
	if (_count > 0)
	{
		writeByte(_file, _count);
		writeByte(_file, _lastId);
		_count = 0;
	}

	_file.close();

	std::cout << "Map created." << std::endl;
}

void Map::saveMap()
{
	std::cout << "Saving map " << m_filename << std::endl;

	_mkdir(std::string("Levels\\").c_str());
	std::ofstream _file(m_filename, std::ios::binary);

	_file << ".OPO";
	writeByte(_file, m_backgroundNumber);
	writeByte(_file, m_width);
	writeByte(_file, m_height);

	writeShort(_file, (GLushort)m_playerSpawnPosition.x);
	writeShort(_file, (GLushort)m_playerSpawnPosition.y);

	GLubyte _lastId = 0;
	GLubyte _count = 0;
	for (int i = 0; i < m_width * m_height; i++)
	{
		if (_count >= 255 || (int)_lastId != (int)m_solidTiles[i].m_id)
		{
			if (_count > 0)
			{
				writeByte(_file, _count);
				writeByte(_file, _lastId);
				_count = 0;
			}
			_lastId = m_solidTiles[i].m_id;
		}
		_count++;
	}
	if (_count > 0)
	{
		writeByte(_file, _count);
		writeByte(_file, _lastId);
		_count = 0;
	}

	_file.close();

	std::cout << "Map saved." << std::endl;
}

int Map::loadMap(std::string p_filename)
{
	printf("Reading %s of file %s\n", m_worldname.c_str(), p_filename.c_str());

	std::ifstream _file;
	_file.open(p_filename.c_str(), std::ios::binary);

	if(!_file.good())
	{
		std::cout << "Error finding file " << p_filename << "." << std::endl;
		return 1;
	}

	_file.seekg(0, _file.end);
	std::streamoff _len = _file.tellg();
	_file.seekg(0, _file.beg);

	long _index = 0;
	char* _data = new char[(GLuint)_len];
	_file.read(_data, _len);

	if(_data[0] != '.' || _data[1] != 'O' || _data[2] != 'P' || _data[3] != 'O')
	{
		std::cout << "First 4 characters are not .OPO, file not trusted." << std::endl;
		return 2;
	}
	_index = 4;

	m_backgroundNumber = readByte(_data, _index);
	std::string backgroundFile = "res\\Backgrounds\\" + std::to_string(m_backgroundNumber);
	backgroundFile.append(".png");
	m_background.loadFromFile(backgroundFile.c_str(), 1280, 1080);
	printf("Background number: %d  // Background: %s\n", m_backgroundNumber, backgroundFile.c_str());

	m_width = readByte(_data, _index);
	m_height = readByte(_data, _index);
	GLushort _x = readShort(_data, _index);
	GLushort _y = readShort(_data, _index);
	m_playerSpawnPosition = Vector2f(_x, _y);
	printf("Map Width: %d\nMap Height: %d \nPlayer Spawn: (%d, %d)\n",
		m_width, m_height, (int)m_playerSpawnPosition.x, (int)m_playerSpawnPosition.y);

	int i = 0;
	GLubyte _amt = 0;
	GLubyte _id = 0;
	if(m_solidTiles != NULL) delete[] m_solidTiles;
	m_solidTiles = new Tile[m_width * m_height];
	while (_index < _len)
	{
		_amt = readByte(_data, _index);
		_id = readByte(_data, _index);
		for (int j = 0; j < _amt; j++)
		{
			m_solidTiles[i] = Tile(Vector2f((GLfloat)(i % m_width), floor((GLfloat)i / m_width)) * 16, _id);
			i++;
		}
	}
	printf("Map Loaded\n\n");

	_file.close();
	delete[] _data;
	return 0;
}

Tile::Tile() :
	m_id(0)
{
	m_collisionBox = { Vector2f(0, 0), 32, 32 };
}
Tile::Tile(Vector2f pos, GLubyte tileType) :
	m_id(tileType)
{
	m_collisionBox = { pos, 32, 32 };
}

void Tile::render()
{
	Rectf box = { m_collisionBox.position.x, m_collisionBox.position.y, 16.f, 16.f };
	if (g_showCollisionBox)
	{
		switch(m_id)
		{
			case 0: //No collision - transparent
			{

			} break;
			case 1: //Solid - GRAY
			{
				renderFillRect(box, color(200, 200, 200, 255));
			} break;
			case 2: //Slow - GREEN
			{
				renderFillRect(box, color(0, 255, 0, 128));
			} break;
			case 3: //Slippery - BLUE
			{
				renderFillRect(box, color(0, 0, 255, 128));
			} break;
			case 4: //DEATH - RED
			{
				renderFillRect(box, color(255, 0, 0, 128));
			} break;
		}
	}
}

///Tile Colors for editor///
/*
Solid Layer Types:
0 - No Collision - Transparent
1 - Solid - RED
2 - Slow - GREEN
3 - Slippery - BLUE
*/
