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

	MenuNode ChangeInput("Change Input", 4);
	options.push(ChangeInput);
	
	m_root.push(play);
	m_root.push(options);
	m_root.push(credits);
	m_root.push(exit);

	m_selection.push_back(0);
	m_activeNode = m_root;

	m_gpDead.clear();
	for (int i = 0; i < 15+6; ++i) {
		m_gpDead.push_back(0.5);
	}
}

void MenuNavigator::init(GLFWwindow* w,Game* gameptr) {
	m_pastGamepadValues = gameptr->getPlayer()->getGamepadValues();
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
	m_wasEscapePressed = m_isEscapePressed;
	m_wasTabPressed = m_isTabPressed;

	if (m_useKeyboardInput) {
		m_isUpPressed = glfwGetKey(m_render.getWindowPtr(), UP_CODE);
		m_isDownPressed = glfwGetKey(m_render.getWindowPtr(), DOWN_CODE);
		m_isSelectPressed = glfwGetKey(m_render.getWindowPtr(), SELECT_CODE);
		m_isBackPressed = glfwGetKey(m_render.getWindowPtr(), BACK_CODE);
	}
	else {
		if (glfwJoystickPresent(GLFW_JOYSTICK_1)) {

			updateGamepadState();

			if (m_gpState.size() > 0) {
				if (m_gpState.at(UP_GAMEPAD) >= m_gpDead.at(UP_GAMEPAD)) {
					m_isUpPressed = true;
				}
				else m_isUpPressed = false;

				if (m_gpState.at(DOWN_GAMEPAD) >= m_gpDead.at(DOWN_GAMEPAD)) {
					m_isDownPressed = true;
				}
				else m_isDownPressed = false;

				if (m_gpState.at(SELECT_GAMEPAD) >= m_gpDead.at(SELECT_GAMEPAD)) {
					m_isSelectPressed = true;
				}
				else m_isSelectPressed = false;

				if (m_gpState.at(BACK_GAMEPAD) >= m_gpDead.at(BACK_GAMEPAD)) {
					m_isBackPressed = true;
				}
				else m_isBackPressed = false;
			}
		}
	}

	m_isEscapePressed = glfwGetKey(m_render.getWindowPtr(), GLFW_KEY_ESCAPE);
	m_isTabPressed = glfwGetKey(m_render.getWindowPtr(), GLFW_KEY_TAB);

	if (glfwGetKey(m_render.getWindowPtr(), GLFW_KEY_SPACE)) {
		remap();
	}
	if (m_wasTabPressed && !m_isTabPressed) {
		if (m_useKeyboardInput) {
			m_game->getPlayer()->m_useKeyboardInput = false;
			std::cout << "Menu Message: Changed Input to Controller" << std::endl;
		}else {
			m_game->getPlayer()->m_useKeyboardInput = true;
			std::cout << "Menu Message: Changed Input to Keyboard" << std::endl;
		}
		m_useKeyboardInput = !m_useKeyboardInput;
	}

	if (m_wasEscapePressed && !m_isEscapePressed && m_activeNode.getId() == m_root.getId()) {
		if (m_scene != 1) {
			m_shouldClose = true;
		}
	}

	if (m_render.m_shouldClose) {
		m_scene = 0;
	}
}

void MenuNavigator::update() {
	if (m_active) {
		if (m_scene == 0) {
			/*
			activeNode is the selected node
			m_selection contains all selected node indices
			the last item in m_selection is the 'cursor position' moved by player
			*/

			updateMenuNode();
			if (m_wasUpPressed && !m_isUpPressed) {
				if (m_selection.back() > 0) {
					m_selection.back()--;
				}
				else if (m_selection.back() == 0) {
					m_selection.back() = m_activeNode.getChildCount() - 1;
					if (m_activeNode.getChildCount() > m_render.VISIBLE_ENTRIES) {
						m_viewOffset = m_activeNode.getChildCount() - m_render.VISIBLE_ENTRIES;
					}
				}
				if (m_activeNode.getChildCount() > m_render.VISIBLE_ENTRIES) {
					if (m_selection.back() < m_viewOffset) {
						m_viewOffset--;
					}
				}
			}
			if (m_wasDownPressed && !m_isDownPressed) {
				if (m_selection.back() < m_activeNode.getChildCount() + 1) {
					m_selection.back()++;
				}
				if (m_selection.back() == m_activeNode.getChildCount()) {
					m_selection.back() = 0;
					if (m_activeNode.getChildCount() > m_render.VISIBLE_ENTRIES) {
						m_viewOffset = 0;
					}
				}
				if (m_activeNode.getChildCount() > m_render.VISIBLE_ENTRIES) {
					if (m_selection.back() > m_viewOffset + m_render.VISIBLE_ENTRIES - 1) {
						m_viewOffset++;
					}
				}
			}
			if (m_wasSelectPressed && !m_isSelectPressed) {
				MenuNode childNode = m_activeNode.getChildrens().at(m_selection.back());
				if (childNode.getChildCount() == 0)
				{
					//do something based on the node id
					activate(childNode, m_activeNode);
				}
				else {
					m_selection.push_back(0);
					m_viewOffset = 0;
				}
			}
			if (m_wasBackPressed && !m_isBackPressed) {
				if (m_selection.size() > 1) {
					m_selection.pop_back();
					m_viewOffset = 0;
				}
			}

			/*
			std::cout << m_activeNode.getText() << ":";
			for (size_t i = 0; i < m_selection.size(); ++i) {
				std::cout << m_selection.at(i) << "|";
			}
			std::cout << std::endl;
			*/
		}
		else if (m_scene == 1) {
			if (m_render.m_editingAxis) {
				int* changing = &(m_game->getPlayer()->GREEN_GAMEPAD);
				if (m_render.m_actionToChange == RED_INDEX) {
					changing = &(m_game->getPlayer()->RED_GAMEPAD);
				}
				else if (m_render.m_actionToChange == BLUE_INDEX) {
					changing = &(m_game->getPlayer()->BLUE_GAMEPAD);
				}
				else if (m_render.m_actionToChange == EU_INDEX) {
					changing = &(m_game->getPlayer()->EU_GAMEPAD);
				}
				else if (m_render.m_actionToChange == CF_LEFT_INDEX) {
					changing = &(m_game->getPlayer()->CF_LEFT_GAMEPAD);
				}
				else if (m_render.m_actionToChange == CF_RIGHT_INDEX) {
					changing = &(m_game->getPlayer()->CF_RIGHT_GAMEPAD);
				}
				else if (m_render.m_actionToChange == SCR_UP_INDEX) {
					changing = &(m_game->getPlayer()->SCR_UP_GAMEPAD);
				}
				else if (m_render.m_actionToChange == SCR_DOWN_INDEX) {
					changing = &(m_game->getPlayer()->SCR_DOWN_GAMEPAD);
				}
				else if (m_render.m_actionToChange == MENU_UP) {
					changing = &UP_GAMEPAD;
				}
				else if (m_render.m_actionToChange == MENU_DOWN) {
					changing = &DOWN_GAMEPAD;
				}
				else if (m_render.m_actionToChange == MENU_SELECT) {
					changing = &SELECT_GAMEPAD;
				}
				else if (m_render.m_actionToChange == MENU_BACK) {
					changing = &BACK_GAMEPAD;
				}

				float deadzone = 0.1f;
				std::vector<float> nowState = m_game->getPlayer()->getGamepadValues();
				for (size_t i = 0; i < m_pastGamepadValues.size(); ++i) {
					float diff = abs(m_pastGamepadValues.at(i) - nowState.at(i));
					if (diff > deadzone) {
						*changing = i;
						m_render.doneEditing();
						m_game->getPlayer()->writeMappingFile();
						break;
					}
				}
			}
			else if (m_render.m_editingKey) {
				int* changing = &(m_game->getPlayer()->GREEN_CODE);
				if (m_render.m_actionToChange == RED_INDEX) {
					changing = &(m_game->getPlayer()->RED_CODE);
				}
				else if (m_render.m_actionToChange == BLUE_INDEX) {
					changing = &(m_game->getPlayer()->BLUE_CODE);
				}
				else if (m_render.m_actionToChange == EU_INDEX) {
					changing = &(m_game->getPlayer()->EUPHORIA);
				}
				else if (m_render.m_actionToChange == CF_LEFT_INDEX) {
					changing = &(m_game->getPlayer()->CROSS_L_CODE);
				}
				else if (m_render.m_actionToChange == CF_RIGHT_INDEX) {
					changing = &(m_game->getPlayer()->CROSS_R_CODE);
				}
				else if (m_render.m_actionToChange == SCR_UP_INDEX) {
					changing = &(m_game->getPlayer()->SCRATCH_UP);
				}
				else if (m_render.m_actionToChange == SCR_DOWN_INDEX) {
					changing = &(m_game->getPlayer()->SCRATCH_DOWN);
				}
				
				for (int i = 0; i < 512; ++i) {
					if (glfwGetKey(m_render.getWindowPtr(), i)) {
						*changing = i;
						m_render.doneEditing();
						m_game->getPlayer()->writeMappingFile();
						break;
					}
				}
			}
		}
		else if (m_scene == 3) {
			if (m_isSelectPressed && !m_wasSelectPressed) {
				m_scene = 0;
				resetMenu();
			}
		}
		m_pastGamepadValues = m_game->getPlayer()->getGamepadValues();
	}
}

void MenuNavigator::render() {
	if (m_active) {
		if (m_scene == 0) {
			updateMenuNode();
			m_render.render(m_activeNode, m_selection.back(), m_viewOffset);
		}
		else if (m_scene == 1) {
			m_render.remapping(m_game);
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
		m_scene = 3;
	}
	else if (id == 4) {
		bool userinput = m_game->getPlayer()->m_useKeyboardInput;
		m_game->getPlayer()->m_useKeyboardInput = !userinput;
		m_useKeyboardInput = !userinput;
		std::cout << "Changed input to " << (userinput ? "Controller" : "Keyboard") << std::endl;
		resetMenu();
	}
	else if (id == 255) {
		index = findIndex(menu, parent);
		m_active = false;
		m_game->start(m_songList.at(index).path);
		resetMenu();
	}
	else if(menu.getChildCount() == 0){
		std::cout << "MenuNavigator: no function attached to id " << menu.getId() << std::endl;
	}
}

void MenuNavigator::remap() {
	m_scene = 1;
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

void MenuNavigator::resetMenu() {
	m_selection.erase(m_selection.begin(), m_selection.end());
	m_selection.push_back(0);
	m_viewOffset = 0;
}

void MenuNavigator::updateMenuNode() {
	MenuNode activeNode = m_root;
	for (size_t i = 0; i < m_selection.size() - 1; i++) {
		if (activeNode.getChildCount() > 0) {
			activeNode = activeNode.getChildrens().at(m_selection.at(i));
		}
	}
	m_activeNode = activeNode;
}

void MenuNavigator::updateGamepadState() {
	int count;
	const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &count);

	m_gpState.clear();
	for (int i = 0; i < count; ++i) {
		if (buttons[i] == '\0') {
			m_gpState.push_back(0.0f);
		}
		else {
			m_gpState.push_back(1.0f);
		}
		
	}
	const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

	for (int i = 0; i < count; ++i) {
		m_gpState.push_back(axes[i]);
	}
}

void MenuNavigator::setActive(bool active) {
	m_active = active;
}

bool MenuNavigator::getShouldClose()
{
	return m_shouldClose;
}

bool MenuNavigator::getActive() {
	return m_active;
}

MenuNavigator::~MenuNavigator(){
	m_render.~MenuRender();
}

