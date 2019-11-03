#include "MenuNavigator.h"

size_t findIndex(MenuNode& element, MenuNode& parent) {
	std::vector<MenuNode>list = parent.getChildrens();
	for (size_t i = 0; i < list.size(); i++) {
		if (list.at(i).getText() == element.getText()) return i;
	}
}

MenuNavigator::MenuNavigator(){
	//create menu tree
	MenuNode play("play!",1);
	MenuNode options("options",2);

	MenuNode op1("Option 1",3);
	MenuNode op2("Option 2",4);
	MenuNode op3("Option 3",5);

	options.push(op1);
	options.push(op2);
	options.push(op3);

	m_root.push(play);
	m_root.push(options);

	m_selection.push_back(0);
	m_activeNode = m_root;
}

void MenuNavigator::init(GLFWwindow* w,Game* gameptr) {
	m_game = gameptr;
	m_render.init(w);
	render();
	scan();
}

void MenuNavigator::input(int key, int action) {
	if (m_active) {
		
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

		if (action == GLFW_PRESS)
		{
			//go up a node
			if (key == UP_CODE) {
				m_selection.back()--;
				if (m_selection.back() < 0) {
					m_selection.back() = activeNode.getChildCount() - 1;
				}
			}
			//go down a node
			else if (key == DOWN_CODE) {
				m_selection.back()++;
				if (m_selection.back() > activeNode.getChildCount() - 1) {
					m_selection.back() = 0;
				}
			}
			//activate selected node
			else if (key == SELECT_CODE) {
				if (activeNode.getChildCount() > 0)
				{
					//do something based on the node id
					activate(activeNode.getChildrens().at(m_selection.back()),activeNode);
					m_selection.push_back(0);
				}
				else {
					std::cout << "Reached End of Tree" << std::endl;
				}
			}
			//go back
			else if (key == BACK_CODE) {
				if (m_selection.size() > 1) m_selection.pop_back();
			}
		}

		else if (action == GLFW_RELEASE)
		{
			if (key == UP_CODE) {

			}
			else if (key == DOWN_CODE) {

			}
			else if (key == SELECT_CODE) {

			}
			else if (key == BACK_CODE) {

			}
		}
		m_activeNode = activeNode;
	}
}

void MenuNavigator::render() {
	if (m_active) {
		m_render.render(m_activeNode,m_selection.back());
	}
}

void MenuNavigator::activate(MenuNode& menu,MenuNode& parent) {
	//every case represents a function called on activate
	size_t index = 0;
	switch (menu.getId()) {
	case 255:
		index = findIndex(menu, parent);
		m_active = false;
		m_game->init(m_render.getWindowPtr(),m_songList.at(index).path);
		m_game->start();
		m_selection.erase(m_selection.begin(), m_selection.end());
		break;
	default:
		std::cout << "MenuNavigator: no function attached to id " << menu.getId() << std::endl;
		break;
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


void MenuNavigator::setActive(bool active) {
	m_active = active;
}

bool MenuNavigator::getActive() {
	return m_active;
}

MenuNavigator::~MenuNavigator()
{
}