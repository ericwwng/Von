#pragma once

#include "utils/general.h"

class Bgm
{
	public:
		Bgm();
		~Bgm();

	private:
		Mix_Music* m_Music;
};