#include "Game.h"

Game::Game() {
}

void Game::init(GLFWwindow* w) {
	m_render.init(w);
	std::cout << "Please press space to start game" << std::endl;
}

void Game::render() {
	if (active) {
		m_render.highway(m_global_time);
		m_render.events(m_global_time, m_event_arr);
		m_render.clicker();
		m_render.meters();
		m_render.lanes(m_global_time, m_event_arr);
		m_render.notes(m_global_time, m_note_arr);
	}
	else {
		m_render.splash();
	}
}

void Game::tick() {
	if (active) {
		
		m_gen.tick(m_global_time, m_note_arr, m_event_arr);
		m_gen.gen(m_note_arr, m_event_arr);
		m_player.pollState(m_gen);
		m_render.pollState(m_global_time, m_player, m_gen);
		m_audio.play();

		double nowTime = glfwGetTime();
		m_global_time += nowTime - m_pastTime;
		m_pastTime = nowTime;

	}	
}

void Game::input(int key, int action) {
	if (active)
	{
		m_player.keyCallback(key, action, m_global_time, m_note_arr, m_event_arr);
		if (action == GLFW_PRESS && key == GLFW_KEY_T)std::cout << m_global_time << std::endl;
	}
	else {
		if (action == GLFW_PRESS && key == GLFW_KEY_SPACE) {
			active = true;
			std::cout << "timer reset" << std::endl;
			glfwSetTime(0.0);
			m_pastTime = glfwGetTime();
		}
	}
}

Game::~Game() {
	m_note_arr.clear();
	m_event_arr.clear();
}

