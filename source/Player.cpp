// SPDX-FileCopyrightText: 2021 MatteoGodzilla
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Player.h"

#include <iostream>

Player::Player() {
	for (int i = 0; i < 10; ++i) {
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

void Player::pollInput(GLFWwindow* window, double time, std::vector<Note>& noteArr, std::vector<Note>& eventArr, std::vector<Note>& cross) {
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
	m_insideFSCross = false;

	for (auto& e : eventArr) {
		if (e.getType() == FS_CROSS_BASE && e.getMilli() <= time && e.getMilli() + e.getLength() > time) {
			m_insideFSCross = true;
		}
	}

	if (m_gpState.size() >= 10 && m_gpHistory.size() >= m_historyLength && !m_botEnabled) {
		if (getRisingEdge(GREEN_INDEX)) {
			if (getHittableNote(TAP_G, noteArr)) {
				hit(time, TAP_G, noteArr);
			} else if (getHittableNote(SCR_G_ZONE, eventArr)) {
				hit(time, SCR_G_ZONE, eventArr);
			} else {
				breakCombo(time);
			}
		}
		if (getRisingEdge(RED_INDEX)) {
			hit(time, TAP_R, noteArr);
		}
		if (getRisingEdge(BLUE_INDEX)) {
			if (getHittableNote(TAP_B, noteArr)) {
				hit(time, TAP_B, noteArr);
			} else if (getHittableNote(SCR_B_ZONE, eventArr)) {
				hit(time, SCR_B_ZONE, eventArr);
			} else {
				breakCombo(time);
			}
		}

		if (m_gpState.at(GREEN_INDEX) * m_gpMult.at(GREEN_INDEX) >= m_gpDead.at(GREEN_INDEX)) {
			//holding green
			if (getHittableNoteAtZero(time, TAP_G_HOLD_TICK, noteArr)) {
				hit(time, TAP_G_HOLD_TICK, noteArr);
			}
		}

		if (m_gpState.at(RED_INDEX) * m_gpMult.at(RED_INDEX) >= m_gpDead.at(RED_INDEX)) {
			//holding green
			if (getHittableNoteAtZero(time, TAP_R_HOLD_TICK, noteArr)) {
				hit(time, TAP_R_HOLD_TICK, noteArr);
			}
		}

		if (m_gpState.at(BLUE_INDEX) * m_gpMult.at(BLUE_INDEX) >= m_gpDead.at(BLUE_INDEX)) {
			//holding green
			if (getHittableNoteAtZero(time, TAP_B_HOLD_TICK, noteArr)) {
				hit(time, TAP_B_HOLD_TICK, noteArr);
			}
		}

		if (getRisingEdge(EU_INDEX)) {
			if (m_eu_value > 0.0) {
				m_euphoria_active = true;
			}
		}

		if (m_digitalScratch || m_useKeyboardInput) {
			// digital scratch
			//std::cout << "DIGITAL" << std::endl;
			if (getRisingEdge(SCR_UP_INDEX)) {
				if (getHittableNote(SCR_G_UP, noteArr) || getHittableNote(SCR_B_UP, noteArr)) {
					if (isAxisAboveDeadzone(GREEN_INDEX) && getScratchZoneActive(time, SCR_G_ZONE, eventArr)) {
						hit(time, SCR_G_UP, noteArr);
					} else if (isAxisAboveDeadzone(BLUE_INDEX) && getScratchZoneActive(time, SCR_B_ZONE, eventArr)) {
						hit(time, SCR_B_UP, noteArr);
					}
				} else {
					if (isAxisAboveDeadzone(GREEN_INDEX) && time - m_pastScratch > m_scratchDebounce) {
						hit(time, SCR_G_UP, noteArr);
					} else if (isAxisAboveDeadzone(BLUE_INDEX) && time - m_pastScratch > m_scratchDebounce) {
						hit(time, SCR_B_UP, noteArr);
					}
				}
			} else if (getRisingEdge(SCR_DOWN_INDEX)) {
				if (getHittableNote(SCR_G_DOWN, noteArr) || getHittableNote(SCR_B_DOWN, noteArr)) {
					if (isAxisAboveDeadzone(GREEN_INDEX) && getScratchZoneActive(time, SCR_G_ZONE, eventArr)) {
						hit(time, SCR_G_DOWN, noteArr);
					} else if (isAxisAboveDeadzone(BLUE_INDEX) && getScratchZoneActive(time, SCR_B_ZONE, eventArr)) {
						hit(time, SCR_B_DOWN, noteArr);
					}
				} else {
					if (time - m_pastScratch > m_scratchDebounce && (isAxisAboveDeadzone(GREEN_INDEX) || isAxisAboveDeadzone(BLUE_INDEX))) {
						breakCombo(time);
					}
				}
			}
		} else {
			//std::cout << "ANALOG" << std::endl;
			if (getFallingZero(SCR_UP_INDEX)) {
				if (getHittableNote(SCR_G_UP, noteArr) || getHittableNote(SCR_B_UP, noteArr)) {
					if (isAxisAboveDeadzone(GREEN_INDEX) && getScratchZoneActive(time, SCR_G_ZONE, eventArr)) {
						hit(time, SCR_G_UP, noteArr);
					} else if (isAxisAboveDeadzone(BLUE_INDEX) && getScratchZoneActive(time, SCR_B_ZONE, eventArr)) {
						hit(time, SCR_B_UP, noteArr);
					}
				} else {
					if (isAxisAboveDeadzone(GREEN_INDEX) && time - m_pastScratch > m_scratchDebounce) {
						hit(time, SCR_G_UP, noteArr);
					} else if (isAxisAboveDeadzone(BLUE_INDEX) && time - m_pastScratch > m_scratchDebounce) {
						hit(time, SCR_B_UP, noteArr);
					}
				}
			} else if (getFallingZero(SCR_DOWN_INDEX)) {
				if (getHittableNote(SCR_G_DOWN, noteArr) || getHittableNote(SCR_B_DOWN, noteArr)) {
					if (isAxisAboveDeadzone(GREEN_INDEX) && getScratchZoneActive(time, SCR_G_ZONE, eventArr)) {
						hit(time, SCR_G_DOWN, noteArr);
					} else if (isAxisAboveDeadzone(BLUE_INDEX) && getScratchZoneActive(time, SCR_B_ZONE, eventArr)) {
						hit(time, SCR_B_DOWN, noteArr);
					}
				} else {
					if (time - m_pastScratch > m_scratchDebounce && (isAxisAboveDeadzone(GREEN_INDEX) || isAxisAboveDeadzone(BLUE_INDEX))) {
						breakCombo(time);
					}
				}
			}
		}

		if (!m_euphoria_active) {
			//manual input by player

			float left = std::max(m_gpState.at(CF_LEFT_INDEX) * m_gpMult.at(CF_LEFT_INDEX), 0.0f);
			float right = std::max(m_gpState.at(CF_RIGHT_INDEX) * m_gpMult.at(CF_RIGHT_INDEX), 0.0f);

			float fpos = right - left;
			int crossNow = 1;
			if (fpos > 0.5) {
				crossNow = 2;
			} else if (fpos < -0.5) {
				crossNow = 0;
			}

			if (m_cross == 0) {
				if (crossNow == 1) {
					if (!m_insideFSCross) {
						if (getHittableNote(CF_SPIKE_C, noteArr)) {
							hit(time, CF_SPIKE_C, noteArr);
						} else if (!getHittableNote(CROSS_B, cross) && !getHittableNote(CF_SPIKE_B, noteArr)) {
							hit(time, CROSS_C, cross);
						}
					}
					m_cfGreenToCenter = true;
				}
			} else if (m_cross == 1) {
				if (crossNow == 0) {
					if (!m_insideFSCross) {
						if (getHittableNote(CF_SPIKE_G, noteArr)) {
							hit(time, CF_SPIKE_G, noteArr);
						} else {
							hit(time, CROSS_G, cross);
						}
					}
					m_cfCenterToGreen = true;
				} else if (crossNow == 2) {
					if (!m_insideFSCross) {
						if (getHittableNote(CF_SPIKE_B, noteArr)) {
							hit(time, CF_SPIKE_B, noteArr);
						} else {
							hit(time, CROSS_B, cross);
						}
					}

					m_cfCenterToBlue = true;
				}
			} else if (m_cross == 2) {
				if (crossNow == 1) {
					if (!m_insideFSCross) {
						if (getHittableNote(CF_SPIKE_C, noteArr)) {
							hit(time, CF_SPIKE_C, noteArr);
						} else if (!getHittableNote(CROSS_G, cross) && !getHittableNote(CF_SPIKE_G, noteArr)) {
							hit(time, CROSS_C, cross);
						}
					}

					m_cfBlueToCenter = true;
				}
			}

			m_cross = crossNow;

			if (getHittableNoteAtZero(time, CROSS_G_TICK, cross) && m_cross == 0) {
				hit(time, CROSS_G_TICK, cross);
			}

			if (getHittableNoteAtZero(time, CROSS_B_TICK, cross) && m_cross == 2) {
				hit(time, CROSS_B_TICK, cross);
			}
		} else {
			//euphoria enabled

			if (!m_insideFSCross) {
				if (getHittableNoteAtZero(time, CROSS_G, cross)) {
					hit(time, CROSS_G, cross);
					if (!m_wasLastNoteSpike) {
						if (m_cross == 2) {
							m_cfBlueToCenter = true;
						}
						m_cfCenterToGreen = true;
					}
					m_cross = 0;
					m_wasLastNoteSpike = false;
				} else if (getHittableNoteAtZero(time, CROSS_C, cross)) {
					hit(time, CROSS_C, cross);
					if (!m_wasLastNoteSpike) {
						if (m_cross == 0) {
							m_cfGreenToCenter = true;
						} else {
							m_cfBlueToCenter = true;
						}
					}
					m_cross = 1;
					m_wasLastNoteSpike = false;
				} else if (getHittableNoteAtZero(time, CROSS_B, cross)) {
					hit(time, CROSS_B, cross);
					if (!m_wasLastNoteSpike) {
						if (m_cross == 0) {
							m_cfGreenToCenter = true;
						}
						m_cfCenterToBlue = true;
					}
					m_cross = 2;
					m_wasLastNoteSpike = false;
				}

				if (getHittableNoteAtZero(time, CROSS_G_TICK, cross)) {
					hit(time, CROSS_G_TICK, cross);
					m_wasLastNoteSpike = false;
				}

				if (getHittableNoteAtZero(time, CROSS_B_TICK, cross)) {
					hit(time, CROSS_B_TICK, cross);
					m_wasLastNoteSpike = false;
				}

				if (getHittableNoteAtZero(time, CF_SPIKE_G, noteArr)) {
					hit(time, CF_SPIKE_G, noteArr);
					if (m_cross == 2) {
						m_cfCenterToBlue = true;
					}
					m_cfGreenToCenter = true;
					m_wasLastNoteSpike = true;
				} else if (getHittableNoteAtZero(time, CF_SPIKE_C, noteArr)) {
					hit(time, CF_SPIKE_C, noteArr);
					if (m_cross == 0) {
						m_cfCenterToGreen = true;
					} else {
						m_cfCenterToBlue = true;
					}
					m_wasLastNoteSpike = true;
				} else if (getHittableNoteAtZero(time, CF_SPIKE_B, noteArr)) {
					hit(time, CF_SPIKE_B, noteArr);
					if (m_cross == 0) {
						m_cfCenterToGreen = true;
					}
					m_cfBlueToCenter = true;
					m_wasLastNoteSpike = true;
				}
			} else {
				// Do not block user actions when inside freestyle crossfade
				float left = std::max(m_gpState.at(CF_LEFT_INDEX) * m_gpMult.at(CF_LEFT_INDEX), 0.0f);
				float right = std::max(m_gpState.at(CF_RIGHT_INDEX) * m_gpMult.at(CF_RIGHT_INDEX), 0.0f);

				float fpos = right - left;
				int crossNow = 1;
				if (fpos > 0.5) {
					crossNow = 2;
				} else if (fpos < -0.5) {
					crossNow = 0;
				}

				if (m_cross == 0) {
					if (crossNow == 1) {
						m_cfGreenToCenter = true;
					}
				} else if (m_cross == 1) {
					if (crossNow == 0) {
						m_cfCenterToGreen = true;
					} else if (crossNow == 2) {
						m_cfCenterToBlue = true;
					}
				} else if (m_cross == 2) {
					if (crossNow == 1) {
						m_cfBlueToCenter = true;
					}
				}

				m_cross = crossNow;
			}
		}

		//variables for GameRender
		m_isRedPressed = isAxisAboveDeadzone(RED_INDEX);
		m_isGreenPressed = isAxisAboveDeadzone(GREEN_INDEX);
		m_isBluePressed = isAxisAboveDeadzone(BLUE_INDEX);
	} else {
		//bot enabled
		if (getHittableNoteAtZero(time, TAP_G, noteArr)) {
			hit(time, TAP_G, noteArr);
		}

		if (getHittableNoteAtZero(time, TAP_R, noteArr)) {
			hit(time, TAP_R, noteArr);
		}
		if (getHittableNoteAtZero(time, TAP_B, noteArr)) {
			hit(time, TAP_B, noteArr);
		}

		if (getHittableNoteAtZero(time, CROSS_G, cross)) {
			hit(time, CROSS_G, cross);
			if (!m_wasLastNoteSpike) {
				if (m_cross == 2) {
					m_cfBlueToCenter = true;
				}
				m_cfCenterToGreen = true;
			}
			m_cross = 0;
			m_wasLastNoteSpike = false;
		} else if (getHittableNoteAtZero(time, CROSS_C, cross)) {
			hit(time, CROSS_C, cross);
			if (!m_wasLastNoteSpike) {
				if (m_cross == 0) {
					m_cfGreenToCenter = true;
				} else {
					m_cfBlueToCenter = true;
				}
			}
			m_cross = 1;
			m_wasLastNoteSpike = false;
		} else if (getHittableNoteAtZero(time, CROSS_B, cross)) {
			hit(time, CROSS_B, cross);
			if (!m_wasLastNoteSpike) {
				if (m_cross == 0) {
					m_cfGreenToCenter = true;
				}
				m_cfCenterToBlue = true;
			}
			m_cross = 2;
			m_wasLastNoteSpike = false;
		}

		if (getHittableNoteAtZero(time, CROSS_G_TICK, cross)) {
			hit(time, CROSS_G_TICK, cross);
			m_wasLastNoteSpike = false;
		}

		if (getHittableNoteAtZero(time, CROSS_B_TICK, cross)) {
			hit(time, CROSS_B_TICK, cross);
			m_wasLastNoteSpike = false;
		}

		if (getHittableNoteAtZero(time, CF_SPIKE_G, noteArr)) {
			hit(time, CF_SPIKE_G, noteArr);
			if (m_cross == 2) {
				m_cfCenterToBlue = true;
			}
			m_cfGreenToCenter = true;
			m_wasLastNoteSpike = true;
		} else if (getHittableNoteAtZero(time, CF_SPIKE_C, noteArr)) {
			hit(time, CF_SPIKE_C, noteArr);
			if (m_cross == 0) {
				m_cfCenterToGreen = true;
			} else {
				m_cfCenterToBlue = true;
			}
			m_wasLastNoteSpike = true;
		} else if (getHittableNoteAtZero(time, CF_SPIKE_B, noteArr)) {
			hit(time, CF_SPIKE_B, noteArr);
			if (m_cross == 0) {
				m_cfCenterToGreen = true;
			}
			m_cfBlueToCenter = true;
			m_wasLastNoteSpike = true;
		}

		if (getHittableNoteAtZero(time, SCR_G_UP, noteArr)) {
			hit(time, SCR_G_UP, noteArr);
		}
		if (getHittableNoteAtZero(time, SCR_G_DOWN, noteArr)) {
			hit(time, SCR_G_DOWN, noteArr);
		}
		if (getHittableNoteAtZero(time, SCR_G_ANY, noteArr)) {
			hit(time, SCR_G_ANY, noteArr);
		}
		if (getHittableNoteAtZero(time, SCR_G_TICK, noteArr)) {
			hit(time, SCR_G_TICK, noteArr);
		}

		if (getHittableNoteAtZero(time, SCR_B_UP, noteArr)) {
			hit(time, SCR_B_UP, noteArr);
		}
		if (getHittableNoteAtZero(time, SCR_B_DOWN, noteArr)) {
			hit(time, SCR_B_DOWN, noteArr);
		}
		if (getHittableNoteAtZero(time, SCR_B_ANY, noteArr)) {
			hit(time, SCR_B_ANY, noteArr);
		}
		if (getHittableNoteAtZero(time, SCR_B_TICK, noteArr)) {
			hit(time, SCR_B_TICK, noteArr);
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
			} else if (noteType == CROSS_G || noteType == CROSS_C || noteType == CROSS_B || noteType == CF_SPIKE_G || noteType == CF_SPIKE_C || noteType == CF_SPIKE_B) {
				m_score += 100 * m_mult;
				m_combo++;
			} else if (noteType == CROSS_G_TICK || noteType == CROSS_B_TICK) {
				m_score += 10 * m_mult;
				m_cross_tick++;
			} else if (noteType == SCR_G_UP || noteType == SCR_G_DOWN || noteType == SCR_B_UP || noteType == SCR_B_DOWN || noteType == SCR_G_ANY || noteType == SCR_G_TICK || noteType == SCR_B_ANY || noteType == SCR_B_TICK) {
				m_score += 100 * m_mult;
				m_combo++;

				m_pastScratch = note.getMilli();
			} else if (noteType == SCR_G_TICK || noteType == SCR_B_TICK) {
				m_score += 25 * m_mult;
				m_scr_tick++;
			} else if (noteType == TAP_G_HOLD_TICK || noteType == TAP_R_HOLD_TICK || noteType == TAP_B_HOLD_TICK) {
				m_score += 25 * m_mult;
			}
			note.click();

			if (noteType == TAP_G || noteType == TAP_G_HOLD_TICK || noteType == SCR_G_UP || noteType == SCR_G_DOWN || noteType == SCR_G_ANY || noteType == SCR_G_TICK) {
				m_greenTimer = true;
			} else if (noteType == TAP_R || noteType == TAP_R_HOLD_TICK) {
				m_redTimer = true;
			} else if (noteType == TAP_B || noteType == TAP_B_HOLD_TICK || noteType == SCR_B_UP || noteType == SCR_B_DOWN || noteType == SCR_B_ANY || noteType == SCR_B_TICK) {
				m_blueTimer = true;
			}

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
				m_greenTimer = true; //start animation
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
			m_redTimer = true; //start animation
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
				m_blueTimer = true; // start animation
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
				m_greenTimer = true;
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
				m_blueTimer = true;
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
				m_greenTimer = true;
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
				m_blueTimer = true;
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
					m_greenTimer = true;
					m_score += 25 * m_mult;
					m_scr_tick++;
				} else if (type == SCR_B_TICK && m_isBluePressed) {
					note.click();
					m_blueTimer = true;
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
					m_redTimer = true;
					break;
				} else if (type == TAP_G) {
					m_score += 100 * m_mult;
					//check for chords (i.e multiple taps in the same time)
					if (note.getMilli() != m_past_tap) {
						m_combo++;
						m_past_tap = note.getMilli();
					}
					note.click();
					m_greenTimer = true;
					break;
				} else if (type == TAP_B) {
					m_score += 100 * m_mult;
					if (note.getMilli() != m_past_tap) {
						m_combo++;
						m_past_tap = note.getMilli();
					}
					note.click();
					m_blueTimer = true;
					break;
				} else if (type == SCR_G_UP || type == SCR_G_DOWN) {
					note.click();
					m_greenTimer = true;
					m_score += 100 * m_mult;
					m_combo++;
					break;
				} else if (type == SCR_G_ANY || type == SCR_G_TICK) {
					note.click();
					m_greenTimer = true;
					m_score += 25 * m_mult;
					m_scr_tick++;
					break;
				} else if (type == SCR_B_UP || type == SCR_B_DOWN) {
					note.click();
					m_blueTimer = true;
					m_score += 100 * m_mult;
					m_combo++;
					break;
				} else if (type == SCR_B_ANY || type == SCR_B_TICK) {
					note.click();
					m_blueTimer = true;
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
	if (m_cross_tick >= 4) {
		m_cross_tick -= 4;
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

void Player::breakCombo(double time) {
	if (time > 0) {
		m_combo = 0;
		m_eu_zone_active = false;
		m_hasPlayerBrokeOnce = true;
	}
}

void Player::readMappingFile() {
	CSimpleIniA ini;
	const char* section = "Game Mappings";
	ini.LoadFile("profile.ini");

	GREEN_CODE = (int)ini.GetLongValue(section, "KB_green", 256);
	RED_CODE = (int)ini.GetLongValue(section, "KB_red", 256);
	BLUE_CODE = (int)ini.GetLongValue(section, "KB_blue", 256);
	EUPHORIA = (int)ini.GetLongValue(section, "KB_euphoria", 256);
	CROSS_L_CODE = (int)ini.GetLongValue(section, "KB_crossLeft", 256);
	CROSS_R_CODE = (int)ini.GetLongValue(section, "KB_crossRight", 256);
	SCRATCH_UP = (int)ini.GetLongValue(section, "KB_scratchUp", 256);
	SCRATCH_DOWN = (int)ini.GetLongValue(section, "KB_scratchDown", 256);
	UP_CODE = (int)ini.GetLongValue(section, "KB_menuUp", 256);
	DOWN_CODE = (int)ini.GetLongValue(section, "KB_menuDown", 256);

	GREEN_GAMEPAD = (int)ini.GetLongValue(section, "GP_green", 0);
	RED_GAMEPAD = (int)ini.GetLongValue(section, "GP_red", 0);
	BLUE_GAMEPAD = (int)ini.GetLongValue(section, "GP_blue", 0);
	EU_GAMEPAD = (int)ini.GetLongValue(section, "GP_euphoria", 0);
	m_useSingleCfAxis = ini.GetBoolValue(section, "GP_sameAxisForCrossfade", false);
	CF_LEFT_GAMEPAD = (int)ini.GetLongValue(section, "GP_crossLeft", 0);
	CF_RIGHT_GAMEPAD = (int)ini.GetLongValue(section, "GP_crossRight", 0);
	m_useSingleScrAxis = ini.GetBoolValue(section, "GP_sameAxisForScratch", false);
	m_digitalScratch = ini.GetBoolValue(section, "GP_digitalScratch", false);
	SCR_UP_GAMEPAD = (int)ini.GetLongValue(section, "GP_scratchUp", 0);
	SCR_DOWN_GAMEPAD = (int)ini.GetLongValue(section, "GP_scratchDown", 0);
	UP_GAMEPAD = (int)ini.GetLongValue(section, "GP_menuUp", 0);
	DOWN_GAMEPAD = (int)ini.GetLongValue(section, "GP_menuDown", 0);

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
	ini.SetLongValue(section, "KB_menuUp", UP_CODE);
	ini.SetLongValue(section, "KB_menuDown", DOWN_CODE);

	ini.SetLongValue(section, "GP_green", GREEN_GAMEPAD);
	ini.SetLongValue(section, "GP_red", RED_GAMEPAD);
	ini.SetLongValue(section, "GP_blue", BLUE_GAMEPAD);
	ini.SetLongValue(section, "GP_euphoria", EU_GAMEPAD);
	ini.SetBoolValue(section, "GP_sameAxisForCrossfade", m_useSingleCfAxis);
	ini.SetLongValue(section, "GP_crossLeft", CF_LEFT_GAMEPAD);
	ini.SetLongValue(section, "GP_crossRight", CF_RIGHT_GAMEPAD);
	ini.SetBoolValue(section, "GP_sameAxisForScratch", m_useSingleScrAxis);
	ini.SetBoolValue(section, "GP_digitalScratch", m_digitalScratch);
	ini.SetLongValue(section, "GP_scratchUp", SCR_UP_GAMEPAD);
	ini.SetLongValue(section, "GP_scratchDown", SCR_DOWN_GAMEPAD);
	ini.SetLongValue(section, "GP_menuUp", UP_GAMEPAD);
	ini.SetLongValue(section, "GP_menuDown", DOWN_GAMEPAD);

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
	localState.push_back(KBMState.at(UP_CODE));
	localState.push_back(KBMState.at(DOWN_CODE));

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
			UP_GAMEPAD = std::max(UP_GAMEPAD, 0);
			UP_GAMEPAD = std::min(UP_GAMEPAD, (int)gamepadAxes.size() - 1);
			localState.push_back(gamepadAxes.at(UP_GAMEPAD));
			DOWN_GAMEPAD = std::max(DOWN_GAMEPAD, 0);
			DOWN_GAMEPAD = std::min(DOWN_GAMEPAD, (int)gamepadAxes.size() - 1);
			localState.push_back(gamepadAxes.at(DOWN_GAMEPAD));

			m_gpState = localState;
		}
	}
}

void Player::reset() {
	m_isRedPressed = false;
	m_isGreenPressed = false;
	m_isBluePressed = false;

	m_greenTimer = false;
	m_redTimer = false;
	m_blueTimer = false;

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
std::vector<float> Player::getGamepadValues() const {
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
	if (m_gpState.size() >= 10 && m_gpMult.size() >= 10 && m_gpDead.size() >= 10 && m_gpHistory.size() >= m_historyLength && complete) {
		float val = m_gpState.at(index) * m_gpMult.at(index);
		float prev = m_gpHistory.back().at(index) * m_gpMult.at(index);
		float deadzone = m_gpDead.at(index);
		return (prev < deadzone && val >= deadzone);
	} else {
		return false;
	}
}

bool Player::getFallingEdge(int index) {
	if (!m_gpState.empty() && !m_gpMult.empty() && !m_gpDead.empty() && !m_gpHistory.empty()) {
		float val = m_gpState.at(index) * m_gpMult.at(index);
		float prev = m_gpHistory.back().at(index) * m_gpMult.at(index);
		float deadzone = m_gpDead.at(index);
		return (prev > deadzone && val <= deadzone);
	} else {
		return false;
	}
}

bool Player::getFallingZero(int index) {
	bool complete = true;
	for (auto& vec : m_gpHistory) {
		if (vec.size() < 8) {
			complete = false;
			break;
		}
	}
	if (complete && m_gpState.size() >= 10 && m_gpMult.size() >= 10 && m_gpHistory.size() >= m_historyLength) {
		float minVelocity = -0.025;
		std::vector<float> averages;
		size_t i;
		for (i = 0; i < m_gpHistory.size() - 2; ++i) {
			averages.push_back((m_gpHistory.at(i).at(index) + m_gpHistory.at(i + 1).at(index) + m_gpHistory.at(i + 2).at(index)) * m_gpMult.at(index) / 3);
		}
		//i is now 2
		averages.push_back((m_gpHistory.at(i).at(index) + m_gpHistory.at(i + 1).at(index) + m_gpState.at(index)) * m_gpMult.at(index) / 3);
		for (size_t i = averages.size() - 1; i >= 2; --i) {
			float v2 = averages.at(i) - averages.at(i - 1);
			float v1 = averages.at(i - 1) - averages.at(i - 2);
			if (v1 >= 0 && v2 < 0 && (v2 - v1) <= minVelocity) {
				return true;
			}
		}
		return false;
	} else {
		return false;
	}
}

bool Player::getWaveTop(int index) {
	bool complete = true;
	for (auto& vec : m_gpHistory) {
		if (vec.size() < 10) {
			complete = false;
			break;
		}
	}
	if (complete && m_gpState.size() >= 10 && m_gpMult.size() >= 10 && m_gpHistory.size() >= m_historyLength) {
		float deadzone = 0.0f;
		std::vector<float> averages;
		size_t i;
		for (i = 0; i < m_gpHistory.size() - 2; ++i) {
			averages.push_back((m_gpHistory.at(i).at(index) + m_gpHistory.at(i + 1).at(index) + m_gpHistory.at(i + 2).at(index)) * m_gpMult.at(index) / 3);
		}
		//i is now 2
		averages.push_back((m_gpHistory.at(i).at(index) + m_gpHistory.at(i + 1).at(index) + m_gpState.at(index)) * m_gpMult.at(index) / 3);
		for (size_t i = averages.size() - 1; i >= 2; --i) {
			float v2 = averages.at(i) - averages.at(i - 1);
			float v1 = averages.at(i - 1) - averages.at(i - 2);
			if (v1 * v2 < 0 && v2 - v1 <= deadzone) {
				return true;
			}
		}
		return false;
	} else {
		return false;
	}
}

bool Player::getWaveRising(int index) {
	bool complete = true;
	for (auto& vec : m_gpHistory) {
		if (vec.size() < 10) {
			complete = false;
			break;
		}
	}
	if (complete && m_gpState.size() >= 10 && m_gpMult.size() >= 8 && m_gpHistory.size() >= m_historyLength) {
		float deadzone = 0.05f;
		std::vector<float> averages;
		size_t i;
		for (i = 0; i < m_gpHistory.size() - 2; ++i) {
			averages.push_back((m_gpHistory.at(i).at(index) + m_gpHistory.at(i + 1).at(index) + m_gpHistory.at(i + 2).at(index)) * m_gpMult.at(index) / 3);
		}
		//i is now 2
		averages.push_back((m_gpHistory.at(i).at(index) + m_gpHistory.at(i + 1).at(index) + m_gpState.at(index)) * m_gpMult.at(index) / 3);
		for (size_t i = averages.size() - 1; i >= 2; --i) {
			float v2 = averages.at(i) - averages.at(i - 1);
			float v1 = averages.at(i - 1) - averages.at(i - 2);

			if (v2 - v1 >= deadzone) {
				return true;
			}
		}
		return false;
	} else {
		return false;
	}
}

bool Player::getHittableNote(int noteType, std::vector<Note>& array) {
	bool found = false;
	for (const auto& note : array) {
		if (note.getHit() && !note.getTouched() && note.getType() == noteType) {
			found = true;
			break;
		}
	}
	return found;
}

bool Player::getHittableNoteAtZero(double time, int noteType, std::vector<Note>& array) {
	bool found = false;
	for (const auto& note : array) {
		if (note.getMilli() < time && !note.getTouched() && note.getType() == noteType) {
			found = true;
			break;
		}
	}
	return found;
}

bool Player::getScratchZoneActive(double time, int zoneType, std::vector<Note>& events) {
	for (auto& ev : events) {
		if (ev.getTouched() && ev.getType() == zoneType && time >= ev.getMilli() && time < ev.getMilli() + ev.getLength()) {
			return true;
		}
	}
	return false;
}

bool Player::isAxisAboveDeadzone(int index) const {
	if (m_gpState.size() == 10 && m_gpMult.size() == 10 && m_gpDead.size() == 10) {
		return m_gpState.at(index) * m_gpMult.at(index) >= m_gpDead.at(index);
	} else {
		return false;
	}
}

int Player::getScore() const {
	return m_score;
}

int Player::getCombo() const {
	return m_combo;
}

int Player::getHighCombo() const {
	return m_highestCombo;
}

int Player::getMult() const {
	return m_mult;
}

int Player::getCross() const {
	return m_cross;
}

bool Player::getBrokeOnce() const {
	return m_hasPlayerBrokeOnce;
}

bool Player::getRedClicker() const {
	return m_isRedPressed;
}

bool Player::getGreenClicker() const {
	return m_isGreenPressed;
}

bool Player::getBlueClicker() const {
	return m_isBluePressed;
}

double Player::getEuValue() const {
	return m_eu_value;
}

bool Player::getEuActive() const {
	return m_euphoria_active;
}
bool Player::getEuZoneActive() const {
	return m_eu_zone_active;
}
