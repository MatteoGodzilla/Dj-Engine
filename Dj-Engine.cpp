#include <iostream>
#include <vector>

#include "Game.h"
#include "MenuNavigator.h"

#include "GLFW/include/GLFW/glfw3.h"

unsigned int WIDTH = 1280;
unsigned int HEIGHT = 720;

Game game;
MenuNavigator menu;

//decides if rendering game or menu
int scene = 0;

GLFWwindow* window;

double pastTick = 0.0;
double now = 0.0f;
double deltaTime = 0.0f;

//utility function to handle resizing
void resizeCallback(GLFWwindow* w,int width,int height) {
	glViewport(0, 0, width, height);
}

void scrollCallback(GLFWwindow* w, double xoff, double yoff) {
	game.getPlayer()->m_scrollX = xoff;
	game.getPlayer()->m_scrollY = yoff;
	game.getPlayer()->m_changedScroll = true;
}

int main() {
	std::cout << "Dj-Engine version alpha 1.2" << std::endl;
	if (glfwInit() == GLFW_FALSE) {
		const char* description;
		int errorCode = glfwGetError(&description);
		std::cout << "Engine Error:GLFW INIT ERROR" << std::endl;
		std::cout << "Error code:" << errorCode << std::endl;
		std::cout << "Error Message:" << description << std::endl;
		return -1;
	}
	std::cout << "Engine Message: GLFW INIT SUCCESS" << std::endl;

	//GLFW init functions (window and callbacks)
	window = glfwCreateWindow(WIDTH, HEIGHT, "Dj-Engine", nullptr, nullptr);
	if (!window) {
		std::cout << "Engine Error:GLFW WINDOW CREATION ERROR" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	glfwSetScrollCallback(window, scrollCallback);
	glfwMakeContextCurrent(window);

	//imgui init
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	//setting up menu and game
	game.init(window);
	menu.init(window,&game);
	game.setActive(false);
	menu.setActive(true);

	while (!glfwWindowShouldClose(window)) {
		
		glfwPollEvents();
		
		now = glfwGetTime();
		deltaTime = now - pastTick;
		pastTick = now;

		if (scene == 0) {
			//change scene if not active
			if (!menu.getActive()) {
				scene = 1;
				game.setActive(true);
			}

			//render menu
			glClearColor(0.13f, 0.21f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			menu.pollInput();
			menu.update();
			menu.render(deltaTime);
		}
		else if (scene == 1) {
			//change scene if not active
			if (!game.getActive()) {
				scene = 0;
				menu.setActive(true);
			}
			//render/update game
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			game.pollInput();
			game.tick(deltaTime);
			game.render();
		}

		//add delta time to m_global_time
		
		
		glfwSwapBuffers(window);
		if (menu.getShouldClose()) {
			glfwSetWindowShouldClose(window, true);
		}
	}
	//imgui shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}
/*
^ ^ V V < > < > B A Start

Oh no you found the hidden konami code
Please keep this hidden from others.
Do not share
*/