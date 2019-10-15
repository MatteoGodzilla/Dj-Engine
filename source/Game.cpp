#include "Game.h"

Game::Game() 
{
}

void Game::init(GLFWwindow* w) {
	m_render.init(w);
	m_audio.load("res/song.ogg");
}

void Game::render() {
	if (m_active){
		//each function is responsible for rendering a single 'object' on screen
		m_render.highway(m_global_time);
		m_render.bpmTicks(m_global_time, m_bpm_arr);
		m_render.meters();

		m_render.events(m_global_time, m_event_arr);
		m_render.clicker();
		m_render.lanes(m_global_time, m_event_arr);
		m_render.notes(m_global_time, m_note_arr);

		//debug
		m_render.debug(m_note_arr, m_event_arr);
	}
}

void Game::tick() {
	if (m_active) {
		//update notes and read chart (text or .fsgmub)
		m_gen.tick(m_global_time, m_note_arr, m_event_arr);
		m_gen.textParser(m_note_arr, m_event_arr);
		m_gen.binaryParser(m_note_arr, m_event_arr);
		m_gen.bpm(m_global_time, m_bpm_arr);

		//update player (combo + multiplier)
		m_player.pollState(m_gen);
		m_player.tick(m_global_time);

		m_render.pollState(m_global_time, m_player, m_gen);
		m_audio.buffer();
		if(m_global_time >= 0.0)m_audio.play();

		//add delta time to m_global_time
		double nowTime = glfwGetTime();
		m_global_time += nowTime - m_pastTime;
		m_pastTime = nowTime;

	}
	else {
		m_audio.stop();
	}
}

void Game::input(int key, int action) {
	if(m_active)
	{
		m_player.keyCallback(key, action, m_global_time, m_note_arr, m_event_arr);
		if (action == GLFW_PRESS && key == GLFW_KEY_T)std::cout << m_global_time << std::endl;
	}
}

void Game::setActive(bool active)
{
	m_active = active;
}

void Game::start() {
	if (!m_active) {
		std::cout << "Game msg: started game" << std::endl;
		glfwSetTime(0.0);
		m_pastTime = glfwGetTime();
		m_global_time = -2.0f;
		
		m_active = true;
	}
}

Game::~Game() {
	m_note_arr.clear();
	m_event_arr.clear();
}

