#include "Game.h"

Game::Game() {}

void Game::init(GLFWwindow* w) {
	m_render.init(w);
	std::ifstream input("config.txt");
	std::string s;
	while (s != std::string("{Engine}")) {
		std::getline(input, s);
		if (input.eof()) {
			std::cerr << "Game Error: found config file, but not {Engine} marker.";
			std::cerr << "Stopped loading of config file" << std::endl;
			return;
		}
	}
	if (input.is_open()) {
		std::cout << "Game Message: loading from config file" << std::endl;
		std::string token;
		input >> token;
		/*
		output << m_audioLatency << std::endl;
		output << m_deckSpeed << std::endl;
		output << m_isButtonsRight << std::endl << std::endl;
		*/

		m_audioLatency = std::stof(token);
		input >> token;
		m_deckSpeed = std::stof(token);
		input >> token;
		m_isButtonsRight = token == "true" ? true : false;
		input >> token;
		m_debugView = token == "true" ? true : false;
		m_player.readMappingFile();
	} else {
		std::cerr << "Game Error: Cannot open config file" << std::endl;
	}
}

void Game::pollInput() {
	if (m_active) {
		m_player.pollInput(m_render.getWindowPtr());
		if (m_mode == 0) {
			m_player.hit(m_global_time, m_note_arr, m_event_arr, m_cross_arr);
			//m_player.keyCallback(key, action, m_global_time, m_note_arr, m_event_arr,m_cross_arr);
			if (glfwGetKey(m_render.getWindowPtr(), GLFW_KEY_T)) {
				std::cout << m_global_time << std::endl;
			}
			if (glfwGetKey(m_render.getWindowPtr(), GLFW_KEY_F1)) {
				m_mode = 1;
			}
		} else {
			if (m_player.m_wasGreenPressed && !m_player.m_isGreenPressed) {
				m_active = false;
				reset();
			}
		}
	}
}

void Game::tick(double dt) {
	if (m_active) {
		if (m_mode == 0) {
			m_render.m_noteVisibleTime = m_deckSpeed;
			m_gen.m_deckSpeed = m_deckSpeed;

			//update notes and read chart (text or .fsgmub)
			m_gen.tick(m_global_time, m_note_arr, m_event_arr, m_cross_arr);
			//m_gen.textParser(m_note_arr, m_event_arr);
			m_gen.addNotesToBuffer(m_note_arr, m_event_arr, m_cross_arr);
			m_gen.bpm(m_global_time, m_bpm_arr);

			//update player (combo + multiplier)
			m_player.pollState(m_gen);
			m_player.tick(m_global_time);

			m_render.pollState(m_global_time, m_player, m_gen);
			m_audio.buffer();
			if (m_global_time >= (double)-m_audioLatency)
				m_audio.play();

			//add delta time to m_global_time
			double nowTime = glfwGetTime();
			m_global_time += nowTime - m_pastTime;
			m_pastTime = nowTime;
		}
		if (!m_audio.isActive(m_global_time)) {
			m_mode = 1;
			m_audio.stop();
		}
	}
}

void Game::render() {
	if (m_active) {
		if (m_mode == 0) {
			//each function is responsible for rendering a single 'object' on screen
			m_render.highway(m_global_time);
			m_render.bpmTicks(m_global_time, m_bpm_arr);
			m_render.meters(m_global_time);

			m_render.events(m_global_time, m_event_arr, m_cross_arr);
			m_render.clicker();
			m_render.lanes(m_global_time, m_note_arr, m_cross_arr);
			m_render.notes(m_global_time, m_note_arr, m_cross_arr);

			m_render.updateAnimations(m_global_time);
			m_render.clickerAnimation();

			//debug
			if (m_debugView)
				m_render.debug(m_note_arr, m_event_arr, m_cross_arr);
		} else if (m_mode == 1) {
			m_render.result(m_player, m_gen);
		}
	}
}

void Game::reset() {
	m_note_arr.clear();
	m_event_arr.clear();
	m_cross_arr.clear();
	m_bpm_arr.clear();

	m_global_time = -2.0f;
	m_active = false;
	firstRun = true;
	m_mode = 0;

	m_player.m_deltaMouse = false;

	m_render.reset();
	m_gen.reset();
	m_player.reset();
	m_audio.reset();
}

bool Game::getActive() {
	return m_active;
}

void Game::setActive(bool active) {
	m_active = active;
}

Player* Game::getPlayer() {
	return &m_player;
}

Audio* Game::getAudio() {
	return &m_audio;
}

GameRender* Game::getGameRender() {
	return &m_render;
}

void Game::setButtonPos(bool value) {
	m_isButtonsRight = value;
	m_render.m_isButtonsRight = value;
	m_player.m_isButtonsRight = value;
}

void Game::start(SongEntry entry) {
	std::cout << "Game msg: started game" << std::endl;
	glfwSetTime(0.0);
	//glfwSetInputMode(m_render.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//m_player.m_deltaMouse = true;
	m_pastTime = glfwGetTime();
	m_global_time = -2.0f;

	std::string audioPath = entry.path + std::string("/song.ogg");
	m_audio.load(audioPath.c_str());

	m_gen.init(entry);
	m_bpm_arr.push_back(0.0);

	m_active = true;
	m_mode = 0;
}

void Game::writeConfig() {
	std::ofstream output("config.txt");
	if (output.is_open()) {
		output << std::boolalpha;
		output << "{Engine}" << std::endl;
		output << m_audioLatency << std::endl;
		output << m_deckSpeed << std::endl;
		output << m_isButtonsRight << std::endl;
		output << m_debugView << std::endl
			   << std::endl;

		std::cout << "Game Message: updated engine values in config file." << std::endl;
		output.close();
	}

	m_player.writeMappingFile();
}

Game::~Game() {
	m_note_arr.clear();
	m_event_arr.clear();
}
