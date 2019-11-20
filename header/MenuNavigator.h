#pragma once
#include "MenuRender.h"
#include "MenuNode.h"
#include "SongScanner.h"
#include "Game.h"
#include "GLFW/glfw3.h"
#include <iostream>

class MenuNavigator {
public:
	MenuNavigator();
	void init(GLFWwindow* w,Game* gameptr);
	void pollInput();
	void update();
	void render();
	void setActive(bool active);
	bool getActive();
	void activate(MenuNode& menu, MenuNode& parent);
	void scan();
	bool getShouldClose();
	~MenuNavigator();

	bool m_useKeyboardInput = true;

	int UP_CODE = GLFW_KEY_W;
	int DOWN_CODE = GLFW_KEY_S;
	int SELECT_CODE = GLFW_KEY_J;
	int BACK_CODE = GLFW_KEY_K;

	int UP_GAMEPAD = 10;
	int DOWN_GAMEPAD = 12;
	int SELECT_GAMEPAD = GLFW_GAMEPAD_BUTTON_A;
	int BACK_GAMEPAD = GLFW_GAMEPAD_BUTTON_B;

	bool m_isUpPressed = false;
	bool m_isDownPressed = false;
	bool m_isSelectPressed = false;
	bool m_isBackPressed = false;

	bool m_wasUpPressed = false;
	bool m_wasDownPressed = false;
	bool m_wasSelectPressed = false;
	bool m_wasBackPressed = false;

private:
	void updateMenuNode();
	void resetMenu();

	int m_scene = 0;
	unsigned int m_viewOffset = 0;
	bool m_shouldClose = false;

	bool m_active = false;
	MenuNode m_root = MenuNode("Main Menu",0);
	MenuNode m_activeNode = m_root;
	MenuRender m_render;
	std::vector<unsigned int> m_selection;

	std::vector<SongEntry> m_songList = {};
	Game* m_game = nullptr;
};