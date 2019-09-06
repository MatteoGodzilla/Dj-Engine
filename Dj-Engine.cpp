#include <iostream>
#include <vector>

#include "header/Note.h"
#include "header/Rendr.h"
#include "header/Generator.h"
#include "header/Player.h"

#include "GLFW/glfw3.h"

unsigned int WIDTH = 1024;
unsigned int HEIGHT = 600;

double global_time;
GLFWwindow* window;

std::vector<Note> note_arr;
std::vector<Note> event_arr;

Rendr rendr;

Generator gen;
Player player;

void check_events(GLFWwindow* w,int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)glfwSetWindowShouldClose(window,true);
	player.keyCallback(key,action,global_time,note_arr,event_arr);
}

void render() {
	rendr.highway(global_time);
	rendr.clicker();
	rendr.lanes(global_time, event_arr);
	rendr.events(global_time, event_arr);
	rendr.notes(global_time, note_arr);
}

void tick() {
	gen.tick(global_time, note_arr, event_arr);
	player.pollState(gen);
	rendr.pollState(global_time, player, gen);
}

int main() {
	if (!glfwInit()) {
		std::cout << "GLFW INIT ERROR" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(WIDTH, HEIGHT, "Dj-Engine", nullptr, nullptr);
	glfwSetKeyCallback(window, check_events);

	rendr.init(window);
	if (!window) {
		std::cout << "GLFW WINDOW CREATION ERROR" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		global_time = glfwGetTime();
		glfwPollEvents();
		tick();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		render();
		glfwSwapBuffers(window);

	}
	note_arr.clear();
	event_arr.clear();
	glfwTerminate();
}