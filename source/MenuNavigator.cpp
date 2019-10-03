#include "MenuNavigator.h"

void playActivate() {
	std::cout << "Play Activated" << std::endl;
}

void op1Activate()
{
	std::cout << "Sub-Option 1 Function Activate" << std::endl;
}

MenuNavigator::MenuNavigator()
{
	MenuNode play("play!");
	MenuNode options("options");

	MenuNode op1("Option 1");
	MenuNode op2("Option 2");

	play.link(playActivate);

	options.push(op1);
	options.push(op2);

	m_root.push(play);
	m_root.push(options);

	m_selection.push_back(0);
}

void MenuNavigator::input(int key, int action)
{
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
			//activeNode.getChildrens().back().activate();
			activeNode.getChildrens().at(m_selection.back()).activate();
			m_selection.push_back(0);
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

MenuNavigator::~MenuNavigator()
{
}
