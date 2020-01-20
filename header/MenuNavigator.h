#pragma once
#include "MenuRender.h"
#include "MenuNode.h"
#include "SongScanner.h"
#include "GLFW/include/GLFW/glfw3.h"

#include <iostream>
#include <fstream>

enum scenes {
	MAIN_SCENE,REMAPPING,CREDITS,SCRATCHES,CALIBRATION
};

class MenuNavigator {
public:
	MenuNavigator();
	void init(GLFWwindow* w,Game* gameptr);
	void pollInput();
	void update();
	void render(double dt);
	void setActive(bool active);
	bool getActive();
	void activate(MenuNode& menu, MenuNode& parent);
	void scan();
	bool getShouldClose();
	~MenuNavigator();

	//switch between keyboard and gamepad
	bool m_useKeyboardInput = true;

	//default key codes for keyboard
	int UP_CODE = GLFW_KEY_UP;
	int DOWN_CODE = GLFW_KEY_DOWN;
	int SELECT_CODE = GLFW_KEY_ENTER;
	int BACK_CODE = GLFW_KEY_ESCAPE;

	//default gamepad bindings for xbox 360 turntable
	int UP_GAMEPAD = 10;
	int DOWN_GAMEPAD = 12;
	int SELECT_GAMEPAD = GLFW_GAMEPAD_BUTTON_A;
	int BACK_GAMEPAD = GLFW_GAMEPAD_BUTTON_B;

	int m_scene = 0;

	//static int testx;
private:
	void remap();
	void updateGamepadState();
	void updateMenuNode();
	void resetMenu();
	void writeConfigFile();
	void readConfigFile();

	//gamepad status
	std::vector<float> m_gpState;
	std::vector<float> m_gpDead;

	//variables used to detect press event
	bool m_isUpPressed = false;
	bool m_isDownPressed = false;
	bool m_isSelectPressed = false;
	bool m_isBackPressed = false;

	bool m_wasUpPressed = false;
	bool m_wasDownPressed = false;
	bool m_wasSelectPressed = false;
	bool m_wasBackPressed = false;

	bool m_isEscapePressed = false;
	bool m_wasEscapePressed = false;
	bool m_wasTabPressed = false;
	bool m_isTabPressed = false;

	unsigned int m_viewOffset = 0;
	bool m_shouldClose = false;
	bool m_active = false;

	//menu structure
	MenuNode m_root = MenuNode("Main Menu",0);
	MenuNode m_activeNode = m_root;
	std::vector<unsigned int> m_selection;
	
	MenuRender m_render;

	std::vector<SongEntry> m_songList = {};
	Game* m_game = nullptr;

	std::vector<float> m_pastGamepadValues;
	std::vector<char> m_keyboardState;
};