#include "game/editorstate.h"

EditorState::EditorState(std::string filename)
{
	Camera::getInstance().setCoords(Vector2f(0, 0));
	changeFontSize(16);

	m_dungeon = new Map(filename);

	id = 0;

	SDL_ShowCursor(SDL_ENABLE);
}

EditorState::~EditorState()
{

}

void EditorState::Render() const
{
	Camera::getInstance().Update();

	m_dungeon->Render();
	m_dungeon->renderSolidTiles();
}

void EditorState::Update(float deltaTime)
{

}

void EditorState::HandleEvents()
{
	if (event.type == SDL_MOUSEWHEEL)
	{
		if (event.wheel.y > 0) id++;
		else if (event.wheel.y < 0) id--;
		m_selectorID.str("");
		m_selectorID << "Tile: " << id;
		m_selectorText.loadFromText(m_selectorID.str().c_str(), color(255, 255, 255, 255));
		printf("Current Tile Selected: %d \n", id);
	}

	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_1 || event.key.keysym.sym == SDLK_2 ||
			event.key.keysym.sym == SDLK_3 || event.key.keysym.sym == SDLK_4)
		{
			switch (event.key.keysym.sym)
			{
				case SDLK_1:
					id -= 10;
				break;
				case SDLK_2:
					id -= 1;
				break;
				case SDLK_3:
					id += 1;
				break;
				case SDLK_4:
					id += 10;
				break;
			}
			m_selectorID.str("");
			m_selectorID << "Tile: " << id;
			m_selectorText.loadFromText(m_selectorID.str().c_str(), color(255, 255, 255, 255));
			printf("Current Tile Selected: %d \n", id);
		}
		if ((event.key.keysym.sym == SDLK_LCTRL || event.key.keysym.sym == SDLK_RCTRL) && event.key.keysym.sym == SDLK_s)
		{
			printf("Map Saved \n");
		}
	}

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		m_dungeon->setSolid((m_dungeon->getDimW() * (y / 16)) + (x / 16), id);
	}

}