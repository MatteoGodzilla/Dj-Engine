#pragma once

#include "irrklang/irrKlang.h"
#include <iostream>

using namespace irrklang;

class Audio
{
public:
	Audio();
	~Audio();
	void play();
private:
	ISoundEngine* m_engine;
	bool firstRun = true;
};

