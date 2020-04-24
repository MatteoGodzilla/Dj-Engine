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

void Player::pollInput(GLFWwindow* window) {
	m_pastCross = m_cross;
	m_pastMouseX = m_nowMouseX;
	m_pastMouseY = m_nowMouseY;

	glfwGetCursorPos(window, &m_nowMouseX, &m_nowMouseY);
	if (m_deltaMouse) {
		int w, h;
		glfwGetWindowSize(window, &w, &h);
		glfwSetCursorPos(window, w / 2, h / 2);
	}

	//for exiting result screen with bot active
	m_wasGreenPressed = m_isGreenPressed;
	if (m_useKeyboardInput) {
		m_isGreenPressed = glfwGetKey(window, GREEN_CODE);
	} else {
		if (glfwJoystickPresent(m_gamepadId)) {
			updateGamepadState();

			if (m_gpState.size() > 0) {
				//value * sensitivity >= deadzone
				if (m_gpState.at(GREEN_INDEX) * m_gpMult.at(GREEN_INDEX) >= m_gpDead.at(GREEN_INDEX)) {
					m_isGreenPressed = true;
				} else
					m_isGreenPressed = false;
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

		if (m_gpState.size() > 0) {
			//value * sensitivity >= deadzone
			if (m_gpState.at(GREEN_INDEX) * m_gpMult.at(GREEN_INDEX) >= m_gpDead.at(GREEN_INDEX)) {
				m_isGreenPressed = true;
			} else
				m_isGreenPressed = false;

			//value * sensitivity >= deadzone
			if (m_gpState.at(RED_INDEX) * m_gpMult.at(RED_INDEX) >= m_gpDead.at(RED_INDEX)) {
				m_isRedPressed = true;
			} else
				m_isRedPressed = false;

			//value * sensitivity >= deadzone
			if (m_gpState.at(BLUE_INDEX) * m_gpMult.at(BLUE_INDEX) >= m_gpDead.at(BLUE_INDEX)) {
				m_isBluePressed = true;
			} else
				m_isBluePressed = false;

			//value * sensitivity >= deadzone
			if (m_gpState.at(EU_INDEX) * m_gpMult.at(EU_INDEX) >= m_gpDead.at(EU_INDEX)) {
				m_isEuPressed = true;
			} else
				m_isEuPressed = false;

			//value * sensitivity >= deadzone
			if (m_gpState.at(CF_LEFT_INDEX) * m_gpMult.at(CF_LEFT_INDEX) >= m_gpDead.at(CF_LEFT_INDEX)) {
				m_isCfGreenPressed = true;
				m_isCfBluePressed = false;
			} else
				m_isCfGreenPressed = false;

			//value * sensitivity >= deadzone
			if (m_gpState.at(CF_RIGHT_INDEX) * m_gpMult.at(CF_RIGHT_INDEX) >= m_gpDead.at(CF_RIGHT_INDEX)) {
				m_isCfBluePressed = true;
				m_isCfGreenPressed = false;
			} else
				m_isCfBluePressed = false;

			//value * sensitivity >= deadzone
			if (m_gpState.at(SCR_UP_INDEX) * m_gpMult.at(SCR_UP_INDEX) >= m_gpDead.at(SCR_UP_INDEX)) {
				m_isUpPressed = true;
				m_isDownPressed = false;
			} else
				m_isUpPressed = false;

			//value * sensitivity >= deadzone
			if (m_gpState.at(SCR_DOWN_INDEX) * m_gpMult.at(SCR_DOWN_INDEX) >= m_gpDead.at(SCR_DOWN_INDEX)) {
				m_isDownPressed = true;
				m_isUpPressed = false;
			} else
				m_isDownPressed = false;
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
			for (size_t i = 0; i < v.size(); ++i) {
				//if there is a note in the clicker, add score
				if (v.at(i).getHit() && !v.at(i).getTouched() && v.at(i).getType() == TAP_G) {
					found = true;
					m_score += 100 * m_mult;
					//check for chords (i.e multiple taps in the same time)
					if (v.at(i).getMilli() != m_past_tap) {
						m_combo++;
						m_past_tap = v.at(i).getMilli();
					}
					v.at(i).click(time);
					break;
				}
			}
			//loop for every event
			for (size_t j = 0; j < ev.size(); ++j) {
				if (ev.at(j).getHit() && ev.at(j).getType() == SCR_G_ZONE) {
					found = true;
					break;
				}
			}
			//if there isn't a note in the clicker, break the combo
			if (!found) {
				m_combo = 0;
				m_eu_zone_active = false;
			} else {
				m_greenAnimation = true; //start animation
			}
		}
	} else {
		if (m_isGreenPressed && !m_wasGreenPressed) {
			if (m_isUpPressed) {
				//loop for every event
				for (size_t i = 0; i < v.size(); ++i) {
					int type = v.at(i).getType();
					if ((type == SCR_G_UP || type == SCR_G_ANY) && v.at(i).getHit() && !v.at(i).getTouched()) {
						v.at(i).click(time);
						m_score += 100 * m_mult;
						m_combo++;
					}
				}
			} else if (m_isDownPressed) {
				//loop for every event
				for (size_t i = 0; i < v.size(); ++i) {
					int type = v.at(i).getType();
					if ((type == SCR_G_DOWN || type == SCR_G_ANY) && v.at(i).getHit() && !v.at(i).getTouched()) {
						v.at(i).click(time);
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
		for (size_t i = 0; i < v.size(); ++i) {
			//if there is a note in the clicker, add score
			if (v.at(i).getHit() && !v.at(i).getTouched() && v.at(i).getType() == TAP_R) {
				found = true;
				m_score += 100 * m_mult;
				if (v.at(i).getMilli() != m_past_tap) {
					m_combo++;
					m_past_tap = v.at(i).getMilli();
				}
				v.at(i).click(time);
				break;
			}
		}
		//if there isn't a note in the clicker, break combo
		if (!found) {
			m_combo = 0;
			m_eu_zone_active = false;
		} else {
			m_redAnimation = true; //start animation
		}
	}
	//blue pressed
	if (m_isBlueTapEnabled) {
		if (m_isBluePressed && !m_wasBluePressed) {
			bool found = false;
			//loop for every note
			for (size_t i = 0; i < v.size(); ++i) {
				//if there is a note in the clicker, add score
				if (v.at(i).getHit() && !v.at(i).getTouched() && v.at(i).getType() == TAP_B) {
					found = true;
					m_score += 100 * m_mult;
					if (v.at(i).getMilli() != m_past_tap) {
						m_combo++;
						m_past_tap = v.at(i).getMilli();
					}
					v.at(i).click(time);
					break;
				}
			}
			//loop for every event
			for (size_t j = 0; j < ev.size(); ++j) {
				if (ev.at(j).getHit() && ev.at(j).getType() == SCR_B_ZONE) {
					found = true;
					break;
				}
			}
			//if there isn't a note in the clicker, break combo
			if (!found) {
				m_combo = 0;
				m_eu_zone_active = false;
			} else {
				m_blueAnimation = true; // start animation
			}
		}
	} else {
		if (m_isBluePressed && !m_wasBluePressed) {
			if (m_isUpPressed) {
				//loop for every event
				for (size_t i = 0; i < v.size(); ++i) {
					int type = v.at(i).getType();
					if ((type == SCR_B_UP || type == SCR_B_ANY) && v.at(i).getHit() && !v.at(i).getTouched()) {
						v.at(i).click(time);
						m_score += 100 * m_mult;
						m_combo++;
					}
				}
			} else if (m_isDownPressed) {
				//loop for every event
				for (size_t i = 0; i < v.size(); ++i) {
					int type = v.at(i).getType();
					if ((type == SCR_B_DOWN || type == SCR_B_ANY) && v.at(i).getHit() && !v.at(i).getTouched()) {
						v.at(i).click(time);
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
		//loop for every event
		for (size_t i = 0; i < cross.size(); ++i) {
			//if there is a note in the clicker, add score
			int type = cross.at(i).getType();
			if (!cross.at(i).getTouched()) {
				if (cross.at(i).getHit()) {
					if (type == CROSS_G) {
						cross.at(i).click(time);
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
		for (size_t i = 0; i < v.size(); i++) {
			int type = v.at(i).getType();
			if (v.at(i).getHit() && !v.at(i).getTouched()) {
				if (type == CF_SPIKE_G) {
					v.at(i).click(time);
					found = true;
					break;
				}
			}
		}
		//if there isn't a note in the clicker, break combo
		if (!found) {
			m_combo = 0;
			m_eu_zone_active = false;
		}
	}
	//cross right pressed/moved
	if (m_isCfBluePressed && !m_wasCfBluePressed) {
		m_cfCenterToBlue = true;
		bool found = false;
		//loop for every event
		for (size_t i = 0; i < cross.size(); ++i) {
			//if there is a note in the clicker, add score
			int type = cross.at(i).getType();

			if (!cross.at(i).getTouched()) {
				if (cross.at(i).getHit()) {
					if (type == CROSS_B) {
						found = true;
						cross.at(i).click(time);
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

		for (size_t i = 0; i < v.size(); i++) {
			int type = v.at(i).getType();
			if (v.at(i).getHit() && !v.at(i).getTouched()) {
				if (type == CF_SPIKE_B) {
					v.at(i).click(time);
					found = true;
					break;
				}
			}
		}

		//if there isn't a note in the clicker, break combo
		if (!found) {
			m_combo = 0;
			m_eu_zone_active = false;
		}
	}
	//up pressed
	if (m_isUpPressed && !m_wasUpPressed) {
		//if the green button is already pressed
		if (m_isGreenPressed) {
			bool found = false;
			//loop for every note
			for (size_t i = 0; i < v.size(); ++i) {
				if (v.at(i).getHit() && !v.at(i).getTouched()) {
					int type = v.at(i).getType();
					//if there is a scratch up inside the clicker
					if (type == SCR_G_UP) {
						found = true;
						v.at(i).click(time);
						m_score += 100 * m_mult;
						m_combo++;
						break;
					}
					//if there is a scratch anydir inside the clicker
					else if (type == SCR_G_ANY || type && SCR_G_TICK) {
						if (!v.at(i).getTouched()) {
							found = true;
							v.at(i).click(time);
							m_score += 25 * m_mult;
							m_scr_tick++;
						}
					}
				}
			}
			//if there isn't a note in the clicker, break combo
			if (!found) {
				m_combo = 0;
				m_eu_zone_active = false;
			} else {
				m_greenAnimation = true;
			}
		}
		//if the blue button is already pressed
		if (m_isBluePressed) {
			bool found = false;
			//loop for every note
			for (size_t i = 0; i < v.size(); ++i) {
				if (v.at(i).getHit() && !v.at(i).getTouched() && !v.at(i).getTouched()) {
					int type = v.at(i).getType();
					//if there is a scratch up inside the clicker
					if (type == SCR_B_UP) {
						found = true;
						v.at(i).click(time);
						m_score += 100 * m_mult;
						m_combo++;
						break;
					}
					//if there is a scratch anydir inside the clicker
					else if (type == SCR_B_ANY || type == SCR_B_TICK) {
						found = true;
						v.at(i).click(time);
						m_score += 25 * m_mult;
						m_scr_tick++;
					}
				}
			}

			//if there isn't a note in the clicker, break combo
			if (!found) {
				m_combo = 0;
				m_eu_zone_active = false;
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
			for (size_t i = 0; i < v.size(); ++i) {
				if (v.at(i).getHit() && !v.at(i).getTouched()) {
					int type = v.at(i).getType();
					//if there is a scratch down inside the clicker
					if (type == SCR_G_DOWN) {
						found = true;
						v.at(i).click(time);
						m_score += 100 * m_mult;
						m_combo++;
						break;
					}
					//if there is a scratch anydir inside the clicker
					else if (type == SCR_G_ANY || type == SCR_G_TICK) {
						if (!v.at(i).getTouched()) {
							found = true;
							v.at(i).click(time);
							m_score += 25 * m_mult;
							m_scr_tick++;
						}
					}
				}
			}
			//if there isn't a note in the clicker, break combo
			if (!found) {
				m_combo = 0;
				m_eu_zone_active = false;
			} else {
				m_greenAnimation = true;
			}
		}
		//if the blue button is already pressed
		if (m_isBluePressed) {
			bool found = false;
			//loop for every note
			for (size_t i = 0; i < v.size(); ++i) {
				if (v.at(i).getHit() && !v.at(i).getTouched()) {
					int type = v.at(i).getType();
					//if there is a scratch down inside the clicker
					if (type == SCR_B_DOWN) {
						found = true;
						v.at(i).click(time);
						m_score += 100 * m_mult;
						m_combo++;
						break;
					}
					//if there is a scratch anydir inside the clicker
					else if (type == SCR_B_ANY || type == SCR_B_TICK) {
						found = true;
						v.at(i).click(time);
						m_score += 25 * m_mult;
						m_scr_tick++;
					}
				}
			}
			//if there isn't a note in the clicker, break combo
			if (!found) {
				m_combo = 0;
				m_eu_zone_active = false;
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
		for (size_t i = 0; i < v.size(); ++i) {
			if (v.at(i).getHit() && !v.at(i).getTouched() && !v.at(i).getTouched()) {
				int type = v.at(i).getType();
				if (type == SCR_G_TICK && m_isGreenPressed) {
					v.at(i).click(time);
					m_greenAnimation = true;
					m_score += 25 * m_mult;
					m_scr_tick++;
				} else if (type == SCR_B_TICK && m_isBluePressed) {
					v.at(i).click(time);
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
		//loop for every event
		for (size_t i = 0; i < cross.size(); ++i) {
			int type = cross.at(i).getType();
			if (!cross.at(i).getTouched()) {
				if (cross.at(i).getHit()) {
					if (type == CROSS_C) {
						cross.at(i).click(time);
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
		for (size_t i = 0; i < v.size(); i++) {
			int type = v.at(i).getType();
			if (v.at(i).getHit() && !v.at(i).getTouched()) {
				if (type == CF_SPIKE_C) {
					v.at(i).click(time);
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
			m_combo = 0;
			m_eu_zone_active = false;
		}
	}
	if (m_wasCfBluePressed && !m_isCfBluePressed) {
		m_cfBlueToCenter = true;
		bool found = false;
		//loop for every event
		for (size_t i = 0; i < cross.size(); ++i) {
			int type = cross.at(i).getType();
			if (!cross.at(i).getTouched()) {
				if (cross.at(i).getHit()) {
					if (type == CROSS_C) {
						cross.at(i).click(time);
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
		for (size_t i = 0; i < v.size(); i++) {
			int type = v.at(i).getType();
			if (v.at(i).getHit() && !v.at(i).getTouched()) {
				if (type == CF_SPIKE_C) {
					v.at(i).click(time);
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
			m_combo = 0;
			m_eu_zone_active = false;
		}
	}
	if (m_euphoria_active) {
		for (size_t i = 0; i < v.size(); ++i) {
			int type = v.at(i).getType();
			if (v.at(i).getMilli() < time && !v.at(i).getTouched()) {
				if (type == CF_SPIKE_G) {
					v.at(i).click(time);
					m_cfGreenToCenter = true;
					if (m_cross == 2) {
						m_cfCenterToBlue = true;
					}
					break;
				} else if (type == CF_SPIKE_B) {
					v.at(i).click(time);
					m_cfBlueToCenter = true;
					if (m_cross == 0) {
						m_cfCenterToGreen = true;
					}
					break;
				} else if (type == CF_SPIKE_C) {
					v.at(i).click(time);
					if (m_cross == 0) {
						m_cfCenterToGreen = true;
					} else if (m_cross == 2) {
						m_cfCenterToBlue = true;
					}
					break;
				}
			}
		}

		for (size_t i = 0; i < cross.size(); ++i) {
			int type = cross.at(i).getType();
			if (cross.at(i).getMilli() < time && cross.at(i).getMilli() > m_lastCrossTime && !cross.at(i).getTouched()) {
				if (type == CROSS_G) {
					m_cfCenterToGreen = true;

					m_cross = 0;
					cross.at(i).click(time);
					m_score += 100 * m_mult;
					m_combo++;
					m_lastCrossTime = cross.at(i).getMilli();
					break;
				} else if (type == CROSS_C) {
					if(m_cross == 0){
						m_cfGreenToCenter = true;
					}else{
						m_cfBlueToCenter = true;
					}
					m_cross = 1;
					cross.at(i).click(time);
					m_score += 100 * m_mult;
					m_combo++;
					m_lastCrossTime = cross.at(i).getMilli();
					break;
				} else if (type == CROSS_B) {
					m_cfCenterToBlue = true;

					m_cross = 2;
					cross.at(i).click(time);
					m_score += 100 * m_mult;
					m_combo++;
					m_lastCrossTime = cross.at(i).getMilli();
					break;
				}
			}
		}
	}
	if (m_botEnabled) {
		for (size_t i = 0; i < v.size(); ++i) {
			int type = v.at(i).getType();
			if (v.at(i).getMilli() < time && !v.at(i).getTouched()) {
				if (type == TAP_R) {
					m_score += 100 * m_mult;
					if (v.at(i).getMilli() != m_past_tap) {
						m_combo++;
						m_past_tap = v.at(i).getMilli();
					}
					v.at(i).click(time);
					m_redAnimation = true;
					break;
				} else if (type == TAP_G) {
					m_score += 100 * m_mult;
					//check for chords (i.e multiple taps in the same time)
					if (v.at(i).getMilli() != m_past_tap) {
						m_combo++;
						m_past_tap = v.at(i).getMilli();
					}
					v.at(i).click(time);
					m_greenAnimation = true;
					break;
				} else if (type == TAP_B) {
					m_score += 100 * m_mult;
					if (v.at(i).getMilli() != m_past_tap) {
						m_combo++;
						m_past_tap = v.at(i).getMilli();
					}
					v.at(i).click(time);
					m_blueAnimation = true;
					break;
				} else if (type == SCR_G_UP || type == SCR_G_DOWN) {
					v.at(i).click(time);
					m_greenAnimation = true;
					m_score += 100 * m_mult;
					m_combo++;
					break;
				} else if (type == SCR_G_ANY || type == SCR_G_TICK) {
					v.at(i).click(time);
					m_greenAnimation = true;
					m_score += 25 * m_mult;
					m_scr_tick++;
					break;
				} else if (type == SCR_B_UP || type == SCR_B_DOWN) {
					v.at(i).click(time);
					m_blueAnimation = true;
					m_score += 100 * m_mult;
					m_combo++;
					break;
				} else if (type == SCR_B_ANY || type == SCR_B_TICK) {
					v.at(i).click(time);
					m_blueAnimation = true;
					m_score += 25 * m_mult;
					m_scr_tick++;
					break;
				}

				else if (type == CF_SPIKE_G) {
					v.at(i).click(time);
					m_cfGreenToCenter = true;
					if (m_cross == 2) {
						m_cfCenterToBlue = true;
					}
					break;
				} else if (type == CF_SPIKE_B) {
					v.at(i).click(time);
					m_cfBlueToCenter = true;
					if (m_cross == 0) {
						m_cfCenterToGreen = true;
					}
					break;
				} else if (type == CF_SPIKE_C) {
					v.at(i).click(time);
					if (m_cross == 0) {
						m_cfCenterToGreen = true;
					} else if (m_cross == 2) {
						m_cfCenterToBlue = true;
					}
					break;
				}
			}
		}

		for (size_t i = 0; i < cross.size(); ++i) {
			int type = cross.at(i).getType();
			if (cross.at(i).getMilli() < time && cross.at(i).getMilli() > m_lastCrossTime && !cross.at(i).getTouched()) {
				if (type == CROSS_G) {
					if(m_cross != 0){
						m_cfCenterToGreen = true;
					}
					if(m_cross == 2){
						m_cfBlueToCenter = true;
					}
					m_cross = 0;
					cross.at(i).click(time);
					m_score += 100 * m_mult;
					m_combo++;
					m_lastCrossTime = cross.at(i).getMilli();
					break;
				} else if (type == CROSS_C) {
					if(m_cross == 0){
						m_cfGreenToCenter = true;
					}else if (m_cross == 2){
						m_cfBlueToCenter = true;
					}
					m_cross = 1;
					cross.at(i).click(time);
					m_score += 100 * m_mult;
					m_combo++;
					m_lastCrossTime = cross.at(i).getMilli();
					break;
				} else if (type == CROSS_B) {
					if(m_cross != 2){
						m_cfCenterToBlue = true;
					}
					if(m_cross == 0){
						m_cfGreenToCenter = true;
					}
					m_cross = 2;
					cross.at(i).click(time);
					m_score += 100 * m_mult;
					m_combo++;
					m_lastCrossTime = cross.at(i).getMilli();
					break;
				}
			}
		}
	}
}

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

void Player::readMappingFile() {
	std::ifstream input("config.txt");
	std::string s;
	while (s != "{Game}") {
		std::getline(input, s);
		if (input.eof()) {
			std::cerr << "Player Error: found config file, but not {Game} marker.";
			std::cerr << "Stopped loading of config file" << std::endl;
			return;
		}
	}
	if (input.is_open()) {
		std::cout << "Player Message: loading config from file" << std::endl;
		std::string token;
		input >> token;
		GREEN_CODE = std::stoi(token);
		input >> token;
		RED_CODE = std::stoi(token);
		input >> token;
		BLUE_CODE = std::stoi(token);
		input >> token;
		EUPHORIA = std::stoi(token);
		input >> token;
		CROSS_L_CODE = std::stoi(token);
		input >> token;
		CROSS_R_CODE = std::stoi(token);
		input >> token;
		SCRATCH_UP = std::stoi(token);
		input >> token;
		SCRATCH_DOWN = std::stoi(token);

		input >> token;
		GREEN_GAMEPAD = std::stoi(token);
		input >> token;
		RED_GAMEPAD = std::stoi(token);
		input >> token;
		BLUE_GAMEPAD = std::stoi(token);
		input >> token;
		EU_GAMEPAD = std::stoi(token);
		input >> token;
		m_useSingleCfAxis = token == "true" ? true : false;
		input >> token;
		CF_LEFT_GAMEPAD = std::stoi(token);
		input >> token;
		CF_RIGHT_GAMEPAD = std::stoi(token);
		input >> token;
		m_useSingleScrAxis = token == "true" ? true : false;
		input >> token;
		SCR_UP_GAMEPAD = std::stoi(token);
		input >> token;
		SCR_DOWN_GAMEPAD = std::stoi(token);

		for (int i = 0; i < 8; ++i) {
			input >> token;
			m_gpMult.at(i) = std::stof(token);
		}
		for (int i = 0; i < 8; ++i) {
			input >> token;
			m_gpDead.at(i) = std::stof(token);
		}
	} else {
		std::cerr << "Player Error: cannot open config file" << std::endl;
	}
	input.close();
}

void Player::writeMappingFile() {
	std::ofstream output("config.txt", std::ios::app);
	if (output.is_open()) {
		output << "{Game}" << std::endl;
		output << std::boolalpha;
		output << GREEN_CODE << std::endl;
		output << RED_CODE << std::endl;
		output << BLUE_CODE << std::endl;
		output << EUPHORIA << std::endl;
		output << CROSS_L_CODE << std::endl;
		output << CROSS_R_CODE << std::endl;
		output << SCRATCH_UP << std::endl;
		output << SCRATCH_DOWN << std::endl
			   << std::endl;
		;

		output << GREEN_GAMEPAD << std::endl;
		output << RED_GAMEPAD << std::endl;
		output << BLUE_GAMEPAD << std::endl;
		output << EU_GAMEPAD << std::endl;
		output << m_useSingleCfAxis << std::endl;
		output << CF_LEFT_GAMEPAD << std::endl;
		output << CF_RIGHT_GAMEPAD << std::endl;
		output << m_useSingleScrAxis << std::endl;
		output << SCR_UP_GAMEPAD << std::endl;
		output << SCR_DOWN_GAMEPAD << std::endl;

		for (size_t i = 0; i < m_gpMult.size(); ++i) {
			output << m_gpMult.at(i) << std::endl;
		}
		output << std::endl;
		for (size_t i = 0; i < m_gpDead.size(); ++i) {
			output << m_gpDead.at(i) << std::endl;
		}
		output << std::endl;

		std::cout << "Player Message: Written config to file" << std::endl;
	} else {
		std::cerr << "Player Error: couldn't create 'config.txt'" << std::endl;
	}
	output.close();
}

void Player::updateKBMState(GLFWwindow* w) {
	std::vector<float> KBMState = getKBMValues(w);
	m_gpState.clear();

	m_gpState.push_back(KBMState.at(GREEN_CODE));
	m_gpState.push_back(KBMState.at(RED_CODE));
	m_gpState.push_back(KBMState.at(BLUE_CODE));
	m_gpState.push_back(KBMState.at(EUPHORIA));
	m_gpState.push_back(KBMState.at(CROSS_L_CODE));
	m_gpState.push_back(KBMState.at(CROSS_R_CODE));
	m_gpState.push_back(KBMState.at(SCRATCH_UP));
	m_gpState.push_back(KBMState.at(SCRATCH_DOWN));

	/*if (glfwGetKey(w, GREEN_CODE))m_gpState.push_back(1.0f);
	else m_gpState.push_back(0.0f);
	if (glfwGetKey(w, RED_CODE))m_gpState.push_back(1.0f);
	else m_gpState.push_back(0.0f);
	if (glfwGetKey(w, BLUE_CODE))m_gpState.push_back(1.0f);
	else m_gpState.push_back(0.0f);
	if (glfwGetKey(w, EUPHORIA))m_gpState.push_back(1.0f);
	else m_gpState.push_back(0.0f);
	if (glfwGetKey(w, CROSS_L_CODE))m_gpState.push_back(1.0f);
	else m_gpState.push_back(0.0f);
	if (glfwGetKey(w, CROSS_R_CODE))m_gpState.push_back(1.0f);
	else m_gpState.push_back(0.0f);
	if (glfwGetKey(w, SCRATCH_UP))m_gpState.push_back(1.0f);
	else m_gpState.push_back(0.0f);
	if (glfwGetKey(w, SCRATCH_DOWN))m_gpState.push_back(1.0f);
	else m_gpState.push_back(0.0f);*/
}

void Player::updateGamepadState() {
	if (glfwJoystickPresent(m_gamepadId)) {
		int count;
		const unsigned char* buttons = glfwGetJoystickButtons(m_gamepadId, &count);

		std::vector<float> localGamepadState;

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

		if (count > 0) {
			m_gpState.clear();
			GREEN_GAMEPAD = std::max(GREEN_GAMEPAD, 0);
			GREEN_GAMEPAD = std::min(GREEN_GAMEPAD, (int)localGamepadState.size() - 1);
			m_gpState.push_back(localGamepadState.at(GREEN_GAMEPAD));
			RED_GAMEPAD = std::max(RED_GAMEPAD, 0);
			RED_GAMEPAD = std::min(RED_GAMEPAD, (int)localGamepadState.size() - 1);
			m_gpState.push_back(localGamepadState.at(RED_GAMEPAD));
			BLUE_GAMEPAD = std::max(BLUE_GAMEPAD, 0);
			BLUE_GAMEPAD = std::min(BLUE_GAMEPAD, (int)localGamepadState.size() - 1);
			m_gpState.push_back(localGamepadState.at(BLUE_GAMEPAD));
			EU_GAMEPAD = std::max(EU_GAMEPAD, 0);
			EU_GAMEPAD = std::min(EU_GAMEPAD, (int)localGamepadState.size() - 1);
			m_gpState.push_back(localGamepadState.at(EU_GAMEPAD));

			if (m_useSingleCfAxis) {
				CF_LEFT_GAMEPAD = std::max(CF_LEFT_GAMEPAD, 0);
				CF_LEFT_GAMEPAD = std::min(CF_LEFT_GAMEPAD, (int)localGamepadState.size() - 1);
				float value = localGamepadState.at(CF_LEFT_GAMEPAD);
				m_gpState.push_back(-value);
				m_gpState.push_back(value);
			} else {
				CF_LEFT_GAMEPAD = std::max(CF_LEFT_GAMEPAD, 0);
				CF_LEFT_GAMEPAD = std::min(CF_LEFT_GAMEPAD, (int)localGamepadState.size() - 1);
				m_gpState.push_back(localGamepadState.at(CF_LEFT_GAMEPAD));
				CF_RIGHT_GAMEPAD = std::max(CF_RIGHT_GAMEPAD, 0);
				CF_RIGHT_GAMEPAD = std::min(CF_RIGHT_GAMEPAD, (int)localGamepadState.size() - 1);
				m_gpState.push_back(localGamepadState.at(CF_RIGHT_GAMEPAD));
			}

			if (m_useSingleScrAxis) {
				SCR_DOWN_GAMEPAD = std::max(SCR_DOWN_GAMEPAD, 0);
				SCR_DOWN_GAMEPAD = std::min(SCR_DOWN_GAMEPAD, (int)localGamepadState.size() - 1);
				float value = localGamepadState.at(SCR_DOWN_GAMEPAD);
				m_gpState.push_back(value);
				m_gpState.push_back(-value);
			} else {
				SCR_DOWN_GAMEPAD = std::max(SCR_DOWN_GAMEPAD, 0);
				SCR_DOWN_GAMEPAD = std::min(SCR_DOWN_GAMEPAD, (int)localGamepadState.size() - 1);
				m_gpState.push_back(localGamepadState.at(SCR_DOWN_GAMEPAD));
				SCR_UP_GAMEPAD = std::max(SCR_UP_GAMEPAD, 0);
				SCR_UP_GAMEPAD = std::min(SCR_UP_GAMEPAD, (int)localGamepadState.size() - 1);
				m_gpState.push_back(localGamepadState.at(SCR_UP_GAMEPAD));
			}

			//debug
			/*
			std::cout << m_gpState.at(GREEN_GAMEPAD) * m_gpMult.at(GREEN_GAMEPAD) << "|";
			std::cout << m_gpState.at(RED_GAMEPAD) * m_gpMult.at(RED_GAMEPAD) << "|";
			std::cout << m_gpState.at(BLUE_GAMEPAD) * m_gpMult.at(BLUE_GAMEPAD) << "|";
			std::cout << m_gpState.at(EU_GAMEPAD) * m_gpMult.at(EU_GAMEPAD) << "|";
			std::cout << m_gpState.at(CF_LEFT_GAMEPAD) * m_gpMult.at(CF_LEFT_GAMEPAD) << "|";
			std::cout << m_gpState.at(CF_RIGHT_GAMEPAD-1) * m_gpMult.at(CF_RIGHT_GAMEPAD) << "|";
			std::cout << m_gpState.at(SCR_UP_GAMEPAD) * m_gpMult.at(SCR_UP_GAMEPAD) << "|";
			std::cout << m_gpState.at(SCR_DOWN_GAMEPAD-1) * m_gpMult.at(SCR_DOWN_GAMEPAD) << std::endl;
			*/
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
	m_pastCross = 1;

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
	if (g.m_combo_reset == true) {
		m_combo = 0;
		m_eu_zone_active = false;
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

Player::~Player() {
	//dtor
}
