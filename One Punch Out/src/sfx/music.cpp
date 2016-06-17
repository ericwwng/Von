#include "sfx/music.h"

Bgm::Bgm()
{
	m_Music = NULL;
}

Bgm::Bgm(const char* filename)
{
	m_Music = Mix_LoadMUS(filename);
	if (m_Music == NULL)
		printf("Failed to load music! Mix_GetError: %s\n", Mix_GetError());
}

Bgm::~Bgm()
{
	Mix_FreeMusic(m_Music);
	m_Music = NULL;
}

void Bgm::loadMusicFile(const char* filename)
{
	Mix_FreeMusic(m_Music);
	m_Music = Mix_LoadMUS(filename);
	if (m_Music == NULL)
		printf("Failed to load music! Mix_GetError: %s\n", Mix_GetError());
}

void Bgm::playMusic()
{
	if (Mix_PlayingMusic() == 0)
	{
		Mix_PlayMusic(m_Music, -1); //-1 is nearest channel
	}
}

void Bgm::repeatMusic()
{
	Mix_PlayMusic(m_Music, -1); //-1 is infinite loop
}

void Bgm::pauseMusic()
{
	Mix_PauseMusic();
}

void Bgm::resumeMusic()
{
	if (Mix_PausedMusic())
	{
		Mix_ResumeMusic();
	}
}

void Bgm::stopMusic()
{
	Mix_HaltMusic();
}

Sfx::Sfx()
{
	m_Chunk = NULL;
}

Sfx::Sfx(const char* filename)
{
	m_Chunk = Mix_LoadWAV(filename);
	if (m_Chunk == NULL)
		printf("Failed to load sound! Mix_GetError: %s\n", Mix_GetError());
}

Sfx::~Sfx()
{
	Mix_FreeChunk(m_Chunk);
	m_Chunk = NULL;
}

void Sfx::loadSoundFile(const char* filename)
{
	Mix_FreeChunk(m_Chunk);
	m_Chunk = Mix_LoadWAV(filename);
	if (m_Chunk == NULL)
		printf("Failed to load sound! Mix_GetError: %s\n", Mix_GetError());
}

void Sfx::playSound()
{
	Mix_HaltChannel(-1);
	Mix_PlayChannel(-1, m_Chunk, 0); //-1 is nearest channel, repeat 0 times
}