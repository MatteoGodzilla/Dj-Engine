#include "Audio.h"

Audio::Audio() {
	m_engine = createIrrKlangDevice();
	if (!m_engine) {
		std::cerr << "ERROR LOADING IRRKLANG ENGINE" << std::endl;
	}
}

void Audio::play() {
	if (firstRun) {
		std::cout << "play" << std::endl;
		m_engine->play2D("res/song.mp3");
		firstRun = false;
	}
}

Audio::~Audio() {
	m_engine->drop();
}