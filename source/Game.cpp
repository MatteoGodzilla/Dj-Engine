#include "Game.h"

//Game::Game() {}

void Game::init(GLFWwindow* w) {
	m_render.init(w);

	CSimpleIniA ini;
	const char* section = "Engine";

	ini.LoadFile("config.ini");

	m_audioLatency = ini.GetDoubleValue(section, "audioLatency", 0.0);
	m_deckSpeed = ini.GetDoubleValue(section, "noteVisible", 1.0);
	m_isButtonsRight = ini.GetBoolValue(section, "buttonsRight", false);
	m_debugView = ini.GetBoolValue(section, "debugView", false);

	m_render.m_greenLaneActiveColor.r = ini.GetDoubleValue(section, "greenLaneActiveR", 0.0);
	m_render.m_greenLaneActiveColor.g = ini.GetDoubleValue(section, "greenLaneActiveG", 0.0);
	m_render.m_greenLaneActiveColor.b = ini.GetDoubleValue(section, "greenLaneActiveB", 0.0);
	m_render.m_greenLaneActiveColor.a = ini.GetDoubleValue(section, "greenLaneActiveA", 0.0);

	m_render.m_greenLaneInactiveColor.r = ini.GetDoubleValue(section, "greenLaneInactiveR", 0.0);
	m_render.m_greenLaneInactiveColor.g = ini.GetDoubleValue(section, "greenLaneInactiveG", 0.0);
	m_render.m_greenLaneInactiveColor.b = ini.GetDoubleValue(section, "greenLaneInactiveB", 0.0);
	m_render.m_greenLaneInactiveColor.a = ini.GetDoubleValue(section, "greenLaneInactiveA", 0.0);

	m_render.m_blueLaneActiveColor.r = ini.GetDoubleValue(section, "blueLaneActiveR", 0.0);
	m_render.m_blueLaneActiveColor.g = ini.GetDoubleValue(section, "blueLaneActiveG", 0.0);
	m_render.m_blueLaneActiveColor.b = ini.GetDoubleValue(section, "blueLaneActiveB", 0.0);
	m_render.m_blueLaneActiveColor.a = ini.GetDoubleValue(section, "blueLaneActiveA", 0.0);

	m_render.m_blueLaneInactiveColor.r = ini.GetDoubleValue(section, "blueLaneInactiveR", 0.0);
	m_render.m_blueLaneInactiveColor.g = ini.GetDoubleValue(section, "blueLaneInactiveG", 0.0);
	m_render.m_blueLaneInactiveColor.b = ini.GetDoubleValue(section, "blueLaneInactiveB", 0.0);
	m_render.m_blueLaneInactiveColor.a = ini.GetDoubleValue(section, "blueLaneInactiveA", 0.0);

	m_render.m_redLaneActiveColor.r = ini.GetDoubleValue(section, "redLaneActiveR", 0.0);
	m_render.m_redLaneActiveColor.g = ini.GetDoubleValue(section, "redLaneActiveG", 0.0);
	m_render.m_redLaneActiveColor.b = ini.GetDoubleValue(section, "redLaneActiveB", 0.0);
	m_render.m_redLaneActiveColor.a = ini.GetDoubleValue(section, "redLaneActiveA", 0.0);

	m_render.m_greenScratchColor.r = ini.GetDoubleValue(section, "greenScratchR", 0.0);
	m_render.m_greenScratchColor.g = ini.GetDoubleValue(section, "greenScratchG", 0.0);
	m_render.m_greenScratchColor.b = ini.GetDoubleValue(section, "greenScratchB", 0.0);
	m_render.m_greenScratchColor.a = ini.GetDoubleValue(section, "greenScratchA", 0.0);

	m_render.m_blueScratchColor.r = ini.GetDoubleValue(section, "blueScratchR", 0.0);
	m_render.m_blueScratchColor.g = ini.GetDoubleValue(section, "blueScratchG", 0.0);
	m_render.m_blueScratchColor.b = ini.GetDoubleValue(section, "blueScratchB", 0.0);
	m_render.m_blueScratchColor.a = ini.GetDoubleValue(section, "blueScratchA", 0.0);

	m_render.m_euphoriaLaneColor.r = ini.GetDoubleValue(section, "euphoriaLaneActiveR", 0.0);
	m_render.m_euphoriaLaneColor.g = ini.GetDoubleValue(section, "euphoriaLaneActiveG", 0.0);
	m_render.m_euphoriaLaneColor.b = ini.GetDoubleValue(section, "euphoriaLaneActiveB", 0.0);
	m_render.m_euphoriaLaneColor.a = ini.GetDoubleValue(section, "euphoriaLaneActiveA", 0.0);

	m_render.m_euphoriaZoneColor.r = ini.GetDoubleValue(section, "euphoriaZoneActiveR", 0.0);
	m_render.m_euphoriaZoneColor.g = ini.GetDoubleValue(section, "euphoriaZoneActiveG", 0.0);
	m_render.m_euphoriaZoneColor.b = ini.GetDoubleValue(section, "euphoriaZoneActiveB", 0.0);
	m_render.m_euphoriaZoneColor.a = ini.GetDoubleValue(section, "euphoriaZoneActiveA", 0.0);

	/*
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
		m_audioLatency = std::stof(token);
		input >> token;
		m_deckSpeed = std::stof(token);
		input >> token;
		m_isButtonsRight = token == "true";
		input >> token;
		m_debugView = token == "true";

		float r;
		float g;
		float b;
		float a;

		input >> token;
		r = std::stof(token);
		input >> token;
		g = std::stof(token);
		input >> token;
		b = std::stof(token);
		input >> token;
		a = std::stof(token);
		m_render.m_greenLaneActiveColor = glm::vec4(r, g, b, a);

		input >> token;
		r = std::stof(token);
		input >> token;
		g = std::stof(token);
		input >> token;
		b = std::stof(token);
		input >> token;
		a = std::stof(token);
		m_render.m_greenLaneInactiveColor = glm::vec4(r, g, b, a);

		input >> token;
		r = std::stof(token);
		input >> token;
		g = std::stof(token);
		input >> token;
		b = std::stof(token);
		input >> token;
		a = std::stof(token);
		m_render.m_blueLaneActiveColor = glm::vec4(r, g, b, a);

		input >> token;
		r = std::stof(token);
		input >> token;
		g = std::stof(token);
		input >> token;
		b = std::stof(token);
		input >> token;
		a = std::stof(token);
		m_render.m_blueLaneInactiveColor = glm::vec4(r, g, b, a);

		input >> token;
		r = std::stof(token);
		input >> token;
		g = std::stof(token);
		input >> token;
		b = std::stof(token);
		input >> token;
		a = std::stof(token);
		m_render.m_redLaneActiveColor = glm::vec4(r, g, b, a);

		input >> token;
		r = std::stof(token);
		input >> token;
		g = std::stof(token);
		input >> token;
		b = std::stof(token);
		input >> token;
		a = std::stof(token);
		m_render.m_euphoriaLaneColor = glm::vec4(r, g, b, a);

		input >> token;
		r = std::stof(token);
		input >> token;
		g = std::stof(token);
		input >> token;
		b = std::stof(token);
		input >> token;
		a = std::stof(token);
		m_render.m_greenScratchColor = glm::vec4(r, g, b, a);

		input >> token;
		r = std::stof(token);
		input >> token;
		g = std::stof(token);
		input >> token;
		b = std::stof(token);
		input >> token;
		a = std::stof(token);
		m_render.m_blueScratchColor = glm::vec4(r, g, b, a);

		input >> token;
		r = std::stof(token);
		input >> token;
		g = std::stof(token);
		input >> token;
		b = std::stof(token);
		input >> token;
		a = std::stof(token);
		m_render.m_euphoriaZoneColor = glm::vec4(r, g, b, a);
		m_player.readMappingFile();
	} else {
		std::cerr << "Game Error: Cannot open config file" << std::endl;
	}
	*/
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
			if (glfwGetKey(m_render.getWindowPtr(), GLFW_KEY_ESCAPE)) {
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

void Game::tick() {
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
			if (m_global_time >= (double)-m_audioLatency) {
				m_audio.play();
			}

			//add delta time to m_global_time
			double nowTime = glfwGetTime();
			m_deltaTime = nowTime - m_pastTime;
			m_global_time += m_deltaTime;
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

			m_render.clicker();
			m_render.lanes(m_global_time, m_note_arr, m_cross_arr);
			m_render.events(m_global_time, m_event_arr, m_cross_arr);
			m_render.notes(m_global_time, m_note_arr, m_cross_arr);

			m_render.updateAnimations(m_global_time);
			m_render.clickerAnimation();

			//debug
			if (m_debugView) {
				m_render.debug(m_deltaTime, m_note_arr, m_event_arr, m_cross_arr);
			}
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

void Game::start(const SongEntry& entry) {
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
	CSimpleIniA ini;
	const char* section = "Engine";
	ini.SetDoubleValue(section, "audioLatency", m_audioLatency);
	ini.SetDoubleValue(section, "noteVisible", m_deckSpeed);
	ini.SetBoolValue(section, "buttonsRight", m_isButtonsRight);
	ini.SetBoolValue(section, "debugView", m_debugView);

	ini.SetDoubleValue(section, "greenLaneActiveR", m_render.m_greenLaneActiveColor.r);
	ini.SetDoubleValue(section, "greenLaneActiveG", m_render.m_greenLaneActiveColor.g);
	ini.SetDoubleValue(section, "greenLaneActiveB", m_render.m_greenLaneActiveColor.b);
	ini.SetDoubleValue(section, "greenLaneActiveA", m_render.m_greenLaneActiveColor.a);

	ini.SetDoubleValue(section, "greenLaneInactiveR", m_render.m_greenLaneInactiveColor.r);
	ini.SetDoubleValue(section, "greenLaneInactiveG", m_render.m_greenLaneInactiveColor.g);
	ini.SetDoubleValue(section, "greenLaneInactiveB", m_render.m_greenLaneInactiveColor.b);
	ini.SetDoubleValue(section, "greenLaneInactiveA", m_render.m_greenLaneInactiveColor.a);

	ini.SetDoubleValue(section, "blueLaneActiveR", m_render.m_blueLaneActiveColor.r);
	ini.SetDoubleValue(section, "blueLaneActiveG", m_render.m_blueLaneActiveColor.g);
	ini.SetDoubleValue(section, "blueLaneActiveB", m_render.m_blueLaneActiveColor.b);
	ini.SetDoubleValue(section, "blueLaneActiveA", m_render.m_blueLaneActiveColor.a);

	ini.SetDoubleValue(section, "blueLaneInactiveR", m_render.m_blueLaneInactiveColor.r);
	ini.SetDoubleValue(section, "blueLaneInactiveG", m_render.m_blueLaneInactiveColor.g);
	ini.SetDoubleValue(section, "blueLaneInactiveB", m_render.m_blueLaneInactiveColor.b);
	ini.SetDoubleValue(section, "blueLaneInactiveA", m_render.m_blueLaneInactiveColor.a);

	ini.SetDoubleValue(section, "redLaneActiveR", m_render.m_redLaneActiveColor.r);
	ini.SetDoubleValue(section, "redLaneActiveG", m_render.m_redLaneActiveColor.g);
	ini.SetDoubleValue(section, "redLaneActiveB", m_render.m_redLaneActiveColor.b);
	ini.SetDoubleValue(section, "redLaneActiveA", m_render.m_redLaneActiveColor.a);

	ini.SetDoubleValue(section, "greenScratchR", m_render.m_greenScratchColor.r);
	ini.SetDoubleValue(section, "greenScratchG", m_render.m_greenScratchColor.g);
	ini.SetDoubleValue(section, "greenScratchB", m_render.m_greenScratchColor.b);
	ini.SetDoubleValue(section, "greenScratchA", m_render.m_greenScratchColor.a);

	ini.SetDoubleValue(section, "blueScratchR", m_render.m_blueScratchColor.r);
	ini.SetDoubleValue(section, "blueScratchG", m_render.m_blueScratchColor.g);
	ini.SetDoubleValue(section, "blueScratchB", m_render.m_blueScratchColor.b);
	ini.SetDoubleValue(section, "blueScratchA", m_render.m_blueScratchColor.a);

	ini.SetDoubleValue(section, "euphoriaLaneActiveR", m_render.m_euphoriaLaneColor.r);
	ini.SetDoubleValue(section, "euphoriaLaneActiveG", m_render.m_euphoriaLaneColor.g);
	ini.SetDoubleValue(section, "euphoriaLaneActiveB", m_render.m_euphoriaLaneColor.b);
	ini.SetDoubleValue(section, "euphoriaLaneActiveA", m_render.m_euphoriaLaneColor.a);

	ini.SetDoubleValue(section, "euphoriaZoneActiveR", m_render.m_euphoriaZoneColor.r);
	ini.SetDoubleValue(section, "euphoriaZoneActiveG", m_render.m_euphoriaZoneColor.g);
	ini.SetDoubleValue(section, "euphoriaZoneActiveB", m_render.m_euphoriaZoneColor.b);
	ini.SetDoubleValue(section, "euphoriaZoneActiveA", m_render.m_euphoriaZoneColor.a);

	ini.SaveFile("config.ini");
	std::cout << "Game Message: Written engine conigs to 'config.ini'" << std::endl;

	m_player.writeMappingFile();
}

Game::~Game() {
	m_note_arr.clear();
	m_event_arr.clear();
}
