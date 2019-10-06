#include "MenuNavigator.h"

MenuNavigator::MenuNavigator()
{
	MenuNode play("play!");
	MenuNode options("options");

	MenuNode op1("Option 1");
	MenuNode op2("Option 2");
	MenuNode op3("Option 3");

	options.push(op1);
	options.push(op2);
	options.push(op3);

	m_root.push(play);
	m_root.push(options);

	m_selection.push_back(0);

	
}

void MenuNavigator::init(GLFWwindow* w) {
	m_render.init(w);
}

void MenuNavigator::input(int key, int action)
{
	if (m_active) {
	MenuNode activeNode = m_root;
	for (size_t i = 0; i < m_selection.size() - 1; i++) {
		activeNode = activeNode.getChildrens().at(m_selection.at(i));
	}
	
	if (action == GLFW_PRESS) {
		if (key == UP_CODE) {
			m_selection.back()--;
			if (m_selection.back() < 0) {
				m_selection.back() = activeNode.getChildCount() - 1;
			}
		}
		else if (key == DOWN_CODE) {
			m_selection.back()++;
			if (m_selection.back() > activeNode.getChildCount()-1) {
				m_selection.back() = 0;
			}
		}
		else if (key == SELECT_CODE) {
			if (activeNode.getChildCount() > 0)
			{
				activeNode.getChildrens().at(m_selection.back()).activate();
				m_selection.push_back(0);
			}
			else {
				std::cout << "Reached End of Tree" << std::endl;
			}
		}
		else if (key == BACK_CODE) {
			if(m_selection.size() > 1) m_selection.pop_back();
		}
	}

	
	else if (action == GLFW_RELEASE) {
		if (key == UP_CODE) {

		}
		else if (key == DOWN_CODE) {

		}
		else if (key == SELECT_CODE) {

		}
		else if (key == BACK_CODE) {

		}
		std::cout << "Selection vector size:" << m_selection.size() << std::endl;
		std::cout << "Active Node size:" << activeNode.getChildCount() << std::endl;
		std::cout << "Selection:" << m_selection.at(m_selection.size() - 1) << std::endl << std::endl;
	}
	}
	
}

void MenuNavigator::render() {
	if (m_active) {
		m_render.render();
	}
}

void MenuNavigator::setActive(bool active)
{
	m_active = active;
}

MenuNavigator::~MenuNavigator()
{
}
