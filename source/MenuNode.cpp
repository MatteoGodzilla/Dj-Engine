#include "MenuNode.h"

MenuNode::MenuNode(std::string text, int id) {
	m_text = std::move(text);
	m_id = id;
}

//push children in current node
void MenuNode::push(MenuNode& n) {
	m_childrens.push_back(n);
}

//utility functions to return variables
std::string MenuNode::getText() {
	return m_text;
}

void MenuNode::setText(const std::string& text) {
	m_text = text;
}

int MenuNode::getId() {
	return m_id;
}

std::vector<MenuNode>& MenuNode::getChildrens() {
	return m_childrens;
}

size_t MenuNode::getChildCount() {
	return m_childrens.size();
}

void MenuNode::updateChildrens(std::vector<MenuNode> list) {
	m_childrens = std::move(list);
}

MenuNode::~MenuNode() {
	m_childrens.erase(m_childrens.begin(), m_childrens.end());
}
