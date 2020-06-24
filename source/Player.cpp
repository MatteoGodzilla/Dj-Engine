#include "Player.h"

#include <iostream>

Player::Player() {
	for (int i = 0; i < 8; ++i) {
		m_gpDead.push_back(0.5f);
		m_gpMult.push_back(1.0f);
	}

	m_gpDead.at(CF_RIGHT_INDEX) = 0.5f;
	m_gpDead.at(CF_LEFT_INDEX) = 0.5f;
	m_gpDead.at(SCR_UP_INDEX) = 0.2f;
	m_gpDead.at(SCR_DOWN_INDEX) = 0.2f;

	m_gpMult.at(SCR_UP_INDEX) = 1000.0f;
	m_gpMult.at(SCR_DOWN_INDEX) = 1000.0f;
	m_pastKBMState.resize(400);
}

void Player::pollInput(GLFWwindow* window, double time, std::vector<Note>& noteArr, std::vector<Note>& eventArr, std::vector<Note>& crossArr) {
	m_pastMouseX = m_nowMouseX;
	m_pastMouseY = m_nowMouseY;

	//mouse movement
	glfwGetCursorPos(window, &m_nowMouseX, &m_nowMouseY);
	if (m_deltaMouse) {
		int w;
		int h;
		glfwGetWindowSize(window, &w, &h);
		glfwSetCursorPos(window, (double)w / 2.0, (double)h / 2.0);
	}

	if (m_useKeyboardInput) {
		updateKBMState(window);
	} else {
		if (glfwJoystickPresent(m_gamepadId)) {
			updateGamepadState();
		}
	}

	if (m_gpState.size() >= 8 && m_gpHistory.size() >= m_historyLength) {
		if (getRisingEdge(GREEN_INDEX)) {
			if (getHittableNote(TAP_G, noteArr)) {
				hit(time, TAP_G, noteArr);
			} else if (!getHittableNote(SCR_G_ZONE, eventArr)) {
				m_combo = 0;
				m_eu_zone_active = false;
			}
		}
		if (getRisingEdge(RED_INDEX)) {
			hit(time, TAP_R, noteArr);
		}
		if (getRisingEdge(BLUE_INDEX)) {
			if (getHittableNote(TAP_B, noteArr)) {
				hit(time, TAP_B, noteArr);
			} else if (!getHittableNote(SCR_B_ZONE, eventArr)) {
				m_combo = 0;
				m_eu_zone_active = false;
			}
		}
		if (getRisingEdge(EU_INDEX)) {
			if (m_eu_value > 0.0) {
				m_euphoria_active = true;
			}
		}

		if (getFallingZero(SCR_UP_INDEX)) {
			if (getHittableNote(SCR_G_UP, noteArr) || getHittableNote(SCR_B_UP, noteArr)) {
				if (m_gpState.at(GREEN_INDEX) * m_gpMult.at(GREEN_INDEX) >= m_gpDead.at(GREEN_INDEX))
					hit(time, SCR_G_UP, noteArr);
				else if (m_gpState.at(BLUE_INDEX) * m_gpMult.at(BLUE_INDEX) >= m_gpDead.at(BLUE_INDEX))
					hit(time, SCR_B_UP, noteArr);
			} else {
				if (m_gpState.at(GREEN_INDEX) * m_gpMult.at(GREEN_INDEX) >= m_gpDead.at(GREEN_INDEX) && time - m_pastScratch > m_scratchDebounce)
					hit(time, SCR_G_UP, noteArr);
				else if (m_gpState.at(BLUE_INDEX) * m_gpMult.at(BLUE_INDEX) >= m_gpDead.at(BLUE_INDEX) && time - m_pastScratch > m_scratchDebounce)
					hit(time, SCR_B_UP, noteArr);
			}
		}
		if (getFallingZero(SCR_DOWN_INDEX)) {
			if (getHittableNote(SCR_G_DOWN, noteArr) || getHittableNote(SCR_B_DOWN, noteArr)) {
				if (m_gpState.at(GREEN_INDEX) * m_gpMult.at(GREEN_INDEX) >= m_gpDead.at(GREEN_INDEX))
					hit(time, SCR_G_DOWN, noteArr);
				else if (m_gpState.at(BLUE_INDEX) * m_gpMult.at(BLUE_INDEX) >= m_gpDead.at(BLUE_INDEX))
					hit(time, SCR_B_DOWN, noteArr);
			} else {
				if (m_gpState.at(GREEN_INDEX) * m_gpMult.at(GREEN_INDEX) >= m_gpDead.at(GREEN_INDEX) && time - m_pastScratch > m_scratchDebounce)
					hit(time, SCR_G_DOWN, noteArr);
				else if (m_gpState.at(BLUE_INDEX) * m_gpMult.at(BLUE_INDEX) >= m_gpDead.at(BLUE_INDEX) && time - m_pastScratch > m_scratchDebounce)
					hit(time, SCR_B_DOWN, noteArr);
			}
		}

		if (m_cross == 0) {
			if (m_baseMove == 0) {
				if (getRisingEdge(CF_RIGHT_INDEX)) {
					m_secondMove = 2;
					m_cross = 2;
					m_cfGreenToCenter = true;
					m_cfCenterToBlue = true;
					if (getHittableNote(CF_SPIKE_B, noteArr))
						hit(time, CF_SPIKE_B, noteArr);
					else
						hit(time, CROSS_B, crossArr);
				} else if (getFallingEdge(CF_LEFT_INDEX)) {
					m_baseMove = 1;
					m_cross = 1;
					m_cfGreenToCenter = true;
					if (getHittableNote(CF_SPIKE_C, noteArr))
						hit(time, CF_SPIKE_C, noteArr);
					else if (!getHittableNote(CF_SPIKE_B, noteArr) && !getHittableNote(CROSS_B, crossArr))
						hit(time, CROSS_C, crossArr);
				}
			} else if (m_baseMove == 2) {
				if (getFallingEdge(CF_LEFT_INDEX)) {
					m_secondMove = 1;
					m_cross = 2;
					m_cfGreenToCenter = true;
					m_cfCenterToBlue = true;
					hit(time, CROSS_B, crossArr);
				} else if (getFallingEdge(CF_RIGHT_INDEX)) {
					m_secondMove = 1;
					m_baseMove = 0;
				}
			}
		} else if (m_cross == 1) {
			if (getRisingEdge(CF_LEFT_INDEX)) {
				m_baseMove = 0;
				m_cross = 0;
				m_cfCenterToGreen = true;
				if (getHittableNote(CF_SPIKE_G, noteArr))
					hit(time, CF_SPIKE_G, noteArr);
				else
					hit(time, CROSS_G, crossArr);
			}

			if (getRisingEdge(CF_RIGHT_INDEX)) {
				m_baseMove = 2;
				m_cross = 2;
				m_cfCenterToBlue = true;
				if (getHittableNote(CF_SPIKE_B, noteArr))
					hit(time, CF_SPIKE_B, noteArr);
				else
					hit(time, CROSS_B, crossArr);
			}
		} else if (m_cross == 2) {
			if (m_baseMove == 2) {
				if (getRisingEdge(CF_LEFT_INDEX)) {
					m_secondMove = 0;
					m_cross = 0;
					m_cfBlueToCenter = true;
					m_cfCenterToGreen = true;
					if (getHittableNote(CF_SPIKE_G, noteArr))
						hit(time, CF_SPIKE_G, noteArr);
					else
						hit(time, CROSS_G, crossArr);
				} else if (getFallingEdge(CF_RIGHT_INDEX)) {
					m_baseMove = 1;
					m_cross = 1;
					m_cfBlueToCenter = true;
					if (getHittableNote(CF_SPIKE_C, noteArr))
						hit(time, CF_SPIKE_C, noteArr);
					else if (!getHittableNote(CF_SPIKE_G, noteArr) && !getHittableNote(CROSS_G, crossArr))
						hit(time, CROSS_C, crossArr);
				}
			} else if (m_baseMove == 0) {
				if (getFallingEdge(CF_RIGHT_INDEX)) {
					m_secondMove = 1;
					m_cross = 0;
					m_cfBlueToCenter = true;
					m_cfCenterToGreen = true;
					hit(time, CROSS_G, crossArr);
				} else if (getFallingEdge(CF_LEFT_INDEX)) {
					m_secondMove = 1;
					m_baseMove = 2;
				}
			}
		}
	}
	m_gpHistory.push_back(m_gpState);
	if (m_gpHistory.size() > m_historyLength) {
		m_gpHistory.pop_front();
	}
}

void Player::hit(double time, int noteType, std::vector<Note>& array) {
	bool found = false;
	//loop for every note
	for (auto& note : array) {
		//if there is a note in the clicker, add score
		if (note.getHit() && !note.getTouched() && note.getType() == noteType) {
			found = true;
			if (noteType == TAP_G || noteType == TAP_R || noteType == TAP_B) {
				m_score += 100 * m_mult;
				//check for chords (i.e multiple taps in the same time)
				if (note.getMilli() != m_past_tap) {
					m_combo++;
					m_past_tap = note.getMilli();
				}
			} else if (noteType == CROSS_G || noteType == CROSS_C || noteType == CROSS_B) {
				m_score += 100 * m_mult;
				m_combo++;
			} else if (noteType == SCR_G_UP || noteType == SCR_G_DOWN || noteType == SCR_B_UP || noteType == SCR_B_DOWN) {
				m_score += 100 * m_mult;
				m_combo++;

				m_pastScratch = note.getMilli();
			} else if (noteType == SCR_G_TICK || noteType == SCR_B_TICK) {
				m_score += 25 * m_mult;
				m_scr_tick++;
			}
			note.click();

			if (noteType == TAP_G || noteType == SCR_G_UP || noteType == SCR_G_DOWN || noteType == SCR_G_TICK)
				m_greenAnimation = true;
			else if (noteType == TAP_R)
				m_redAnimation = true;
			else if (noteType == TAP_B || noteType == SCR_B_UP || noteType == SCR_B_DOWN || noteType == SCR_B_TICK)
				m_blueAnimation = true;

			break;
		}
	}
	//if there isn't a note in the clicker, break combo
	if (!found) {
		breakCombo(time);
	}
}

	/*
void Player::pollInput(GLFWwindow* window) {
	m_pastCross = m_cross;
	m_pastMouseX = m_nowMouseX;
	m_pastMouseY = m_nowMouseY;

	glfwGetCursorPos(window, &m_nowMouseX, &m_nowMouseY);
	if (m_deltaMouse) {
		int w;
		int h;
		glfwGetWindowSize(window, &w, &h);
		glfwSetCursorPos(window, (double)w / 2.0, (double)h / 2.0);
	}

	//for exiting result screen with bot active
	m_wasGreenPressed = m_isGreenPressed;
	if (m_useKeyboardInput) {
		m_isGreenPressed = glfwGetKey(window, GREEN_CODE);
	} else {
		if (glfwJoystickPresent(m_gamepadId)) {
			updateGamepadState();

			if (!m_gpState.empty()) {
				//value * sensitivity >= deadzone
				m_isGreenPressed = (m_gpState.at(GREEN_INDEX) * m_gpMult.at(GREEN_INDEX) >= m_gpDead.at(GREEN_INDEX));
			}
		}
	}

	if (!m_botEnabled) {
		m_wasRedPressed = m_isRedPressed;

		m_wasBluePressed = m_isBluePressed;
		m_wasUpPressed = m_isUpPressed;
		m_wasDownPressed = m_isDownPressed;
		m_wasCfGreenPressed = m_isCfGreenPressed;
		m_wasCfBluePressed = m_isCfBluePressed;

		if (m_useKeyboardInput) {
			updateKBMState(window);
		} else {
			if (glfwJoystickPresent(m_gamepadId)) {
				updateGamepadState();
			}
		}

		if (!m_gpState.empty()) {
			//value * sensitivity >= deadzone
			m_isGreenPressed = (m_gpState.at(GREEN_INDEX) * m_gpMult.at(GREEN_INDEX) >= m_gpDead.at(GREEN_INDEX));
			m_isRedPressed = (m_gpState.at(RED_INDEX) * m_gpMult.at(RED_INDEX) >= m_gpDead.at(RED_INDEX));
			m_isBluePressed = (m_gpState.at(BLUE_INDEX) * m_gpMult.at(BLUE_INDEX) >= m_gpDead.at(BLUE_INDEX));
			m_isEuPressed = (m_gpState.at(EU_INDEX) * m_gpMult.at(EU_INDEX) >= m_gpDead.at(EU_INDEX));

			if (m_gpState.at(CF_LEFT_INDEX) * m_gpMult.at(CF_LEFT_INDEX) >= m_gpDead.at(CF_LEFT_INDEX)) {
				m_isCfGreenPressed = true;
				m_isCfBluePressed = false;
			} else {
				m_isCfGreenPressed = false;
			}
			//value * sensitivity >= deadzone
			if (m_gpState.at(CF_RIGHT_INDEX) * m_gpMult.at(CF_RIGHT_INDEX) >= m_gpDead.at(CF_RIGHT_INDEX)) {
				m_isCfBluePressed = true;
				m_isCfGreenPressed = false;
			} else {
				m_isCfBluePressed = false;
			}
			//value * sensitivity >= deadzone
			if (m_gpState.at(SCR_UP_INDEX) * m_gpMult.at(SCR_UP_INDEX) >= m_gpDead.at(SCR_UP_INDEX)) {
				m_isUpPressed = true;
				m_isDownPressed = false;
			} else {
				m_isUpPressed = false;
			}
			//value * sensitivity >= deadzone
			if (m_gpState.at(SCR_DOWN_INDEX) * m_gpMult.at(SCR_DOWN_INDEX) >= m_gpDead.at(SCR_DOWN_INDEX)) {
				m_isDownPressed = true;
				m_isUpPressed = false;
			} else {
				m_isDownPressed = false;
			}
		}

		if (m_isButtonsRight) {
			bool temp = m_isCfGreenPressed;
			m_isCfGreenPressed = m_isCfBluePressed;
			m_isCfBluePressed = temp;
		}

		if (!m_euphoria_active) {
			if (m_isCfGreenPressed && !m_wasCfGreenPressed) {
				m_cross = 0;
			} else if (m_isCfBluePressed && !m_wasCfBluePressed) {
				m_cross = 2;
			} else if (m_wasCfGreenPressed && !m_isCfGreenPressed && !m_isCfBluePressed) {
				m_cross = 1;
			} else if (m_wasCfBluePressed && !m_isCfBluePressed && !m_isCfGreenPressed) {
				m_cross = 1;
			}
		}
	}
}

void Player::hit(double time, std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& cross) {
	//green pressed
	if (m_isGreenTapEnabled) {
		if (m_isGreenPressed && !m_wasGreenPressed) {
			bool found = false;
			//loop for every note
			for (auto& note : v) {
				//if there is a note in the clicker, add score
				if (note.getHit() && !note.getTouched() && note.getType() == TAP_G) {
					found = true;
					m_score += 100 * m_mult;
					//check for chords (i.e multiple taps in the same time)
					if (note.getMilli() != m_past_tap) {
						m_combo++;
						m_past_tap = note.getMilli();
					}
					note.click();
					break;
				}
			}
			//loop for every event
			for (auto& event : ev) {
				if (event.getHit() && event.getType() == SCR_G_ZONE) {
					found = true;
					break;
				}
			}
			//if there isn't a note in the clicker, break the combo
			if (!found) {
				breakCombo(time);
			} else {
				m_greenAnimation = true; //start animation
			}
		}
	} else {
		if (m_isGreenPressed && !m_wasGreenPressed) {
			if (m_isUpPressed) {
				//loop for every note
				for (auto& note : v) {
					int type = note.getType();
					if ((type == SCR_G_UP || type == SCR_G_ANY) && note.getHit() && !note.getTouched()) {
						note.click();
						m_score += 100 * m_mult;
						m_combo++;
					}
				}
			} else if (m_isDownPressed) {
				//loop for every note
				for (auto& note : v) {
					int type = note.getType();
					if ((type == SCR_G_DOWN || type == SCR_G_ANY) && note.getHit() && !note.getTouched()) {
						note.click();
						m_score += 100 * m_mult;
						m_combo++;
					}
				}
			}
		}
	}
	//red pressed
	if (m_isRedPressed && !m_wasRedPressed && m_isRedTapEnabled) {
		bool found = false;
		//loop for every note
		for (auto& note : v) {
			//if there is a note in the clicker, add score
			if (note.getHit() && !note.getTouched() && note.getType() == TAP_R) {
				found = true;
				m_score += 100 * m_mult;
				if (note.getMilli() != m_past_tap) {
					m_combo++;
					m_past_tap = note.getMilli();
				}
				note.click();
				break;
			}
		}
		//if there isn't a note in the clicker, break combo
		if (!found) {
			breakCombo(time);
		} else {
			m_redAnimation = true; //start animation
		}
	}
	//blue pressed
	if (m_isBlueTapEnabled) {
		if (m_isBluePressed && !m_wasBluePressed) {
			bool found = false;
			//loop for every note
			for (auto& note : v) {
				//if there is a note in the clicker, add score
				if (note.getHit() && !note.getTouched() && note.getType() == TAP_B) {
					found = true;
					m_score += 100 * m_mult;
					if (note.getMilli() != m_past_tap) {
						m_combo++;
						m_past_tap = note.getMilli();
					}
					note.click();
					break;
				}
			}
			//loop for every event
			for (auto& event : ev) {
				if (event.getHit() && event.getType() == SCR_B_ZONE) {
					found = true;
					break;
				}
			}
			//if there isn't a note in the clicker, break combo
			if (!found) {
				breakCombo(time);
			} else {
				m_blueAnimation = true; // start animation
			}
		}
	} else {
		if (m_isBluePressed && !m_wasBluePressed) {
			if (m_isUpPressed) {
				//loop for every note
				for (auto& note : v) {
					int type = note.getType();
					if ((type == SCR_B_UP || type == SCR_B_ANY) && note.getHit() && !note.getTouched()) {
						note.click();
						m_score += 100 * m_mult;
						m_combo++;
					}
				}
			} else if (m_isDownPressed) {
				//loop for every note
				for (auto& note : v) {
					int type = note.getType();
					if ((type == SCR_B_DOWN || type == SCR_B_ANY) && note.getHit() && !note.getTouched()) {
						note.click();
						m_score += 100 * m_mult;
						m_combo++;
					}
				}
			}
		}
	}
	//cross left pressed/moved
	if (m_isCfGreenPressed && !m_wasCfGreenPressed) {
		m_cfCenterToGreen = true;
		bool found = false;
		//loop for every crossfade
		for (auto& c : cross) {
			//if there is a note in the clicker, add score
			int type = c.getType();
			if (!c.getTouched()) {
				if (c.getHit()) {
					if (type == CROSS_G) {
						c.click();
						m_score += 100 * m_mult;
						m_combo++;
						found = true;
						break;
					}
				}
			} else if (m_euphoria_active) {
				found = true;
				break;
			}
		}
		for (auto& note : v) {
			int type = note.getType();
			if (note.getHit() && !note.getTouched()) {
				if (type == CF_SPIKE_G) {
					note.click();
					found = true;
					break;
				}
			}
		}
		//if there isn't a note in the clicker, break combo
		if (!found) {
			breakCombo(time);
		}
	}
	//cross right pressed/moved
	if (m_isCfBluePressed && !m_wasCfBluePressed) {
		m_cfCenterToBlue = true;
		bool found = false;
		//loop for every crossfade
		for (auto& c : cross) {
			//if there is a note in the clicker, add score
			int type = c.getType();

			if (!c.getTouched()) {
				if (c.getHit()) {
					if (type == CROSS_B) {
						found = true;
						c.click();
						m_score += 100 * m_mult;
						m_combo++;
						break;
					}
				}
			} else if (m_euphoria_active) {
				found = true;
				break;
			}
		}

		for (auto& note : v) {
			int type = note.getType();
			if (note.getHit() && !note.getTouched()) {
				if (type == CF_SPIKE_B) {
					note.click();
					found = true;
					break;
				}
			}
		}

		//if there isn't a note in the clicker, break combo
		if (!found) {
			breakCombo(time);
		}
	}
	//up pressed
	if (m_isUpPressed && !m_wasUpPressed) {
		//if the green button is already pressed
		if (m_isGreenPressed) {
			bool found = false;
			//loop for every note
			for (auto& note : v) {
				if (note.getHit() && !note.getTouched()) {
					int type = note.getType();
					//if there is a scratch up inside the clicker
					if (type == SCR_G_UP) {
						found = true;
						note.click();
						m_score += 100 * m_mult;
						m_combo++;
						break;
					}
					//if there is a scratch anydir inside the clicker
					else if (type == SCR_G_ANY || type == SCR_G_TICK) {
						if (!note.getTouched()) {
							found = true;
							note.click();
							m_score += 25 * m_mult;
							m_scr_tick++;
						}
					}
				}
			}
			//if there isn't a note in the clicker, break combo
			if (!found) {
				breakCombo(time);
			} else {
				m_greenAnimation = true;
			}
		}
		//if the blue button is already pressed
		if (m_isBluePressed) {
			bool found = false;
			//loop for every note
			for (auto& note : v) {
				if (note.getHit() && !note.getTouched() && !note.getTouched()) {
					int type = note.getType();
					//if there is a scratch up inside the clicker
					if (type == SCR_B_UP) {
						found = true;
						note.click();
						m_score += 100 * m_mult;
						m_combo++;
						break;
					}
					//if there is a scratch anydir inside the clicker
					else if (type == SCR_B_ANY || type == SCR_B_TICK) {
						found = true;
						note.click();
						m_score += 25 * m_mult;
						m_scr_tick++;
					}
				}
			}

			//if there isn't a note in the clicker, break combo
			if (!found) {
				breakCombo(time);
			} else {
				m_blueAnimation = true;
			}
		}
	}
	//down pressed
	if (m_isDownPressed && !m_wasDownPressed) {
		//if the green button is already pressed
		if (m_isGreenPressed) {
			bool found = false;
			//loop for every note
			for (auto& note : v) {
				if (note.getHit() && !note.getTouched()) {
					int type = note.getType();
					//if there is a scratch down inside the clicker
					if (type == SCR_G_DOWN) {
						found = true;
						note.click();
						m_score += 100 * m_mult;
						m_combo++;
						break;
					}
					//if there is a scratch anydir inside the clicker
					else if (type == SCR_G_ANY || type == SCR_G_TICK) {
						if (!note.getTouched()) {
							found = true;
							note.click();
							m_score += 25 * m_mult;
							m_scr_tick++;
						}
					}
				}
			}
			//if there isn't a note in the clicker, break combo
			if (!found) {
				breakCombo(time);
			} else {
				m_greenAnimation = true;
			}
		}
		//if the blue button is already pressed
		if (m_isBluePressed) {
			bool found = false;
			//loop for every note
			for (auto& note : v) {
				if (note.getHit() && !note.getTouched()) {
					int type = note.getType();
					//if there is a scratch down inside the clicker
					if (type == SCR_B_DOWN) {
						found = true;
						note.click();
						m_score += 100 * m_mult;
						m_combo++;
						break;
					}
					//if there is a scratch anydir inside the clicker
					else if (type == SCR_B_ANY || type == SCR_B_TICK) {
						found = true;
						note.click();
						m_score += 25 * m_mult;
						m_scr_tick++;
					}
				}
			}
			//if there isn't a note in the clicker, break combo
			if (!found) {
				breakCombo(time);
			} else {
				m_blueAnimation = true;
			}
		}
	}
	if (m_isEuPressed) {
		//activate euphoria if there is some
		if (m_eu_value > 0.0) {
			m_euphoria_active = true;
		}
	}
	if (m_isUpPressed || m_isDownPressed) {
		for (auto& note : v) {
			if (note.getHit() && !note.getTouched() && !note.getTouched()) {
				int type = note.getType();
				if (type == SCR_G_TICK && m_isGreenPressed) {
					note.click();
					m_greenAnimation = true;
					m_score += 25 * m_mult;
					m_scr_tick++;
				} else if (type == SCR_B_TICK && m_isBluePressed) {
					note.click();
					m_blueAnimation = true;
					m_score += 25 * m_mult;
					m_scr_tick++;
				}
			}
		}
	}
	//release section
	//cross left released
	if (m_wasCfGreenPressed && !m_isCfGreenPressed) {
		m_cfGreenToCenter = true;
		bool found = false;
		//loop for every crossfade
		for (auto& c : cross) {
			int type = c.getType();
			if (!c.getTouched()) {
				if (c.getHit()) {
					if (type == CROSS_C) {
						c.click();
						m_score += 100 * m_mult;
						m_combo++;
						found = true;
						break;
					} else if (type == CROSS_B) {
						found = true;
						break;
					}
				}
			} else if (m_euphoria_active) {
				found = true;
				break;
			}
		}

		//loop for every note
		for (auto& note : v) {
			int type = note.getType();
			if (note.getHit() && !note.getTouched()) {
				if (type == CF_SPIKE_C) {
					note.click();
					found = true;
					break;
				} else if (type == CF_SPIKE_B) {
					found = true;
					break;
				}
			}
		}

		//if there isn't an event in the clicker, break combo
		if (!found) {
			breakCombo(time);
		}
	}
	if (m_wasCfBluePressed && !m_isCfBluePressed) {
		m_cfBlueToCenter = true;
		bool found = false;
		//loop for every crossfade
		for (auto& c : cross) {
			int type = c.getType();
			if (!c.getTouched()) {
				if (c.getHit()) {
					if (type == CROSS_C) {
						c.click();
						m_score += 100 * m_mult;
						m_combo++;
						found = true;
						break;
					}
					if (type == CROSS_G) {
						found = true;
						break;
					}
				}
			} else if (m_euphoria_active) {
				found = true;
				break;
			}
		}

		//loop for every note
		for (auto& note : v) {
			int type = note.getType();
			if (note.getHit() && !note.getTouched()) {
				if (type == CF_SPIKE_C) {
					note.click();
					found = true;
					break;
				} else if (type == CF_SPIKE_G) {
					found = true;
					break;
				}
			}
		}

		//if there isn't an event in the clicker, break combo
		if (!found) {
			breakCombo(time);
		}
	}
	if (m_euphoria_active) {
		for (auto& note : v) {
			int type = note.getType();
			if (note.getMilli() < time && !note.getTouched()) {
				if (type == CF_SPIKE_G) {
					note.click();
					m_cfGreenToCenter = true;
					if (m_cross == 2) {
						m_cfCenterToBlue = true;
					}
					break;
				} else if (type == CF_SPIKE_B) {
					note.click();
					m_cfBlueToCenter = true;
					if (m_cross == 0) {
						m_cfCenterToGreen = true;
					}
					break;
				} else if (type == CF_SPIKE_C) {
					note.click();
					if (m_cross == 0) {
						m_cfCenterToGreen = true;
					} else if (m_cross == 2) {
						m_cfCenterToBlue = true;
					}
					break;
				}
			}
		}

		for (auto& c : cross) {
			int type = c.getType();
			if (c.getMilli() < time && c.getMilli() > m_lastCrossTime && !c.getTouched()) {
				if (type == CROSS_G) {
					if (m_cross != 0) {
						m_cfCenterToGreen = true;
					}
					if (m_cross == 2) {
						m_cfBlueToCenter = true;
					}
					m_cross = 0;
					c.click();
					m_score += 100 * m_mult;
					m_combo++;
					m_lastCrossTime = c.getMilli();
					break;
				} else if (type == CROSS_C) {
					if (m_cross == 0) {
						m_cfGreenToCenter = true;
					} else if (m_cross == 2) {
						m_cfBlueToCenter = true;
					}
					m_cross = 1;
					c.click();
					m_score += 100 * m_mult;
					m_combo++;
					m_lastCrossTime = c.getMilli();
					break;
				} else if (type == CROSS_B) {
					if (m_cross != 2) {
						m_cfCenterToBlue = true;
					}
					if (m_cross == 0) {
						m_cfGreenToCenter = true;
					}
					m_cross = 2;
					c.click();
					m_score += 100 * m_mult;
					m_combo++;
					m_lastCrossTime = c.getMilli();
					break;
				}
			}
		}
	}
	if (m_botEnabled) {
		for (auto& note : v) {
			int type = note.getType();
			if (note.getMilli() < time && !note.getTouched()) {
				if (type == TAP_R) {
					m_score += 100 * m_mult;
					if (note.getMilli() != m_past_tap) {
						m_combo++;
						m_past_tap = note.getMilli();
					}
					note.click();
					m_redAnimation = true;
					break;
				} else if (type == TAP_G) {
					m_score += 100 * m_mult;
					//check for chords (i.e multiple taps in the same time)
					if (note.getMilli() != m_past_tap) {
						m_combo++;
						m_past_tap = note.getMilli();
					}
					note.click();
					m_greenAnimation = true;
					break;
				} else if (type == TAP_B) {
					m_score += 100 * m_mult;
					if (note.getMilli() != m_past_tap) {
						m_combo++;
						m_past_tap = note.getMilli();
					}
					note.click();
					m_blueAnimation = true;
					break;
				} else if (type == SCR_G_UP || type == SCR_G_DOWN) {
					note.click();
					m_greenAnimation = true;
					m_score += 100 * m_mult;
					m_combo++;
					break;
				} else if (type == SCR_G_ANY || type == SCR_G_TICK) {
					note.click();
					m_greenAnimation = true;
					m_score += 25 * m_mult;
					m_scr_tick++;
					break;
				} else if (type == SCR_B_UP || type == SCR_B_DOWN) {
					note.click();
					m_blueAnimation = true;
					m_score += 100 * m_mult;
					m_combo++;
					break;
				} else if (type == SCR_B_ANY || type == SCR_B_TICK) {
					note.click();
					m_blueAnimation = true;
					m_score += 25 * m_mult;
					m_scr_tick++;
					break;
				}

				else if (type == CF_SPIKE_G) {
					note.click();
					m_cfGreenToCenter = true;
					if (m_cross == 2) {
						m_cfCenterToBlue = true;
					}
					break;
				} else if (type == CF_SPIKE_B) {
					note.click();
					m_cfBlueToCenter = true;
					if (m_cross == 0) {
						m_cfCenterToGreen = true;
					}
					break;
				} else if (type == CF_SPIKE_C) {
					note.click();
					if (m_cross == 0) {
						m_cfCenterToGreen = true;
					} else if (m_cross == 2) {
						m_cfCenterToBlue = true;
					}
					break;
				}
			}
		}

		for (auto& c : cross) {
			int type = c.getType();
			if (c.getMilli() < time && c.getMilli() > m_lastCrossTime && !c.getTouched()) {
				if (type == CROSS_G) {
					if (m_cross != 0) {
						m_cfCenterToGreen = true;
					}
					if (m_cross == 2) {
						m_cfBlueToCenter = true;
					}
					m_cross = 0;
					c.click();
					m_score += 100 * m_mult;
					m_combo++;
					m_lastCrossTime = c.getMilli();
					break;
				} else if (type == CROSS_C) {
					if (m_cross == 0) {
						m_cfGreenToCenter = true;
					} else if (m_cross == 2) {
						m_cfBlueToCenter = true;
					}
					m_cross = 1;
					c.click();
					m_score += 100 * m_mult;
					m_combo++;
					m_lastCrossTime = c.getMilli();
					break;
				} else if (type == CROSS_B) {
					if (m_cross != 2) {
						m_cfCenterToBlue = true;
					}
					if (m_cross == 0) {
						m_cfGreenToCenter = true;
					}
					m_cross = 2;
					c.click();
					m_score += 100 * m_mult;
					m_combo++;
					m_lastCrossTime = c.getMilli();
					break;
				}
			}
		}
	}
}
*/

	//update combo/multiplier for every frame
	void Player::tick(double time) {
	if (m_combo >= 24) {
		m_mult = 4;
	} else if (m_combo >= 16 && m_combo < 24) {
		m_mult = 3;
	} else if (m_combo >= 8 && m_combo < 16) {
		m_mult = 2;
	} else {
		m_mult = 1;
	}

	if (m_scr_tick >= 4) {
		m_scr_tick -= 4;
		m_combo++;
	}

	//decrease euphoria if active
	if (m_euphoria_active) {
		if (m_eu_value < 0.0) {
			m_euphoria_active = false;
			m_eu_value = 0.0f;
		} else {
			m_double_mult = true;
			double dt = time - m_lastTapTime;
			double de = (dt * m_genBpm) / 960;
			m_eu_value -= de;
		}
	} else {
		m_double_mult = false;
	}

	if (m_double_mult) {
		m_mult *= 2;
	}
	m_lastTapTime = time;

	if (m_combo > m_highestCombo) {
		m_highestCombo = m_combo;
	}
}

void Player::breakCombo(double time){
	if(time > 0){
		m_combo = 0;
		m_eu_zone_active = false;
		m_hasPlayerBrokeOnce = true;
	}
}

void Player::readMappingFile() {
	CSimpleIniA ini;
	const char* section = "Game Mappings";
	ini.LoadFile("profile.ini");

	GREEN_CODE = ini.GetLongValue(section, "KB_green", 0);
	RED_CODE = ini.GetLongValue(section, "KB_red", 0);
	BLUE_CODE = ini.GetLongValue(section, "KB_blue", 0);
	EUPHORIA = ini.GetLongValue(section, "KB_euphoria", 0);
	CROSS_L_CODE = ini.GetLongValue(section, "KB_crossLeft", 0);
	CROSS_R_CODE = ini.GetLongValue(section, "KB_crossRight", 0);
	SCRATCH_UP = ini.GetLongValue(section, "KB_scratchUp", 0);
	SCRATCH_DOWN = ini.GetLongValue(section, "KB_scratchDown", 0);

	GREEN_GAMEPAD = ini.GetLongValue(section, "GP_green", 0);
	RED_GAMEPAD = ini.GetLongValue(section, "GP_red", 0);
	BLUE_GAMEPAD = ini.GetLongValue(section, "GP_blue", 0);
	EU_GAMEPAD = ini.GetLongValue(section, "GP_euphoria", 0);
	m_useSingleCfAxis = ini.GetBoolValue(section, "GP_sameAxisForCrossfade", false);
	CF_LEFT_GAMEPAD = ini.GetLongValue(section, "GP_crossLeft", 0);
	CF_RIGHT_GAMEPAD = ini.GetLongValue(section, "GP_crossRight", 0);
	m_useSingleScrAxis = ini.GetBoolValue(section, "GP_sameAxisForScratch", false);
	SCR_UP_GAMEPAD = ini.GetLongValue(section, "GP_scratchUp", 0);
	SCR_DOWN_GAMEPAD = ini.GetLongValue(section, "GP_scratchDown", 0);

	const char* sensitivityKey = "MULT_";
	for (size_t i = 0; i < m_gpMult.size(); i++) {
		std::string key = std::string(sensitivityKey) + std::to_string(i);
		m_gpMult.at(i) = ini.GetDoubleValue(section, key.c_str(), 0.0);
	}

	const char* deadzoneKey = "DEAD_";
	for (size_t i = 0; i < m_gpDead.size(); i++) {
		std::string key = std::string(deadzoneKey) + std::to_string(i);
		m_gpDead.at(i) = ini.GetDoubleValue(section, key.c_str(), 0.0);
	}
}

void Player::writeMappingFile() {
	CSimpleIniA ini;
	const char* section = "Game Mappings";

	ini.SetLongValue(section, "KB_green", GREEN_CODE);
	ini.SetLongValue(section, "KB_red", RED_CODE);
	ini.SetLongValue(section, "KB_blue", BLUE_CODE);
	ini.SetLongValue(section, "KB_euphoria", EUPHORIA);
	ini.SetLongValue(section, "KB_crossLeft", CROSS_L_CODE);
	ini.SetLongValue(section, "KB_crossRight", CROSS_R_CODE);
	ini.SetLongValue(section, "KB_scratchUp", SCRATCH_UP);
	ini.SetLongValue(section, "KB_scratchDown", SCRATCH_DOWN);

	ini.SetLongValue(section, "GP_green", GREEN_GAMEPAD);
	ini.SetLongValue(section, "GP_red", RED_GAMEPAD);
	ini.SetLongValue(section, "GP_blue", BLUE_GAMEPAD);
	ini.SetLongValue(section, "GP_euphoria", EU_GAMEPAD);
	ini.SetBoolValue(section, "GP_sameAxisForCrossfade", m_useSingleCfAxis);
	ini.SetLongValue(section, "GP_crossLeft", CF_LEFT_GAMEPAD);
	ini.SetLongValue(section, "GP_crossRight", CF_RIGHT_GAMEPAD);
	ini.SetBoolValue(section, "GP_sameAxisForScratch", m_useSingleScrAxis);
	ini.SetLongValue(section, "GP_scratchUp", SCR_UP_GAMEPAD);
	ini.SetLongValue(section, "GP_scratchDown", SCR_DOWN_GAMEPAD);

	const char* sensitivityKey = "MULT_";
	for (size_t i = 0; i < m_gpMult.size(); i++) {
		std::string key = std::string(sensitivityKey) + std::to_string(i);
		ini.SetDoubleValue(section, key.c_str(), m_gpMult.at(i));
	}

	const char* deadzoneKey = "DEAD_";
	for (size_t i = 0; i < m_gpDead.size(); i++) {
		std::string key = std::string(deadzoneKey) + std::to_string(i);
		ini.SetDoubleValue(section, key.c_str(), m_gpDead.at(i));
	}

	ini.SaveFile("profile.ini");
	std::cout << "Player Message: written game mappings to 'profile.ini'" << std::endl;
}

void Player::updateKBMState(GLFWwindow* w) {
	std::vector<float> KBMState = getKBMValues(w);
	std::vector<float> localState;

	localState.push_back(KBMState.at(GREEN_CODE));
	localState.push_back(KBMState.at(RED_CODE));
	localState.push_back(KBMState.at(BLUE_CODE));
	localState.push_back(KBMState.at(EUPHORIA));
	localState.push_back(KBMState.at(CROSS_L_CODE));
	localState.push_back(KBMState.at(CROSS_R_CODE));
	localState.push_back(KBMState.at(SCRATCH_UP));
	localState.push_back(KBMState.at(SCRATCH_DOWN));

	m_gpState = localState;
}

void Player::updateGamepadState() {
	if (glfwJoystickPresent(m_gamepadId)) {
		int count;
		const unsigned char* buttons = glfwGetJoystickButtons(m_gamepadId, &count);

		std::vector<float> gamepadAxes;
		std::vector<float> localState;

		for (int i = 0; i < count; ++i) {
			if (buttons[i] == '\0') {
				gamepadAxes.push_back(0.0f);
			} else {
				gamepadAxes.push_back(1.0f);
			}
		}
		const float* axes = glfwGetJoystickAxes(m_gamepadId, &count);

		for (int i = 0; i < count; ++i) {
			gamepadAxes.push_back(axes[i]);
		}

		if (count > 0) {
			GREEN_GAMEPAD = std::max(GREEN_GAMEPAD, 0);
			GREEN_GAMEPAD = std::min(GREEN_GAMEPAD, (int)gamepadAxes.size() - 1);
			localState.push_back(gamepadAxes.at(GREEN_GAMEPAD));
			RED_GAMEPAD = std::max(RED_GAMEPAD, 0);
			RED_GAMEPAD = std::min(RED_GAMEPAD, (int)gamepadAxes.size() - 1);
			localState.push_back(gamepadAxes.at(RED_GAMEPAD));
			BLUE_GAMEPAD = std::max(BLUE_GAMEPAD, 0);
			BLUE_GAMEPAD = std::min(BLUE_GAMEPAD, (int)gamepadAxes.size() - 1);
			localState.push_back(gamepadAxes.at(BLUE_GAMEPAD));
			EU_GAMEPAD = std::max(EU_GAMEPAD, 0);
			EU_GAMEPAD = std::min(EU_GAMEPAD, (int)gamepadAxes.size() - 1);
			localState.push_back(gamepadAxes.at(EU_GAMEPAD));

			if (m_useSingleCfAxis) {
				CF_LEFT_GAMEPAD = std::max(CF_LEFT_GAMEPAD, 0);
				CF_LEFT_GAMEPAD = std::min(CF_LEFT_GAMEPAD, (int)gamepadAxes.size() - 1);
				float value = gamepadAxes.at(CF_LEFT_GAMEPAD);
				localState.push_back(-value);
				localState.push_back(value);
			} else {
				CF_LEFT_GAMEPAD = std::max(CF_LEFT_GAMEPAD, 0);
				CF_LEFT_GAMEPAD = std::min(CF_LEFT_GAMEPAD, (int)gamepadAxes.size() - 1);
				localState.push_back(gamepadAxes.at(CF_LEFT_GAMEPAD));
				CF_RIGHT_GAMEPAD = std::max(CF_RIGHT_GAMEPAD, 0);
				CF_RIGHT_GAMEPAD = std::min(CF_RIGHT_GAMEPAD, (int)gamepadAxes.size() - 1);
				localState.push_back(gamepadAxes.at(CF_RIGHT_GAMEPAD));
			}

			if (m_useSingleScrAxis) {
				SCR_DOWN_GAMEPAD = std::max(SCR_DOWN_GAMEPAD, 0);
				SCR_DOWN_GAMEPAD = std::min(SCR_DOWN_GAMEPAD, (int)gamepadAxes.size() - 1);
				float value = gamepadAxes.at(SCR_DOWN_GAMEPAD);
				localState.push_back(value);
				localState.push_back(-value);
			} else {
				SCR_DOWN_GAMEPAD = std::max(SCR_DOWN_GAMEPAD, 0);
				SCR_DOWN_GAMEPAD = std::min(SCR_DOWN_GAMEPAD, (int)gamepadAxes.size() - 1);
				localState.push_back(gamepadAxes.at(SCR_DOWN_GAMEPAD));
				SCR_UP_GAMEPAD = std::max(SCR_UP_GAMEPAD, 0);
				SCR_UP_GAMEPAD = std::min(SCR_UP_GAMEPAD, (int)gamepadAxes.size() - 1);
				localState.push_back(gamepadAxes.at(SCR_UP_GAMEPAD));
			}

			m_gpState = localState;
		}
	}
}

void Player::reset() {
	m_isRedPressed = false;
	m_isGreenPressed = false;
	m_isBluePressed = false;
	m_isUpPressed = false;
	m_isDownPressed = false;
	m_isCfGreenPressed = false;
	m_isCfBluePressed = false;
	m_isEuPressed = false;

	m_wasRedPressed = false;
	m_wasGreenPressed = false;
	m_wasBluePressed = false;
	m_wasUpPressed = false;
	m_wasDownPressed = false;
	m_wasCfGreenPressed = false;
	m_wasCfBluePressed = false;

	m_greenAnimation = false;
	m_redAnimation = false;
	m_blueAnimation = false;

	m_cross = 1;
	m_hasPlayerBrokeOnce = false;

	m_score = 0;
	m_mult = 1;
	m_combo = 0;
	m_scr_tick = 0;
	m_past_tap = -1;
	m_eu_value = 0;
	m_eu_zone_active = false;
	m_euphoria_active = false;
	m_double_mult = false;
	m_lastTapTime = 0.0;
	m_lastCrossTime = 0.0;
	m_genBpm = 0;
	m_highestCombo = 0;
}

//poll reset signals from generator
void Player::pollState(Generator& g) {
	if (g.m_combo_reset) {
		breakCombo(1.0);
	}
	if (g.m_eu_start) {
		m_eu_zone_active = true;
	}
	if (g.m_eu_check && m_eu_zone_active) {
		if (m_eu_value < 3.0) {
			m_eu_value += 1.0;
		}
	}
	m_genBpm = g.m_bpm;

	m_isGreenTapEnabled = g.m_isGreenTapEnabled;
	m_isRedTapEnabled = g.m_isRedTapEnabled;
	m_isBlueTapEnabled = g.m_isBlueTapEnabled;
}

//utility functions
std::vector<float> Player::getGamepadValues() {
	std::vector<float> localGamepadState = {};
	if (glfwJoystickPresent(m_gamepadId)) {
		int count;

		const unsigned char* buttons = glfwGetJoystickButtons(m_gamepadId, &count);
		for (int i = 0; i < count; ++i) {
			if (buttons[i] == '\0') {
				localGamepadState.push_back(0.0f);
			} else {
				localGamepadState.push_back(1.0f);
			}
		}
		const float* axes = glfwGetJoystickAxes(m_gamepadId, &count);
		for (int i = 0; i < count; ++i) {
			localGamepadState.push_back(axes[i]);
		}
	}
	return localGamepadState;
}

std::vector<float> Player::getKBMValues(GLFWwindow* w) {
	std::vector<float> localState;
	localState.resize(400);
	for (int i = GLFW_KEY_SPACE; i < GLFW_KEY_LAST; ++i) {
		if (glfwGetKey(w, i) != -1) {
			localState.at(i) = glfwGetKey(w, i);
		}
	}

	localState.at(0) = (m_nowMouseX - m_pastMouseX) / 100;
	localState.at(1) = (m_nowMouseY - m_pastMouseY) / 100;
	localState.at(2) = m_scrollX;
	localState.at(3) = m_scrollY;
	if (m_changedScroll) {
		m_changedScroll = false;
		m_scrollX = 0.0;
		m_scrollY = 0.0;
	}
	for (int i = GLFW_MOUSE_BUTTON_1; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
		localState.at(i + 4) = glfwGetMouseButton(w, i);
	}
	return localState;
}

bool Player::getRisingEdge(int index) {
	bool complete = true;
	for (auto& vec : m_gpHistory) {
		if (vec.size() < 8) {
			complete = false;
			break;
		}
	}
	if (m_gpState.size() >= 8 && m_gpMult.size() >= 8 && m_gpDead.size() >= 8 && m_gpHistory.size() >= m_historyLength && complete) {
		float val = m_gpState.at(index) * m_gpMult.at(index);
		float prev = m_gpHistory.back().at(index) * m_gpMult.at(index);
		float deadzone = m_gpDead.at(index);
		return (prev < deadzone && val >= deadzone);
	} else
		return false;
}

bool Player::getFallingEdge(int index) {
	if (!m_gpState.empty() && !m_gpMult.empty() && !m_gpDead.empty() && !m_gpHistory.empty()) {
		float val = m_gpState.at(index) * m_gpMult.at(index);
		float prev = m_gpHistory.back().at(index) * m_gpMult.at(index);
		float deadzone = m_gpDead.at(index);
		return (prev > deadzone && val <= deadzone);
	} else
		return false;
}

bool Player::getFallingZero(int index) {
	bool complete = true;
	for (auto& vec : m_gpHistory) {
		if (vec.size() < 8) {
			complete = false;
			break;
		}
	}
	if (complete && m_gpState.size() >= 8 && m_gpHistory.size() >= m_historyLength) {
		float minVelocity = -2.0;
		std::vector<float> averages;
		size_t i;
		for (i = 0; i <= m_gpHistory.size() - 3; ++i) {
			averages.push_back((m_gpHistory.at(i).at(index) + m_gpHistory.at(i + 1).at(index) + m_gpHistory.at(i + 2).at(index)) / 3);
		}
		averages.push_back((m_gpHistory.at(i).at(index) + m_gpHistory.at(i + 1).at(index) + m_gpState.at(index)) / 3);

		for (size_t i = averages.size() - 1; i >= 2; i++) {
			float v2 = averages.at(i) - averages.at(i - 1);
			float v1 = averages.at(i - 1) - averages.at(i - 2);
			return (v1 >= 0 && v2 < 0 && (v2 - v1) >= minVelocity);
		}
	} else
		return false;
}

bool Player::getHittableNote(int noteType, std::vector<Note>& array) {
	bool found = false;
	for (auto& note : array) {
		if (note.getHit() && !note.getTouched() && note.getType() == noteType) {
			found = true;
			break;
		}
	}
	return found;
}

int Player::getScore() {
	return m_score;
}

int Player::getCombo() {
	return m_combo;
}

int Player::getHighCombo() {
	return m_highestCombo;
}

int Player::getMult() {
	return m_mult;
}

int Player::getCross() {
	return m_cross;
}

bool Player::getBrokeOnce(){
	return m_hasPlayerBrokeOnce;
}

bool Player::getRedClicker() {
	return m_isRedPressed;
}

bool Player::getGreenClicker() {
	return m_isGreenPressed;
}

bool Player::getBlueClicker() {
	return m_isBluePressed;
}

double Player::getEuValue() {
	return m_eu_value;
}

bool Player::getEuActive() {
	return m_euphoria_active;
}
bool Player::getEuZoneActive() {
	return m_eu_zone_active;
}
