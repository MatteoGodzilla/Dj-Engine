#include "Game.h"

Game::Game() {}

void Game::init(GLFWwindow* w) {
	m_render.init(w);
}

void Game::pollInput() {
	if (m_active) {
		m_player.pollInput(m_render.getWindowPtr());
		if (m_mode == 0) {
			m_player.hit(m_global_time, m_note_arr, m_event_arr, m_cross_arr);
			//m_player.keyCallback(key, action, m_global_time, m_note_arr, m_event_arr,m_cross_arr);
			if (glfwGetKey(m_render.getWindowPtr(), GLFW_KEY_T))std::cout << m_global_time << std::endl;
			if (glfwGetKey(m_render.getWindowPtr(), GLFW_KEY_BACKSPACE))m_mode = 1;
		}
		else {
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
			//update notes and read chart (text or .fsgmub)
			m_gen.tick(m_global_time, m_note_arr, m_event_arr, m_cross_arr);
			//m_gen.textParser(m_note_arr, m_event_arr);
			m_gen.binaryParser(m_note_arr, m_event_arr, m_cross_arr);
			m_gen.bpm(m_global_time, m_bpm_arr);

			//update player (combo + multiplier)
			m_player.pollState(m_gen);
			m_player.tick(m_global_time);

			m_render.pollState(m_global_time, m_player, m_gen);
			m_audio.buffer();
			if (m_global_time >= 0.0)m_audio.play();

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
	if (m_active){
		if (m_mode == 0) {
			//each function is responsible for rendering a single 'object' on screen
			m_render.highway(m_global_time);
			m_render.bpmTicks(m_global_time, m_bpm_arr);
			m_render.meters();

			m_render.events(m_global_time, m_event_arr,m_cross_arr);
			m_render.clicker();
			m_render.lanes(m_global_time, m_note_arr, m_cross_arr);
			m_render.notes(m_global_time, m_note_arr,m_cross_arr);

			m_render.updateAnimations(m_global_time);
			m_render.clickerAnimation();

			//debug
			//m_render.debug(m_note_arr, m_event_arr, m_cross_arr);
		}
		else if (m_mode == 1) {
			m_render.result(m_player,m_gen);
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

	m_render.reset();
	m_gen.reset();
	m_player.reset();
	m_audio.reset();
}

bool Game::getActive() {
	return m_active;
}

void Game::setActive(bool active){
	m_active = active;
}

Player* Game::getPlayer(){
	return &m_player;
}

void Game::setButtonPos(bool value){
	m_isButtonsRight = value;
	m_render.m_isButtonsRight = value;
	m_player.m_isButtonsRight = value;
}

void Game::start(std::string path) {
	std::cout << "Game msg: started game" << std::endl;
	glfwSetTime(0.0);
	m_pastTime = glfwGetTime();
	m_global_time = -2.0f;
	
	std::string audioPath = path + std::string("/song.ogg");
	m_audio.load(audioPath.c_str());

	m_gen.init(path);
	m_bpm_arr.push_back(0.0);

	m_active = true;
	m_mode = 0;
}

Game::~Game() {
	m_note_arr.clear();
	m_event_arr.clear();
}

