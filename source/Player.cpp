#include "Player.h"
#include <iostream>

Player::Player() {
}

void Player::pollInput(GLFWwindow* window){
	m_wasRedPressed = m_isRedPressed;
	m_wasGreenPressed = m_isGreenPressed;
	m_wasBluePressed = m_isBluePressed;
	m_wasUpPressed = m_isUpPressed;
	m_wasDownPressed = m_isDownPressed;
	m_wasCfLeftPressed = m_isCfLeftPressed;
	m_wasCfRightPressed = m_isCfRightPressed;

	m_isRedPressed = glfwGetKey(window, RED_CODE);
	m_isGreenPressed = glfwGetKey(window, GREEN_CODE);
	m_isBluePressed = glfwGetKey(window, BLUE_CODE);
	m_isUpPressed = glfwGetKey(window, SCRATCH_UP);
	m_isDownPressed = glfwGetKey(window, SCRATCH_DOWN);
	m_isCfLeftPressed = glfwGetKey(window, CROSS_L_CODE);
	m_isCfRightPressed = glfwGetKey(window, CROSS_R_CODE);
	m_isEuPressed = glfwGetKey(window, EUPHORIA);

	if (m_isCfLeftPressed && !m_wasCfLeftPressed) {
		m_cross = 0;
	}
	else if (m_isCfRightPressed && !m_wasCfRightPressed) {
		m_cross = 2;
	}
	else if (m_wasCfLeftPressed && !m_isCfLeftPressed && !m_isCfRightPressed) {
		m_cross = 1;
	}
	else if (m_wasCfRightPressed && !m_isCfRightPressed && !m_isCfLeftPressed) {
		m_cross = 1;
	}
}

void Player::hit(double time, std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& cross){
	//green pressed
	if (m_isGreenPressed && !m_wasGreenPressed) {
		bool found = false;
		//loop for every note 
		for (size_t i = 0; i < v.size(); ++i) {
			//if there is a note in the clicker, add score
			if (v.at(i).getHit() && v.at(i).getType() == TAP_G) {
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
			if (ev.at(j).getHit() && ev.at(j).getType() == SCR_G_ZONE) {
				found = true;
				break;
			}
		}
		//if there isn't a note in the clicker, break the combo
		if (!found) {
			m_combo = 0;
			m_eu_zone_active = false;
		}
	}
	//red pressed
	if (m_isRedPressed && !m_wasRedPressed) {
		bool found = false;
		//loop for every note 
		for (size_t i = 0; i < v.size(); ++i) {
			//if there is a note in the clicker, add score
			if (v.at(i).getHit() && v.at(i).getType() == TAP_R) {
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
		}
	}
	//blue pressed
	if (m_isBluePressed && !m_wasBluePressed) {
		bool found = false;
		//loop for every note 
		for (size_t i = 0; i < v.size(); ++i) {
			//if there is a note in the clicker, add score
			if (v.at(i).getHit() && v.at(i).getType() == TAP_B) {
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
		}
	}
	//cross left pressed/moved
	if (m_isCfLeftPressed && !m_wasCfLeftPressed) {
		bool found = false;
		//loop for every event
		for (size_t i = 0; i < cross.size(); ++i) {
			//if there is a note in the clicker, add score
			int type = cross.at(i).getType();
			if (cross.at(i).getHit()) {
				if (type == CROSS_G) {
					cross.at(i).click(time);
					m_score += 100 * m_mult;
					m_combo++;
					found = true;
					break;
				}
			}
		}
		for (size_t i = 0; i < v.size(); i++) {
			int type = v.at(i).getType();
			if (v.at(i).getHit()) {
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
	if (m_isCfRightPressed && !m_wasCfRightPressed) {
		bool found = false;
		//loop for every event
		for (size_t i = 0; i < cross.size(); ++i) {
			//if there is a note in the clicker, add score
			int type = cross.at(i).getType();
			if (cross.at(i).getHit()) {
				if (type == CROSS_B) {
					found = true;
					cross.at(i).click(time);
					m_score += 100 * m_mult;
					m_combo++;
					break;
				}
			}
		}

		for (size_t i = 0; i < v.size(); i++) {
			int type = v.at(i).getType();
			if (v.at(i).getHit()) {
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
		bool found = false;
		//if the green button is already pressed
		if (m_isGreenPressed) {
			//loop for every note
			for (size_t i = 0; i < v.size(); ++i) {
				if (v.at(i).getHit()) {
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
					else if (type == SCR_G_ANY) {
						found = true;
						v.at(i).click(time);
						m_score += 25 * m_mult;
						m_scr_tick++;
					}
				}
			}

		}
		//if the blue button is already pressed
		if (m_isBluePressed) {
			//loop for every note
			for (size_t i = 0; i < v.size(); ++i) {
				if (v.at(i).getHit()) {
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
					else if (type == SCR_B_ANY) {
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
		}
	}
	//down pressed
	if (m_isDownPressed && !m_wasDownPressed) {
		bool found = false;
		//if the green button is already pressed
		if (m_isGreenPressed) {
			//loop for every note
			for (size_t i = 0; i < v.size(); ++i) {
				if (v.at(i).getHit()) {
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
					else if (type == SCR_G_ANY) {
						found = true;
						v.at(i).click(time);
						m_score += 25 * m_mult;
						m_scr_tick++;
					}
				}
			}
		}
		//if the blue button is already pressed
		if (m_isBluePressed) {
			//loop for every note
			for (size_t i = 0; i < v.size(); ++i) {
				if (v.at(i).getHit()) {
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
					else if (type == SCR_B_ANY) {
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
		}
	}
	if (m_isEuPressed) {
		//activate euphoria if there is some
		if (m_eu_value > 0.0) {
			m_euphoria_active = true;
		}
	}
	//release section
	//cross left released
	if (m_wasCfLeftPressed && !m_isCfLeftPressed) {
		bool found = false;
		//loop for every event
		for (size_t i = 0; i < cross.size(); ++i) {
			int type = cross.at(i).getType();
			if (cross.at(i).getHit()) {
				if (type == CROSS_C) {
					cross.at(i).click(time);
					m_score += 100 * m_mult;
					m_combo++;
					found = true;
					break;
				}
				if (type == CROSS_B) {
					found = true;
					break;
				}
			}
		}

		//loop for every note
		for (size_t i = 0; i < v.size(); i++) {
			int type = v.at(i).getType();
			if (v.at(i).getHit()) {
				if (type == CF_SPIKE_C) {
					v.at(i).click(time);
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
	if (m_wasCfRightPressed && !m_isCfRightPressed) {
		bool found = false;
		//loop for every event
		for (size_t i = 0; i < cross.size(); ++i) {
			int type = cross.at(i).getType();
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
		}

		//loop for every note
		for (size_t i = 0; i < v.size(); i++) {
			int type = v.at(i).getType();
			if (v.at(i).getHit()) {
				if (type == CF_SPIKE_C) {
					v.at(i).click(time);
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
}

//update combo/multiplier for every frame
void Player::tick(double time) {
	if (m_combo >= 24)m_mult = 4;
	else if (m_combo >= 16 && m_combo < 24) m_mult = 3;
	else if (m_combo >= 8 && m_combo < 16) m_mult = 2;
	else m_mult = 1;

	if (m_scr_tick >= 4) {
		m_scr_tick = 0;
		m_combo++;
	}

	//decrease euphoria if active
	if (m_euphoria_active) {
		if (m_eu_value < 0.0)m_euphoria_active = false;
		else {
			m_double_mult = true;
			double dt = time - m_lastTime;
			double de = (dt * m_genBpm) / 960;
			m_eu_value -= de;
		}
	}
	else {
		m_double_mult = false;
	}

	if (m_double_mult)m_mult *= 2;
	m_lastTime = time;

	if (m_combo > m_highestCombo)m_highestCombo = m_combo;
}

//poll reset signals from generator 
void Player::pollState(Generator &g){
    if(g.m_combo_reset == true){
        m_combo = 0;
		m_eu_zone_active = false;
    }
	if (g.m_eu_start) {
		m_eu_zone_active = true;
	}
	if (g.m_eu_check && m_eu_zone_active) {
		if(m_eu_value < 3.0)m_eu_value += 1.0;
	}
	m_genBpm = g.m_bpm;
}

//utility functions
int Player::getScore(){
    return m_score;
}

int Player::getCombo(){
    return m_combo;
}

int Player::getHighCombo() {
	return m_highestCombo;
}

int Player::getMult(){
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
