#include "Game.h"

Game::Game() 
{
}

void Game::init(GLFWwindow* w) {
	m_render.init(w);
	m_audio.load("res/song.ogg");
	std::cout << "Please press space to start game" << std::endl;
	resetTime();
}

void Game::render() {
	if (m_scene == 0) {
		m_render.splash();
	}
	else if (m_scene == 1){
		m_render.highway(m_global_time);
		m_render.bpmTicks(m_global_time, m_bpm_arr);
		m_render.events(m_global_time, m_event_arr);
		m_render.clicker();
		m_render.meters();
		m_render.lanes(m_global_time, m_event_arr);
		m_render.notes(m_global_time, m_note_arr);
	}
}

void Game::tick() {
	if (m_scene == 1) {
		
		m_gen.tick(m_global_time, m_note_arr, m_event_arr);
		m_gen.textParser(m_note_arr, m_event_arr);
		m_gen.binaryParser(m_note_arr, m_event_arr);
		m_gen.bpm(m_global_time, m_bpm_arr);
		m_player.pollState(m_gen);
		m_player.tick(m_global_time);
		m_render.pollState(m_global_time, m_player, m_gen);
		m_audio.buffer();
		if(m_global_time >= 0.0)m_audio.play();

		double nowTime = glfwGetTime();
		m_global_time += nowTime - m_pastTime;
		m_pastTime = nowTime;

	}	
}

void Game::input(int key, int action) {
	if(m_scene == 0) {
		m_menu.input(key, action);
		if (action == GLFW_PRESS && key == GLFW_KEY_SPACE) {
			m_scene = 1;
			resetTime();
		}
	}
	else if (m_scene == 1)
	{
		m_player.keyCallback(key, action, m_global_time, m_note_arr, m_event_arr);
		if (action == GLFW_PRESS && key == GLFW_KEY_T)std::cout << m_global_time << std::endl;
		if (action == GLFW_PRESS && key == GLFW_KEY_P)m_scene = 0;
	}
}

void Game::resetTime() {
	std::cout << "timer reset" << std::endl;
	glfwSetTime(0.0);
	m_pastTime = glfwGetTime();
	m_global_time = -2.0f;
}

Game::~Game() {
	m_note_arr.clear();
	m_event_arr.clear();
}

