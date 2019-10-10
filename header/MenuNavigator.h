#pragma once
#include "MenuRender.h"
#include "MenuNode.h"
#include "GLFW/glfw3.h"
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
	void init(GLFWwindow* w);
	void input(int key, int action);
	void render();
	void setActive(bool active);
	bool getActive();
	void activate(MenuNode& menu);
	~MenuNavigator();
private:
	bool m_active = false;
	MenuNode m_root = MenuNode("Main Menu",0);
	MenuNode m_activeNode = m_root;
	MenuRender m_render;
	std::vector<int> m_selection;


};