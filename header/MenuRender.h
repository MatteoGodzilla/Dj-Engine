#pragma once
#include "Rendr.h"

#include "Graphics.h"
#include "MenuNode.h"
#include "SongScanner.h"

#include "Game.h"

#include "GLFW/include/GLFW/glfw3.h"
#include <iostream>
#include <map>

class MenuRender : public Rendr {
public:
	MenuRender();
	void init(GLFWwindow* w);
	void render(MenuNode node, int selected, unsigned int vOffset);
	void remapping(Game* game, int uk, int dk, int sk, int bk, int ug, int dg, int sg, int bg);
	void scratches(Player* player);
	void splashArt();
	GLFWwindow* getWindowPtr();
	void doneEditing();
	void credits();

	const size_t VISIBLE_ENTRIES = 4;
	bool m_editingAxis = false;
	bool m_editingKey = false;
	int m_gameActionToChange = -1;
	int m_menuActionToChange = -1;

	std::string m_testBuffer = "";

	bool m_shouldClose = false;
	bool m_input = true;
	~MenuRender();
private:
	std::string m_inputSelection = "Keyboard";
	unsigned int m_buttonTexture = 0;
	unsigned int m_splashTexture = 0;

	void editingGameAxis(int axis);
	void editingGameKey(int axis);

	

	void editingMenuAxis(int axis);
	void editingMenuKey(int axis);
};