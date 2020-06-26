#include "MenuNavigator.h"

size_t findIndex(MenuNode& element, MenuNode& parent) {
	std::vector<MenuNode> list = parent.getChildrens();
	for (size_t i = 0; i < list.size(); i++) {
		if (list.at(i).getText() == element.getText()) {
			return i;
		}
	}
	return 0xffffffff;
}

void MenuNavigator::writeConfigFile() {
	m_game->writeConfig();

	CSimpleIniA ini;
	const char* section = "Menu Mappings";
	ini.SetLongValue(section, "KB_up", UP_CODE);
	ini.SetLongValue(section, "KB_down", DOWN_CODE);
	ini.SetLongValue(section, "KB_select", SELECT_CODE);
	ini.SetLongValue(section, "KB_back", BACK_CODE);

	ini.SetLongValue(section, "GP_up", UP_GAMEPAD);
	ini.SetLongValue(section, "GP_down", DOWN_GAMEPAD);
	ini.SetLongValue(section, "GP_select", SELECT_GAMEPAD);
	ini.SetLongValue(section, "GP_back", BACK_GAMEPAD);

	std::string mappings;
	ini.Save(mappings);

	std::ofstream output("profile.ini", std::ios::app | std::ios::binary);
	if (output.is_open()) {
		output << std::endl
			   << mappings;
		std::cout << "MenuNavigator Message: written menu mappings to 'profile.ini'";
	}
}

void MenuNavigator::readConfigFile() {
	CSimpleIniA ini;
	ini.LoadFile("profile.ini");
	const char* section = "Menu Mappings";

	std::cout << "MenuNavigator Message: loading config from file" << std::endl;

	UP_CODE = ini.GetLongValue(section, "KB_up", 0);
	DOWN_CODE = ini.GetLongValue(section, "KB_down", 0);
	SELECT_CODE = ini.GetLongValue(section, "KB_select", 0);
	BACK_CODE = ini.GetLongValue(section, "KB_back", 0);

	UP_GAMEPAD = ini.GetLongValue(section, "GP_up", 0);
	DOWN_GAMEPAD = ini.GetLongValue(section, "GP_down", 0);
	SELECT_GAMEPAD = ini.GetLongValue(section, "GP_select", 0);
	BACK_GAMEPAD = ini.GetLongValue(section, "GP_back", 0);
}

void MenuNavigator::init(GLFWwindow* w, Game* gameptr) {
	m_pastGamepadValues = gameptr->getPlayer()->getGamepadValues();
	m_pastKBMState = gameptr->getPlayer()->getKBMValues(w);
	//std::cout << m_pastKBMState.at(0) << ";" << m_pastKBMState.at(1) << std::endl;
	m_game = gameptr;
	m_render.init(w);

	readConfigFile();

	std::ifstream available("config.ini");
	if (!available.is_open())
		writeConfigFile();

	//create menu tree
	MenuNode play("Play!", PLAY_ID);
	MenuNode options("Options", OPTIONS_ID);
	MenuNode credits("Credits", CREDITS_ID);
	MenuNode exit("Exit", EXIT_ID);

	MenuNode scratches("Test Scratches", SCRATCHES_ID);
	MenuNode latency("Calibrate latency", LATENCY_ID);
	MenuNode flipButtons("Toggle Buttons Right/Left:", LR_BUTTONS_ID);
	MenuNode speed("Set Deck Speed:", SPEED_ID);
	MenuNode refreshList("Refresh song list", REFRESH_ID);
	MenuNode bot("Toggle Bot:", BOT_ID);
	MenuNode color("Change lanes color", COLOR_ID);
	MenuNode pollrate("Change input poll rate:", POLL_ID);
	MenuNode debug("Toggle Debug Informations:", DEBUG_ID);

	//add values to text after:
	flipButtons.setText(flipButtons.getText() + std::string(m_game->getPlayer()->m_isButtonsRight ? "true" : "false"));
	speed.setText(speed.getText() + std::to_string(m_game->m_deckSpeed));
	bot.setText(bot.getText() + std::string(m_game->getPlayer()->m_botEnabled ? "true" : "false"));
	pollrate.setText(pollrate.getText() + std::to_string(m_game->m_inputThreadPollRate));
	debug.setText(debug.getText() + std::string(m_game->m_debugView ? "true" : "false"));

	//options.push(scratches);
	options.push(latency);
	options.push(flipButtons);
	options.push(speed);
	options.push(refreshList);
	options.push(bot);
	options.push(color);
	options.push(pollrate);
	options.push(debug);

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
	} else {
		if (glfwJoystickPresent(m_game->getPlayer()->m_gamepadId)) {
			updateGamepadState();

			if (!m_gpState.empty()) {
				m_isUpPressed = (m_gpState.at(UP_GAMEPAD) >= m_gpDead.at(UP_GAMEPAD));
				m_isDownPressed = (m_gpState.at(DOWN_GAMEPAD) >= m_gpDead.at(DOWN_GAMEPAD));
				m_isSelectPressed = (m_gpState.at(SELECT_GAMEPAD) >= m_gpDead.at(SELECT_GAMEPAD));
				m_isBackPressed = (m_gpState.at(BACK_GAMEPAD) >= m_gpDead.at(BACK_GAMEPAD));
			}
		}
	}

	if (glfwGetKey(m_render.getWindowPtr(), GLFW_KEY_SPACE)) {
		if (m_scene != CALIBRATION) {
			remap();
		}
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
			if (m_popupId == HIGHWAY_SPEED || m_popupId == LANE_COLORS) {
				writeConfigFile();
				if (m_popupId == HIGHWAY_SPEED) {
					m_root.getChildrens().at(1).getChildrens().at(2).setText(std::string("Set Deck Speed:") + std::to_string(m_game->m_deckSpeed));
				} else if (m_popupId == POLLRATE_CHANGE) {
					m_root.getChildrens().at(1).getChildrens().at(6).setText(std::string("Change input poll rate:") + std::to_string(m_game->m_inputThreadPollRate));
				}
			}
			m_popupId = -1;
			m_debounce = true;
		} else {
			glfwSetInputMode(m_render.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			resetMenu();
			if (m_scene == MAIN_SCENE && m_activeNode.getId() == m_root.getId()) {
				if (m_scene != 1) {
					m_shouldClose = true;
				}
			} else if (m_scene == CREDITS || m_scene == SCRATCHES || m_scene == RESULTS) {
				m_scene = MAIN_SCENE;
				resetMenu();
			}
		}
	}

	if (m_render.m_shouldClose) {
		if (m_popupId != -1) {
			if (m_popupId == HIGHWAY_SPEED || m_popupId == LANE_COLORS) {
				writeConfigFile();
				if (m_popupId == HIGHWAY_SPEED) {
					m_root.getChildrens().at(1).getChildrens().at(2).setText(std::string("Set Deck Speed:") + std::to_string(m_game->m_deckSpeed));
				} else if (m_popupId == POLLRATE_CHANGE) {
					m_root.getChildrens().at(1).getChildrens().at(6).setText(std::string("Change input poll rate:") + std::to_string(m_game->m_inputThreadPollRate));
				}
			}
			m_popupId = -1;
			m_debounce = true;
		} else if (m_scene == REMAPPING) {
			writeConfigFile();
			m_scene = MAIN_SCENE;
			resetMenu();
		} else if (m_scene == CALIBRATION) {
			writeConfigFile();
			resetMenu();
			m_scene = MAIN_SCENE;
		}
	}

	if (m_render.m_input != m_useKeyboardInput) {
		m_useKeyboardInput = m_render.m_input;
	}

	if (m_game->m_mode == 1) {
		m_scene = RESULTS;
		m_game->m_mode = 0;
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
				} else if (m_selection.back() == 0) {
					m_selection.back() = m_activeNode.getChildCount() - 1;
					if (m_activeNode.getChildCount() > m_render.VISIBLE_ENTRIES) {
						m_viewOffset = (int)(m_activeNode.getChildCount() - m_render.VISIBLE_ENTRIES);
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
				if (childNode.getChildCount() == 0) {
					//do something based on the node id
					activate(childNode, m_activeNode);
				} else {
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
		} else if (m_scene == REMAPPING) {
			if (m_render.m_editingAxis && m_render.m_ActionToChange != -1) {
				int* changing = &(m_game->getPlayer()->GREEN_GAMEPAD);
				if (m_render.m_ActionToChange == RED_INDEX) {
					changing = &(m_game->getPlayer()->RED_GAMEPAD);
				} else if (m_render.m_ActionToChange == BLUE_INDEX) {
					changing = &(m_game->getPlayer()->BLUE_GAMEPAD);
				} else if (m_render.m_ActionToChange == EU_INDEX) {
					changing = &(m_game->getPlayer()->EU_GAMEPAD);
				} else if (m_render.m_ActionToChange == CF_LEFT_INDEX) {
					changing = &(m_game->getPlayer()->CF_LEFT_GAMEPAD);
				} else if (m_render.m_ActionToChange == CF_RIGHT_INDEX) {
					changing = &(m_game->getPlayer()->CF_RIGHT_GAMEPAD);
				} else if (m_render.m_ActionToChange == SCR_UP_INDEX) {
					changing = &(m_game->getPlayer()->SCR_UP_GAMEPAD);
				} else if (m_render.m_ActionToChange == SCR_DOWN_INDEX) {
					changing = &(m_game->getPlayer()->SCR_DOWN_GAMEPAD);
				} else if (m_render.m_ActionToChange == MENU_UP) {
					changing = &UP_GAMEPAD;
				} else if (m_render.m_ActionToChange == MENU_DOWN) {
					changing = &DOWN_GAMEPAD;
				} else if (m_render.m_ActionToChange == MENU_SELECT) {
					changing = &SELECT_GAMEPAD;
				} else if (m_render.m_ActionToChange == MENU_BACK) {
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
			} else if (m_render.m_editingKey && m_render.m_ActionToChange != -1) {
				int* changing = &(m_game->getPlayer()->GREEN_CODE);
				if (m_render.m_ActionToChange == RED_INDEX) {
					changing = &(m_game->getPlayer()->RED_CODE);
				} else if (m_render.m_ActionToChange == BLUE_INDEX) {
					changing = &(m_game->getPlayer()->BLUE_CODE);
				} else if (m_render.m_ActionToChange == EU_INDEX) {
					changing = &(m_game->getPlayer()->EUPHORIA);
				} else if (m_render.m_ActionToChange == CF_LEFT_INDEX) {
					changing = &(m_game->getPlayer()->CROSS_L_CODE);
				} else if (m_render.m_ActionToChange == CF_RIGHT_INDEX) {
					changing = &(m_game->getPlayer()->CROSS_R_CODE);
				} else if (m_render.m_ActionToChange == SCR_UP_INDEX) {
					changing = &(m_game->getPlayer()->SCRATCH_UP);
				} else if (m_render.m_ActionToChange == SCR_DOWN_INDEX) {
					changing = &(m_game->getPlayer()->SCRATCH_DOWN);
				} else if (m_render.m_ActionToChange == MENU_UP) {
					changing = &UP_CODE;
				} else if (m_render.m_ActionToChange == MENU_DOWN) {
					changing = &DOWN_CODE;
				} else if (m_render.m_ActionToChange == MENU_SELECT) {
					changing = &SELECT_CODE;
				} else if (m_render.m_ActionToChange == MENU_BACK) {
					changing = &BACK_CODE;
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
		} else if (m_scene == REMAPPING) {
			m_render.remapping(m_game, {&UP_CODE, &DOWN_CODE, &SELECT_CODE, &BACK_CODE, &UP_GAMEPAD, &DOWN_GAMEPAD, &SELECT_GAMEPAD, &BACK_GAMEPAD});
		} else if (m_scene == CREDITS) {
			m_render.credits();
		} else if (m_scene == SCRATCHES) {
			m_render.scratches(m_game->getPlayer());
		} else if (m_scene == CALIBRATION) {
			m_render.calibration(m_game, dt);
		} else if (m_scene == RESULTS) {
			m_render.result(m_game);
		}
		if (m_popupId != -1) {
			if (m_popupId == HIGHWAY_SPEED) {
				m_render.setDeckSpeed(m_game);
			} else if (m_popupId == LANE_COLORS) {
				m_render.setLaneColors(m_game);
			} else if (m_popupId == POLLRATE_CHANGE) {
				m_render.setPollRate(m_game);
			}
		}
	}
}

void MenuNavigator::activate(MenuNode& menu, MenuNode& parent) {
	//every case represents a function called on activate
	size_t index = 0;
	int id = menu.getId();
	if (id == EXIT_ID) {
		m_shouldClose = true;
	} else if (id == CREDITS_ID) {
		m_scene = CREDITS;
	} else if (id == SCRATCHES_ID) {
		m_scene = SCRATCHES;
		glfwSetInputMode(m_render.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	} else if (id == LATENCY_ID) {
		m_scene = CALIBRATION;
	} else if (id == LR_BUTTONS_ID) {
		m_game->setButtonPos(!m_game->getPlayer()->m_isButtonsRight);
		//update options node text
		m_root.getChildrens().at(1).getChildrens().at(2).setText(std::string("Toggle Buttons Right/Left:") + std::string(m_game->getPlayer()->m_isButtonsRight ? "true" : "false"));
		writeConfigFile();
	} else if (id == SPEED_ID) {
		m_popupId = HIGHWAY_SPEED;
	} else if (id == BOT_ID) {
		m_game->getPlayer()->m_botEnabled = !m_game->getPlayer()->m_botEnabled;
		m_root.getChildrens().at(1).getChildrens().at(5).setText(std::string("Toggle Bot:") + std::string(m_game->getPlayer()->m_botEnabled ? "true" : "false"));
	} else if (id == DEBUG_ID) {
		m_game->m_debugView = !m_game->m_debugView;
		m_root.getChildrens().at(1).getChildrens().at(7).setText(std::string("Toggle Debug Informations:") + std::string(m_game->m_debugView ? "true" : "false"));
		writeConfigFile();
	} else if (id == REFRESH_ID) {
		std::vector<MenuNode> emptyList;
		m_root.getChildrens().at(0).getChildrens().clear();
		m_songList.clear();
		scan(false);
	} else if (id == COLOR_ID) {
		m_popupId = LANE_COLORS;
	} else if (id == POLL_ID) {
		m_popupId = POLLRATE_CHANGE;
	} else if (id == SONG_GENERAL_ID) {
		index = findIndex(menu, parent);
		m_active = false;
		m_game->reset();
		m_game->start(m_songList.at(index));
		resetMenu();
	} else if (menu.getChildCount() == 0) {
		if (menu.getId() == PLAY_ID) {
			std::cout << "Menu Message: No songs found in the install path." << std::endl;
		} else {
			std::cout << "Menu Error: no function attached to id " << menu.getId() << std::endl;
		}
	}
}

void MenuNavigator::remap() {
	m_scene = REMAPPING;
}

void MenuNavigator::scan(bool useCache) {
	SongScanner scanner = SongScanner();
	if (!useCache) {
		scanner.load("./songs", m_songList);
		SongScanner::writeCache(m_songList);
	} else {
		SongScanner::readCache(m_songList);
	}
	for (const SongEntry& entry : m_songList) {
		std::string text;
		if (!entry.s2.empty()) {
			text = entry.s1 + " vs " + entry.s2;
		} else {
			text = entry.s1;
		}
		MenuNode song(text, SONG_GENERAL_ID);
		m_root.getChildrens().at(0).push(song);
	}
}

void MenuNavigator::resetMenu() {
	m_selection.clear();
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
		} else {
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

bool MenuNavigator::getShouldClose() {
	return m_shouldClose;
}

bool MenuNavigator::getActive() {
	return m_active;
}
