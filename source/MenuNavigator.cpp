#include "MenuNavigator.h"

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

void MenuNavigator::init(GLFWwindow* w) {
	m_render.init(w);
	render();
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
					activate(activeNode.getChildrens().at(m_selection.back()));
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

void MenuNavigator::setActive(bool active) {
	m_active = active;
}

bool MenuNavigator::getActive() {
	return m_active;
}

void MenuNavigator::activate(MenuNode& menu) {
	//every case represents a function called on activate
	switch (menu.getId()) {
	case 1:
		m_active = false;
		break;
	default:
		std::cout << "MenuNavigator: no function attached to id " << menu.getId() << std::endl;
		break;
	}
}

MenuNavigator::~MenuNavigator()
{
}
