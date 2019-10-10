#include "MenuNode.h"

MenuNode::MenuNode(const char* text,int id)
{
	m_text = std::string(text);
	m_id = id;
}

void MenuNode::push(MenuNode n)
{
	m_childrens.push_back(n);
}

std::string MenuNode::getText()
{
	return m_text;
}

int MenuNode::getId()
{
	return m_id;
}

std::vector<MenuNode> MenuNode::getChildrens() {
	return m_childrens;
}

size_t MenuNode::getChildCount() {
	return m_childrens.size();
}

MenuNode::~MenuNode()
{
	m_childrens.erase(m_childrens.begin(), m_childrens.end());
}
