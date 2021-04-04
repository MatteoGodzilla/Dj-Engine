// SPDX-FileCopyrightText: 2021 MatteoGodzilla
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include <string>
#include <utility>
#include <vector>

class MenuNode {
public:
	MenuNode(const std::string& text, int id);
	MenuNode(const MenuNode& other);
	void push(const MenuNode& n);
	std::vector<MenuNode>& getChildrens();
	size_t getChildCount();
	void updateChildrens(const std::vector<MenuNode>& list);
	std::string getText();
	void setText(const std::string& text);
	int getId() const;
	~MenuNode();

private:
	std::string m_text = "";
	int m_id = 0;
	std::vector<MenuNode> m_childrens;
};
