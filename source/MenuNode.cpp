#include "MenuNode.h"

MenuNode::MenuNode(const char* text)
{
	m_text = std::string(text);
}

void MenuNode::link(void(*callback)())
{
	m_function = callback;
}

void MenuNode::activate()
{
	if(m_function != nullptr)m_function();
}

void MenuNode::push(MenuNode n)
{
	m_childrens.push_back(n);
}

std::string MenuNode::getText()
{
	return m_text;
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
