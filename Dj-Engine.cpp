#include <iostream>
#include <vector>

#include "Game.h"
#include "MenuNavigator.h"

#include "GLFW/glfw3.h"


unsigned int WIDTH = 1024;
unsigned int HEIGHT = 600;

Game game1;
MenuNavigator menu;

int scene = 0;

GLFWwindow* window;

void check_events(GLFWwindow* w,int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)glfwSetWindowShouldClose(window, true);
	if (scene == 0)menu.input(key, action);
	else game1.input(key, action);
}

void resizeCallback(GLFWwindow* w,int width,int height) {
	glViewport(0, 0, width, height);
}

int main() {
	if (!glfwInit()) {
		std::cout << "GLFW INIT ERROR" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(WIDTH, HEIGHT, "Dj-Engine", nullptr, nullptr);
	glfwSetKeyCallback(window, check_events);
	glfwSetWindowSizeCallback(window, resizeCallback);

	game1.init(window);
	game1.setActive(false);
	menu.init(window);
	menu.setActive(true);

	if (!window) {
		std::cout << "GLFW WINDOW CREATION ERROR" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		
		glfwPollEvents();
		
		if (!menu.getActive()) {
			scene = 1;
			game1.start();
		}
		else {
			scene = 0;
			game1.setActive(false);
		}

		if (scene == 0) {
			glClearColor(0.13f, 0.21f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			menu.render();
		}
		else {
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			game1.tick();
			game1.render();
		}
		

		
		glfwSwapBuffers(window);

	}
	
	glfwTerminate();
}