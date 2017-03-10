#include "entity/warp.h"

Warp::Warp()
{
	currentLevel = 0;

	m_texture.loadFromFile("res/Entity/warp.png", 64, 64, true);
	m_position = { 0, 0 };
	m_collisionBox = { m_position, 64, 64};
}

void Warp::render() const
{
	m_texture.render(m_position.x, m_position.y);
}

void Warp::update(float deltaTime)
{
	m_collisionBox = { m_position, 64, 64 };
}

void Warp::goToNextLevel()
{
	//Intro(0) -> Transition(1) -> BigMoney -> Transition(3) -> Satori -> Transition(4) -> Sun
	currentLevel++;

	delete g_gameState;

	switch (currentLevel)
	{
		case 0:
		{
			g_gameState = new Level("Levels/Introduction.opo", "Introduction.opo");
			break;
		}
		case 1:
		{
			g_gameState = new Level("Levels/Transition.opo", "Transition.opo");
			break;
		}
		case 2:
		{
			g_gameState = new Level("Levels/BigMoney.opo", "BigMoney.opo");
			break;
		}
		case 3:
		{
			g_gameState = new Level("Levels/Transition.opo", "Transition.opo");
			break;
		}
		case 4:
		{
			g_gameState = new Level("Levels/Satori.opo", "Satori.opo");
			break;
		}
		case 5:
		{
			g_gameState = new Level("Levels/Transition.opo", "Transition.opo");
			break;
		}
		case 6:
		{
			g_gameState = new Level("Levels/Sun.opo", "Sun.opo");
		}
		default:
		{
			g_gameState = new Level("Levels/Transition.opo", "Transition.opo");
			break;
		}
	}
}

void Warp::goToCertainLevel(int level)
{
	currentLevel = level - 1;
	printf("%d", level);
	goToNextLevel(); 
}