#include <iostream>
#include <vector>

#include "Game.h"

#include "GLFW/glfw3.h"

unsigned int WIDTH = 1024;
unsigned int HEIGHT = 600;

Game game1;

GLFWwindow* window;

void check_events(GLFWwindow* w,int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)glfwSetWindowShouldClose(window,true);
	game1.input(key, action);
}

int main() {
	if (!glfwInit()) {
		std::cout << "GLFW INIT ERROR" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(WIDTH, HEIGHT, "Dj-Engine", nullptr, nullptr);
	glfwSetKeyCallback(window, check_events);

	game1.init(window);
	if (!window) {
		std::cout << "GLFW WINDOW CREATION ERROR" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		
		glfwPollEvents();
		game1.tick();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		game1.render();
		glfwSwapBuffers(window);

	}
	
	glfwTerminate();
}