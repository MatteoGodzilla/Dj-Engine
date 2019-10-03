#pragma once
#include "GLFW/glfw3.h"
#include "MenuNode.h"
#include <iostream>

enum Keys {
	UP_CODE = GLFW_KEY_W,
	DOWN_CODE = GLFW_KEY_S,
	SELECT_CODE = GLFW_KEY_ENTER,
	BACK_CODE = GLFW_KEY_BACKSPACE,
};

class MenuNavigator {
public:
	MenuNavigator();
	void input(int key, int action);
	~MenuNavigator();
private:
	MenuNode m_root = MenuNode("Root");
	std::vector<int> m_selection;
};