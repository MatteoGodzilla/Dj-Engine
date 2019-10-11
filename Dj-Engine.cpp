#include <iostream>
#include <vector>

#include "Game.h"
#include "MenuNavigator.h"

#include "GLFW/glfw3.h"

unsigned int WIDTH = 1024;
unsigned int HEIGHT = 600;

Game game;
MenuNavigator menu;

//decides if rendering game or menu
int scene = 0;

GLFWwindow* window;

//callback function for keyboard input
void check_events(GLFWwindow* w,int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)glfwSetWindowShouldClose(window, true);
	if (scene == 0)menu.input(key, action);
	else game.input(key, action);
}

//utility function to handle resizing
void resizeCallback(GLFWwindow* w,int width,int height) {
	glViewport(0, 0, width, height);
}

int main() {
	if (!glfwInit()) {
		std::cout << "GLFW INIT ERROR" << std::endl;
		return -1;
	}

	//GLFW init functions (window and callbacks)
	window = glfwCreateWindow(WIDTH, HEIGHT, "Dj-Engine", nullptr, nullptr);
	glfwSetKeyCallback(window, check_events);
	glfwSetWindowSizeCallback(window, resizeCallback);

	//initialize menu and game
	menu.init(window);
	menu.setActive(true);
	game.init(window);
	game.setActive(false);

	if (!window) {
		std::cout << "GLFW WINDOW CREATION ERROR" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		
		glfwPollEvents();
		
		//changes scene if menu gets deactivated
		if (!menu.getActive()) {
			scene = 1;
			game.start();
		}
		else {
			scene = 0;
			game.setActive(false);
		}


		if (scene == 0) {
			//render menu
			glClearColor(0.13f, 0.21f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			menu.render();
		}
		else {
			//render game
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			game.tick();
			game.render();
		}
		
		glfwSwapBuffers(window);
	}
	glfwTerminate();
}