#pragma once
#include <vector> 
#include <string>

class MenuNode
{
public:
	MenuNode(const char* text,int id);
	void push(MenuNode n);
	std::vector<MenuNode> getChildrens();
	size_t getChildCount();
	std::string getText();
	int getId();
	~MenuNode();
private:
	std::string m_text = "";
	int m_id = 0;
	std::vector<MenuNode> m_childrens;
};

