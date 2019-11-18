#include "MenuNavigator.h"

size_t findIndex(MenuNode& element, MenuNode& parent) {
	std::vector<MenuNode>list = parent.getChildrens();
	for (size_t i = 0; i < list.size(); i++) {
		if (list.at(i).getText() == element.getText()) return i;
	}
	return 0xffffffff;
}

MenuNavigator::MenuNavigator(){
	//create menu tree
	MenuNode play("Play!",1);
	MenuNode options("Options",2);
	MenuNode credits("Credits", 3);
	MenuNode exit("Exit", -1);

	for (int i = 0; i < 10; i++) {
		std::string text("option");
		text += std::to_string(i);
		MenuNode t(text, 10 + i);
		options.push(t);
	}

	m_root.push(play);
	m_root.push(options);
	m_root.push(credits);
	m_root.push(exit);

	m_selection.push_back(0);
	m_activeNode = m_root;
}

void MenuNavigator::init(GLFWwindow* w,Game* gameptr) {
	m_game = gameptr;
	m_render.init(w);
	render();
	scan();
}

void MenuNavigator::pollInput(){
	m_wasUpPressed = m_isUpPressed;
	m_wasDownPressed = m_isDownPressed;
	m_wasSelectPressed = m_isSelectPressed;
	m_wasBackPressed = m_isBackPressed;

	m_isUpPressed= glfwGetKey(m_render.getWindowPtr(), UP_CODE);
	m_isDownPressed = glfwGetKey(m_render.getWindowPtr(), DOWN_CODE);
	m_isSelectPressed = glfwGetKey(m_render.getWindowPtr(), SELECT_CODE);
	m_isBackPressed = glfwGetKey(m_render.getWindowPtr(), BACK_CODE);
}

void MenuNavigator::update() {
	if (m_active) {
		if (m_scene == 0) {
			/*
			activeNode is the selected node
			m_selection contains all selected node indices
			the last item in m_selection is the 'cursor position' moved by player
			*/

			MenuNode activeNode = m_root;
			for (size_t i = 0; i < m_selection.size() - 1; i++) {
				if (activeNode.getChildCount() > 0) {
					activeNode = activeNode.getChildrens().at(m_selection.at(i));
				}
			}

			if (m_isUpPressed && !m_wasUpPressed) {
				if (m_selection.back() > 0) {
					m_selection.back()--;
				}
				else if (m_selection.back() == 0) {
					m_selection.back() = activeNode.getChildCount() - 1;
					if (activeNode.getChildCount() > m_render.VISIBLE_ENTRIES) {
						m_viewOffset = activeNode.getChildCount() - m_render.VISIBLE_ENTRIES;
					}
				}
				if (activeNode.getChildCount() > m_render.VISIBLE_ENTRIES) {
					if (m_selection.back() < m_viewOffset) {
						m_viewOffset--;
					}
				}
			}
			if (m_isDownPressed && !m_wasDownPressed) {
				if (m_selection.back() < activeNode.getChildCount() + 1) {
					m_selection.back()++;
				}
				if (m_selection.back() == activeNode.getChildCount()) {
					m_selection.back() = 0;
					if (activeNode.getChildCount() > m_render.VISIBLE_ENTRIES) {
						m_viewOffset = 0;
					}
				}
				if (activeNode.getChildCount() > m_render.VISIBLE_ENTRIES) {
					if (m_selection.back() > m_viewOffset + m_render.VISIBLE_ENTRIES - 1) {
						m_viewOffset++;
					}
				}
			}
			if (m_isSelectPressed && !m_wasSelectPressed) {
				if (activeNode.getChildCount() > 0)
				{
					//do something based on the node id
					activate(activeNode.getChildrens().at(m_selection.back()), activeNode);
					m_selection.push_back(0);
					m_viewOffset = 0;
				}
				else {
					std::cout << "Reached End of Tree" << std::endl;
				}
			}
			if (m_isBackPressed && !m_wasBackPressed) {
				if (m_selection.size() > 1) {
					m_selection.pop_back();
					m_viewOffset = 0;
				}
			}
			m_activeNode = activeNode;
		}
		else if (m_scene == 2) {
			if (m_isSelectPressed && !m_wasSelectPressed) {
				m_scene = 0;
				resetMenu();
			}
		}
	}
}

void MenuNavigator::render() {
	if (m_active) {
		if (m_scene == 0) {
			m_render.render(m_activeNode, m_selection.back(), m_viewOffset);
		}
		else if (m_scene == 2) {
			m_render.credits();
		}
	}
}

void MenuNavigator::activate(MenuNode& menu, MenuNode& parent) {
	//every case represents a function called on activate
	size_t index = 0;
	int id = menu.getId();
	if (id == -1) {
		m_shouldClose = true;
	}
	else if (id == 3) {
		m_scene = 2;
	}
	else if (id == 255) {
		index = findIndex(menu, parent);
		m_active = false;
		m_game->init(m_render.getWindowPtr(), m_songList.at(index).path);
		m_game->start();
		resetMenu();
	}
	else {
		std::cout << "MenuNavigator: no function attached to id " << menu.getId() << std::endl;
	}
}

void MenuNavigator::scan() {
	SongScanner::load("./songs", m_songList);
	for (const SongEntry& entry: m_songList) {
		std::string text;
		if (!entry.s2.empty()) {
			text = entry.s1 + " vs " + entry.s2;
		}
		else {
			text = entry.s1;
		}

		MenuNode song(text, 255);
		std::vector<MenuNode> list = m_root.getChildrens();
		list.at(0).push(song);
		m_root.updateChildrens(list);
	}
}

bool MenuNavigator::getShouldClose()
{
	return m_shouldClose;
}

void MenuNavigator::resetMenu() {
	m_selection.erase(m_selection.begin(), m_selection.end());
	m_selection.push_back(0);
}

void MenuNavigator::setActive(bool active) {
	m_active = active;
}

bool MenuNavigator::getActive() {
	return m_active;
}

MenuNavigator::~MenuNavigator()
{
}

