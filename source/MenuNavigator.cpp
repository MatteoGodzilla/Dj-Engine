#include "MenuNavigator.h"

size_t findIndex(MenuNode& element, MenuNode& parent) {
	std::vector<MenuNode>list = parent.getChildrens();
	for (size_t i = 0; i < list.size(); i++) {
		if (list.at(i).getText() == element.getText()) return i;
	}
	return 0xffffffff;
}

void MenuNavigator::writeConfigFile() {
	m_game->getPlayer()->writeMappingFile();

	std::ofstream output("config.txt", std::ios::app);
	if (output.is_open()) {
		output << "{Menu}" << std::endl;
		output << UP_CODE << std::endl;
		output << DOWN_CODE << std::endl;
		output << SELECT_CODE << std::endl;
		output << BACK_CODE << "\n\n";

		output << UP_GAMEPAD << std::endl;
		output << DOWN_GAMEPAD << std::endl;
		output << SELECT_GAMEPAD << std::endl;
		output << BACK_GAMEPAD << std::endl << std::endl;
		std::cout << "MenuNavigator Message: wrote mapping file" << std::endl;
		output.close();
	}
}

void MenuNavigator::readConfigFile() {
	std::ifstream input("config.txt");
	std::string s;
	while (s != std::string("{Menu}")) {
		std::getline(input, s);
		if (input.eof()) {
			std::cerr << "MenuNavigator Error: found config file, but not {Menu} marker.";
			std::cerr << "Stopped loading of config file" << std::endl;
			return;
		}
	}
	if (input.is_open()) {
		std::cout << "MenuNavigator Message: loading config from file" << std::endl;
		std::string token;
		input >> token;
		UP_CODE = std::stoi(token);
		input >> token;
		DOWN_CODE = std::stoi(token);
		input >> token;
		SELECT_CODE = std::stoi(token);
		input >> token;
		BACK_CODE = std::stoi(token);
		input >> token;
		UP_GAMEPAD = std::stoi(token);
		input >> token;
		DOWN_GAMEPAD = std::stoi(token);
		input >> token;
		SELECT_GAMEPAD = std::stoi(token);
		input >> token;
		BACK_GAMEPAD = std::stoi(token);
	}
	else {
		std::cerr << "MenuNavigator Error: Cannot open config file" << std::endl;
	}
}

MenuNavigator::MenuNavigator() {
	//create menu tree
	MenuNode play("Play!", 1);
	MenuNode options("Options", 2);
	MenuNode credits("Credits", 3);
	MenuNode exit("Exit", -1);

	MenuNode scratches("Test Scratches", 4);
	MenuNode latency("Calibrate latency", 5);
	MenuNode flipButtons("Toggle Buttons Right/Left", 6);
	MenuNode speed("Set Deck Speed", 7);
	MenuNode bot("Toggle Bot", 8);
	options.push(scratches);
	options.push(latency);
	options.push(flipButtons);
	options.push(bot);
	options.push(speed);

	m_root.push(play);
	m_root.push(options);
	m_root.push(credits);
	m_root.push(exit);

	m_selection.push_back(0);
	m_activeNode = m_root;

	m_gpDead.clear();
	for (int i = 0; i < 15 + 6; ++i) {
		m_gpDead.push_back(0.5);
	}
	readConfigFile();
}

void MenuNavigator::init(GLFWwindow* w, Game* gameptr) {
	m_pastGamepadValues = gameptr->getPlayer()->getGamepadValues();
	m_pastKBMState = gameptr->getPlayer()->getKBMValues(w);
	//std::cout << m_pastKBMState.at(0) << ";" << m_pastKBMState.at(1) << std::endl;
	m_game = gameptr;
	m_render.init(w);
	render(0.0f);
	scan();
}

void MenuNavigator::pollInput() {
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
		if (glfwJoystickPresent(m_game->getPlayer()->m_gamepadId)) {

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

	if (glfwGetKey(m_render.getWindowPtr(), GLFW_KEY_SPACE)) {
		if (m_scene != CALIBRATION) remap();
	}
	/*
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
	*/


	if (m_wasBackPressed && !m_isBackPressed) {
		if (m_popupId != -1) {
			m_popupId = -1;
			m_debounce = true;
		}
		else {
			glfwSetInputMode(m_render.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			resetMenu();
			if (m_scene == MAIN_SCENE && m_activeNode.getId() == m_root.getId()) {
				if (m_scene != 1) {
					m_shouldClose = true;
				}
			}
			else if (m_scene == CREDITS) {
				m_scene = MAIN_SCENE;
			}
		}
	}

	if (m_render.m_shouldClose) {
		if (m_popupId != -1) {
			m_popupId = -1;
			m_debounce = true;
		}
		else if (m_scene == REMAPPING) {
			writeConfigFile();
			m_scene = MAIN_SCENE;
			resetMenu();
		}
		else if (m_scene == CALIBRATION) {
			resetMenu();
			m_scene = MAIN_SCENE;
		}

	}

	if (m_render.m_input != m_useKeyboardInput) {
		m_useKeyboardInput = m_render.m_input;
	}

}

void MenuNavigator::update() {
	if (m_active) {
		if (m_scene == MAIN_SCENE) {

			/*
			activeNode is the selected node
			m_selection contains all selected node indices
			the last item in m_selection is the 'cursor position' moved by player
			*/
			m_render.tick();
			updateMenuNode();
			if (m_wasUpPressed && !m_isUpPressed) {
				//reset idle animation
				m_render.m_currentIdleTime = 0.0f;
				m_render.m_selectionDX = 0.0f;
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
				//reset idle animation
				m_render.m_currentIdleTime = 0.0f;
				m_render.m_selectionDX = 0.0f;
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
				//reset idle animation
				m_render.m_currentIdleTime = 0.0f;
				m_render.m_selectionDX = 0.0f;
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
			if (m_wasBackPressed && !m_isBackPressed && m_popupId == -1 && !m_debounce) {
				//reset idle animation
				m_render.m_currentIdleTime = 0.0f;
				m_render.m_selectionDX = 0.0f;
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
		else if (m_scene == REMAPPING) {
			if (m_render.m_editingAxis && m_render.m_gameActionToChange != -1) {
				int* changing = &(m_game->getPlayer()->GREEN_GAMEPAD);
				if (m_render.m_gameActionToChange == RED_INDEX) {
					changing = &(m_game->getPlayer()->RED_GAMEPAD);
				}
				else if (m_render.m_gameActionToChange == BLUE_INDEX) {
					changing = &(m_game->getPlayer()->BLUE_GAMEPAD);
				}
				else if (m_render.m_gameActionToChange == EU_INDEX) {
					changing = &(m_game->getPlayer()->EU_GAMEPAD);
				}
				else if (m_render.m_gameActionToChange == CF_LEFT_INDEX) {
					changing = &(m_game->getPlayer()->CF_LEFT_GAMEPAD);
				}
				else if (m_render.m_gameActionToChange == CF_RIGHT_INDEX) {
					changing = &(m_game->getPlayer()->CF_RIGHT_GAMEPAD);
				}
				else if (m_render.m_gameActionToChange == SCR_UP_INDEX) {
					changing = &(m_game->getPlayer()->SCR_UP_GAMEPAD);
				}
				else if (m_render.m_gameActionToChange == SCR_DOWN_INDEX) {
					changing = &(m_game->getPlayer()->SCR_DOWN_GAMEPAD);
				}
				else if (m_render.m_gameActionToChange == MENU_UP) {
					changing = &UP_GAMEPAD;
				}
				else if (m_render.m_gameActionToChange == MENU_DOWN) {
					changing = &DOWN_GAMEPAD;
				}
				else if (m_render.m_gameActionToChange == MENU_SELECT) {
					changing = &SELECT_GAMEPAD;
				}
				else if (m_render.m_gameActionToChange == MENU_BACK) {
					changing = &BACK_GAMEPAD;
				}

				float deadzone = 0.05f;

				std::vector<float> nowState = m_game->getPlayer()->getGamepadValues();
				for (size_t i = 0; i < m_pastGamepadValues.size(); ++i) {
					float diff = abs(m_pastGamepadValues.at(i) - nowState.at(i));
					if (diff > deadzone) {
						*changing = i;
						m_render.doneEditing();
						break;
					}
				}
			}
			else if (m_render.m_editingKey && m_render.m_gameActionToChange != -1) {
				int* changing = &(m_game->getPlayer()->GREEN_CODE);
				if (m_render.m_gameActionToChange == RED_INDEX) {
					changing = &(m_game->getPlayer()->RED_CODE);
				}
				else if (m_render.m_gameActionToChange == BLUE_INDEX) {
					changing = &(m_game->getPlayer()->BLUE_CODE);
				}
				else if (m_render.m_gameActionToChange == EU_INDEX) {
					changing = &(m_game->getPlayer()->EUPHORIA);
				}
				else if (m_render.m_gameActionToChange == CF_LEFT_INDEX) {
					changing = &(m_game->getPlayer()->CROSS_L_CODE);
				}
				else if (m_render.m_gameActionToChange == CF_RIGHT_INDEX) {
					changing = &(m_game->getPlayer()->CROSS_R_CODE);
				}
				else if (m_render.m_gameActionToChange == SCR_UP_INDEX) {
					changing = &(m_game->getPlayer()->SCRATCH_UP);
				}
				else if (m_render.m_gameActionToChange == SCR_DOWN_INDEX) {
					changing = &(m_game->getPlayer()->SCRATCH_DOWN);
				}

				float deadzone = 0.5f;
				std::vector<float> KBMState = m_game->getPlayer()->getKBMValues(m_render.getWindowPtr());

				for (size_t i = 0; i < m_pastKBMState.size(); ++i) {
					float diff = abs(m_pastKBMState.at(i) - KBMState.at(i));
					if (diff > deadzone) {
						*changing = i;
						m_render.doneEditing();
						break;
					}

				}
			}
			else if (m_render.m_editingAxis && m_render.m_menuActionToChange != -1) {
				int* changing = &UP_GAMEPAD;
				if (m_render.m_menuActionToChange == MENU_DOWN) {
					changing = &DOWN_GAMEPAD;
				}
				else if (m_render.m_menuActionToChange == MENU_SELECT) {
					changing = &SELECT_GAMEPAD;
				}
				else if (m_render.m_menuActionToChange == MENU_BACK) {
					changing = &BACK_GAMEPAD;
				}

				float deadzone = 0.5f;
				std::vector<float> nowState = m_game->getPlayer()->getGamepadValues();
				for (size_t i = 0; i < m_pastGamepadValues.size(); ++i) {
					float diff = abs(m_pastGamepadValues.at(i) - nowState.at(i));
					if (diff > deadzone) {
						*changing = i;
						m_render.doneEditing();
						break;
					}
				}

			}
			else if (m_render.m_editingKey && m_render.m_menuActionToChange != -1) {
				int* changing = &UP_CODE;
				if (m_render.m_menuActionToChange == MENU_DOWN) {
					changing = &DOWN_CODE;
				}
				else if (m_render.m_menuActionToChange == MENU_SELECT) {
					changing = &SELECT_CODE;
				}
				else if (m_render.m_menuActionToChange == MENU_BACK) {
					changing = &BACK_CODE;
				}

				float deadzone = 0.05f;
				std::vector<float> KBMState = m_game->getPlayer()->getKBMValues(m_render.getWindowPtr());

				for (size_t i = 0; i < m_pastKBMState.size(); ++i) {
					float diff = abs(m_pastKBMState.at(i) - KBMState.at(i));
					if (diff > deadzone) {
						*changing = i;
						m_render.doneEditing();
						break;
					}
				}
			}
			m_pastGamepadValues = m_game->getPlayer()->getGamepadValues();
			m_pastKBMState = m_game->getPlayer()->getKBMValues(m_render.getWindowPtr());
		}

		m_debounce = false;
	}
}

void MenuNavigator::render(double dt) {
	if (m_active) {
		if (m_scene == MAIN_SCENE) {
			updateMenuNode();
			m_render.render(m_activeNode, m_selection.back(), m_viewOffset);

			if (m_activeNode.getId() == m_root.getId()) {
				m_render.splashArt();
			}
		}
		else if (m_scene == REMAPPING) {
			m_render.remapping(m_game, { &UP_CODE, &DOWN_CODE, &SELECT_CODE, &BACK_CODE,
				&UP_GAMEPAD, &DOWN_GAMEPAD, &SELECT_GAMEPAD, &BACK_GAMEPAD });
		}
		else if (m_scene == CREDITS) {
			m_render.credits();
		}
		else if (m_scene == SCRATCHES) {
			m_render.scratches(m_game->getPlayer());
		}
		else if (m_scene == CALIBRATION) {
			m_render.calibration(m_game, dt);
		}
		if (m_popupId != -1) {
			if (m_popupId == 0) {
				m_render.setDeckSpeed(m_game);
			}
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
		m_scene = CREDITS;
	}
	else if (id == 4) {
		m_scene = SCRATCHES;
		glfwSetInputMode(m_render.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else if (id == 5) {
		m_scene = CALIBRATION;
	}
	else if (id == 6) {
		if (m_game->getPlayer()->m_isButtonsRight) {
			std::cout << "Game message: Changed Buttons to Right" << std::endl;
			m_game->setButtonPos(false);
		}
		else {
			std::cout << "Game message: Changed Buttons to Left" << std::endl;
			m_game->setButtonPos(true);
		}
	}
	else if (id == 7) {
		m_popupId = 0;
	}
	else if (id == 8) {
		if (m_game->getPlayer()->m_botEnabled) {
			std::cout << "Menu Message: disabled bot" << std::endl;
			m_game->getPlayer()->m_botEnabled = false;
		}
		else {
			std::cout << "Menu Message: enabled bot" << std::endl;
			m_game->getPlayer()->m_botEnabled = true;
		}
	}
	else if (id == 255) {
		index = findIndex(menu, parent);
		m_active = false;
		m_game->start(m_songList.at(index));
		resetMenu();
	}
	else if (menu.getChildCount() == 0) {
		if (menu.getId() == 1) {
			std::cout << "MenuNavigator Message: No songs found in the install path." << std::endl;
		}
		else {
			std::cout << "MenuNavigator Error: no function attached to id " << menu.getId() << std::endl;
		}
	}
}

void MenuNavigator::remap() {
	m_scene = REMAPPING;
}

void MenuNavigator::scan() {
	SongScanner::load("./songs", m_songList);
	for (const SongEntry& entry : m_songList) {
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
	m_popupId = -1;
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
	const unsigned char* buttons = glfwGetJoystickButtons(m_game->getPlayer()->m_gamepadId, &count);

	m_gpState.clear();
	for (int i = 0; i < count; ++i) {
		if (buttons[i] == '\0') {
			m_gpState.push_back(0.0f);
		}
		else {
			m_gpState.push_back(1.0f);
		}

	}
	const float* axes = glfwGetJoystickAxes(m_game->getPlayer()->m_gamepadId, &count);

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

MenuNavigator::~MenuNavigator() {
	m_render.~MenuRender();
}