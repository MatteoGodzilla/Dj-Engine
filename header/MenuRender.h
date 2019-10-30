#pragma once
#include "Rendr.h"

#include "Graphics.h"
#include "MenuNode.h"
#include "SongScanner.h"

#include "GLFW/glfw3.h"
#include <iostream>
#include <map>

class MenuRender : public Rendr{
public:
	MenuRender();
	void init(GLFWwindow* w);
	void render(MenuNode node,int selected);
	void list(std::vector<SongEntry>& list, MenuNode& node,int offset);
	~MenuRender();
private:
};