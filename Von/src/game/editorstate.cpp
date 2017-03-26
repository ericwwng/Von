#include "game/editorstate.h"

EditorState::EditorState(
	std::string filename,
	std::string worldName)
{
	m_camera = new Camera();
	m_camera->setCoords(Vector2f(0, 0));
	changeFontSize(16);

	m_dungeon = new Map(filename, worldName);

	m_id = 0;

	SDL_ShowCursor(SDL_ENABLE);
}

EditorState::~EditorState()
{

}

void EditorState::render()
{
	m_dungeon->render();
	m_dungeon->renderSolidTiles();
}

void EditorState::update(float deltaTime)
{
	m_camera->update();
}

void EditorState::handleEvents(SDL_Event* event)
{
	if (event->type == SDL_MOUSEWHEEL)
	{
		if (event->wheel.y > 0) m_id++;
		else if (event->wheel.y < 0) m_id--;
		m_selectorID.str("");
		m_selectorID << "Tile: " << m_id;
		m_selectorText.loadFromText(m_selectorID.str().c_str(), color(255, 255, 255, 255));
		printf("Current Tile Selected: %d \n", m_id);
	}

	if (event->type == SDL_KEYDOWN)
	{
		if (event->key.keysym.sym == SDLK_1 || event->key.keysym.sym == SDLK_2 ||
			event->key.keysym.sym == SDLK_3 || event->key.keysym.sym == SDLK_4)
		{
			switch (event->key.keysym.sym)
			{
				case SDLK_1:
				{
					m_id -= 10;
				} break;
				case SDLK_2:
				{
					m_id -= 1;
				} break;
				case SDLK_3:
				{
					m_id += 1;
				} break;
				case SDLK_4:
				{
					m_id += 10;
				}break;
			}
			m_selectorID.str("");
			m_selectorID << "Tile: " << m_id;
			m_selectorText.loadFromText(m_selectorID.str().c_str(), color(255, 255, 255, 255));
			printf("Current Tile Selected: %d \n", m_id);
		}
		if (event->key.keysym.sym == SDLK_LEFT || event->key.keysym.sym == SDLK_RIGHT ||
			event->key.keysym.sym == SDLK_UP || event->key.keysym.sym == SDLK_DOWN)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			switch (event->key.keysym.sym)
			{
				case SDLK_LEFT:
				{
					SDL_WarpMouseInWindow(g_window, x - 16, y);
				} break;
				case SDLK_RIGHT:
				{
					SDL_WarpMouseInWindow(g_window, x + 16, y);
				} break;
				case SDLK_UP:
				{
					SDL_WarpMouseInWindow(g_window, x, y - 16);
				} break;
				case SDLK_DOWN:
				{
					SDL_WarpMouseInWindow(g_window, x, y + 16);
				} break;
			}
		}

		if (event->key.keysym.sym == SDLK_s)
			m_dungeon->saveMap();
		if (event->key.keysym.sym == SDLK_n)
		{
			m_dungeon->saveMap();
			printf("Creating New Map\n");
			m_dungeon->newMap(80, 45, 4, "Sun.opo");
		}

		if (event->key.keysym.sym == SDLK_ESCAPE)
		{
			m_dungeon->saveMap();
			delete g_gameState;
			g_gameState = new Menu(false);
		}
	}

	if (event->type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		m_dungeon->setSolid((m_dungeon->getDimW() * (y / 16)) + (x / 16), m_id);
	}

	while(SDL_PollEvent(event))
	{
		if (event->type == SDL_QUIT)
		{
			g_gameState = new Menu(false);
		}
	}
}
