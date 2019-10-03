#pragma once
#include <vector> 
#include <string>

class MenuNode
{
public:
	MenuNode(const char* text);
	void link(void(*callback)());
	void activate();
	void push(MenuNode n);
	std::vector<MenuNode> getChildrens();
	size_t getChildCount();
	std::string getText();
	~MenuNode();
private:
	std::string m_text = "";
	void(*m_function)() = nullptr;
	std::vector<MenuNode> m_childrens;
};

