// SPDX-FileCopyrightText: 2021 MatteoGodzilla
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "DJEUtils.h"
#include "Game.h"
#include "MenuNavigator.h"
#include "stb_image.h"

#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

int WIDTH = 1280;
int HEIGHT = 720;

Game game;
MenuNavigator menu;

//decides if rendering game or menu
int scene = 0;

GLFWwindow* window;

double pastTick = 0.0;
double now = 0.0f;
double deltaTime = 0.0f;

//utility function to handle resizing
void resizeCallback(GLFWwindow* w, int width, int height) {
	glViewport(0, 0, width, height);
}

void scrollCallback(GLFWwindow* w, double xoff, double yoff) {
	game.getPlayer()->m_scrollX = xoff;
	game.getPlayer()->m_scrollY = yoff;
	game.getPlayer()->m_changedScroll = true;
}

void errorCallback(int code, const char* description) {
	ErrorLog << "GLFW ERROR:(" << code << ")," << description << ENDL;
}

int main(int argc, char** argv) {
	NormalLog << "Dj-Engine " << VERSION << ENDL;
	if (glfwInit() == GLFW_FALSE) {
		const char* description;
		int errorCode = glfwGetError(&description);
		NormalLog << "Engine Error:GLFW INIT ERROR" << ENDL;
		NormalLog << "Error code:" << errorCode << ENDL;
		NormalLog << "Error Message:" << description << ENDL;
		return -1;
	}
	NormalLog << "Engine Message: GLFW INIT SUCCESS" << ENDL;

	bool MSAActive = false;
	if (argc > 1) {
		for (int i = 0; i < argc - 1; ++i) {
			if (strcmp(argv[i], "--msaa") == 0) {
				int factor = std::stoi(argv[i + 1]);
				NormalLog << "Engine Message: Creating window with MSAA x" << factor << ENDL;
				glfwWindowHint(GLFW_SAMPLES, factor);
				MSAActive = true;
			}
		}
	}

	//GLFW init functions (window and callbacks)
	std::string title = std::string("Dj-Engine ") + VERSION;
	window = glfwCreateWindow(WIDTH, HEIGHT, title.c_str(), nullptr, nullptr);
	if (!window) {
		NormalLog << "Engine Error:GLFW WINDOW CREATION ERROR" << ENDL;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowSizeCallback(window, resizeCallback);
	glfwSetWindowAspectRatio(window, WIDTH, HEIGHT);
	glfwSetErrorCallback(&errorCallback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}
	glfwSetScrollCallback(window, scrollCallback);
	glfwMakeContextCurrent(window);

	int w;
	int h;
	int n;
	unsigned char* data = stbi_load("res/GameIcon.png", &w, &h, &n, 0);

	GLFWimage image = {w, h, data};
	glfwSetWindowIcon(window, 1, &image);

	if (MSAActive) {
		glEnable(GL_MULTISAMPLE);
	}

	//imgui init
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	//setting up menu and game
	game.init(window);
	menu.init(window, &game);
	game.setActive(false);
	menu.setActive(true);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

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
			menu.render();
		} else if (scene == 1) {
			//change scene if not active
			if (!game.getActive()) {
				scene = 0;
				menu.setActive(true);
			}
			//render/update game
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			game.tick();
			game.render();
		}
		glfwSwapBuffers(window);
		if (menu.getShouldClose()) {
			glfwSetWindowShouldClose(window, true);
		}
	}
	//imgui shutdown
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	game.stopThread();

	glfwTerminate();
}
/*
^ ^ V V < > < > B A Start

Oh no you found the hidden konami code
Please keep this hidden from others.
Do not share
*/
