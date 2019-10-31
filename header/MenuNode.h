#pragma once
#include <vector> 
#include <string>

class MenuNode
{
public:
	MenuNode(std::string text,int id);
	void push(MenuNode& n);
	std::vector<MenuNode> getChildrens();
	size_t getChildCount();
	void updateChildrens(std::vector<MenuNode>list);
	std::string getText();
	int getId();
	~MenuNode();
private:
	std::string m_text = "";
	int m_id = 0;
	std::vector<MenuNode> m_childrens;
};

