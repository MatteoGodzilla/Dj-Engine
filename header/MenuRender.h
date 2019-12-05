#pragma once
#include "Rendr.h"

#include "Graphics.h"
#include "MenuNode.h"
#include "SongScanner.h"

#include "Game.h"

#include "GLFW/glfw3.h"
#include <iostream>
#include <map>

class MenuRender : public Rendr {
public:
	MenuRender();
	void init(GLFWwindow* w);
	void render(MenuNode node, int selected, unsigned int vOffset);
	void remapping(Game* game);
	GLFWwindow* getWindowPtr();
	void doneEditing();
	void credits();

	const size_t VISIBLE_ENTRIES = 4;
	bool m_editingAxis = false;
	bool m_editingKey = false;
	int m_actionToChange = -1;

	bool m_shouldClose = false;
	~MenuRender();
private:
	void editingAxis(int axis);
	void editingKey(int axis);
};