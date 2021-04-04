// SPDX-FileCopyrightText: 2021 MatteoGodzilla
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "MenuNode.h"

MenuNode::MenuNode(const std::string& text, int id) {
	m_text = text;
	m_id = id;
}

MenuNode::MenuNode(const MenuNode& other) {
	m_text = other.m_text;
	m_id = other.m_id;
	m_childrens = other.m_childrens;
}

//push children in current node
void MenuNode::push(const MenuNode& n) {
	m_childrens.push_back(n);
}

//utility functions to return variables
std::string MenuNode::getText() {
	return m_text;
}

void MenuNode::setText(const std::string& text) {
	m_text = text;
}

int MenuNode::getId() const {
	return m_id;
}

std::vector<MenuNode>& MenuNode::getChildrens() {
	return m_childrens;
}

size_t MenuNode::getChildCount() {
	return m_childrens.size();
}

void MenuNode::updateChildrens(const std::vector<MenuNode>& list) {
	m_childrens = list;
}

MenuNode::~MenuNode() {
	m_childrens.erase(m_childrens.begin(), m_childrens.end());
}
