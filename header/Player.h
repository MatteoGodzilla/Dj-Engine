// SPDX-FileCopyrightText: 2021 MatteoGodzilla
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once
#include "DJEUtils.h"
#include "Generator.h"
#include "SimpleIni.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <deque>
#include <fstream>
#include <vector>

enum indices {
	GREEN_INDEX,
	RED_INDEX,
	BLUE_INDEX,
	EU_INDEX,
	CF_LEFT_INDEX,
	CF_RIGHT_INDEX,
	SCR_UP_INDEX,
	SCR_DOWN_INDEX,
	MENU_UP,
	MENU_DOWN,
};

class Player {
public:
	Player();
	void pollInput(GLFWwindow* window, double time, std::vector<Note>& noteArr, std::vector<Note>& eventArr, std::vector<Note>& cross);
	void hit(double time, int noteType, std::vector<Note>& array);

	bool getRedClicker() const;
	bool getGreenClicker() const;
	bool getBlueClicker() const;
	int getCross() const;
	int getScore() const;
	int getCombo() const;
	int getHighCombo() const;
	int getMult() const;
	double getEuValue() const;
	bool getEuActive() const;
	bool getEuZoneActive() const;
	bool getBrokeOnce() const;
	void pollState(Generator& g);
	void readMappingFile();
	void writeMappingFile();
	void updateGamepadState();
	void updateKBMState(GLFWwindow* w);
	std::vector<float> getGamepadValues() const;
	std::vector<float> getKBMValues(GLFWwindow* w);
	bool getRisingEdge(int index);
	bool getFallingEdge(int index);
	bool getFallingZero(int index);
	bool getWaveTop(int index);
	bool getWaveRising(int index);
	static bool getHittableNote(int noteType, std::vector<Note>& array);
	static bool getHittableNoteAtZero(double time, int noteType, std::vector<Note>& array);
	static bool getScratchZoneActive(double time, int zoneType, std::vector<Note>& events);
	bool isAxisAboveDeadzone(int index) const;
	void tick(double time);
	void reset();

	int GREEN_CODE = GLFW_KEY_J;
	int RED_CODE = GLFW_KEY_K;
	int BLUE_CODE = GLFW_KEY_L;
	int CROSS_L_CODE = GLFW_KEY_A;
	int CROSS_R_CODE = GLFW_KEY_D;
	int SCRATCH_UP = GLFW_KEY_W;
	int SCRATCH_DOWN = GLFW_KEY_S;
	int EUPHORIA = GLFW_KEY_E;
	int UP_CODE = GLFW_KEY_W;
	int DOWN_CODE = GLFW_KEY_S;

	int GREEN_GAMEPAD = GLFW_GAMEPAD_BUTTON_A;
	int RED_GAMEPAD = GLFW_GAMEPAD_BUTTON_B;
	int BLUE_GAMEPAD = GLFW_GAMEPAD_BUTTON_X;
	int EU_GAMEPAD = GLFW_GAMEPAD_BUTTON_Y;

	int CF_LEFT_GAMEPAD = GLFW_GAMEPAD_AXIS_RIGHT_Y + GLFW_GAMEPAD_BUTTON_LAST;
	int CF_RIGHT_GAMEPAD = GLFW_GAMEPAD_AXIS_RIGHT_Y + GLFW_GAMEPAD_BUTTON_LAST + 1;
	int SCR_DOWN_GAMEPAD = GLFW_GAMEPAD_AXIS_LEFT_Y + GLFW_GAMEPAD_BUTTON_LAST;
	int SCR_UP_GAMEPAD = GLFW_GAMEPAD_AXIS_LEFT_Y + GLFW_GAMEPAD_BUTTON_LAST + 1;
	int UP_GAMEPAD = GLFW_GAMEPAD_BUTTON_DPAD_UP;
	int DOWN_GAMEPAD = GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
	bool m_useSingleCfAxis = true;
	bool m_useSingleScrAxis = true;
	bool m_digitalScratch = false;

	bool m_useKeyboardInput = true;

	int m_gamepadId = 1;

	std::vector<float> m_gpState;
	std::vector<float> m_gpMult;
	std::vector<float> m_gpDead;
	std::vector<bool> m_gpInvertDead;

	std::deque<std::vector<float>> m_gpHistory;
	size_t m_historyLength = 4;
	std::vector<float> m_pastKBMState;

	bool m_isRedPressed = false;
	bool m_isGreenPressed = false;
	bool m_isBluePressed = false;

	bool m_greenTimer = false;
	bool m_redTimer = false;
	bool m_blueTimer = false;
	bool m_cfCenterToGreen = false;
	bool m_cfCenterToBlue = false;
	bool m_cfGreenToCenter = false;
	bool m_cfBlueToCenter = false;
	bool m_wasLastNoteSpike = false;

	bool m_insideFSCross = false;
	int m_cross = 1;

	bool m_deltaMouse = false;
	double m_scrollX;
	double m_scrollY;
	bool m_changedScroll = false;

	bool m_isButtonsRight = false;
	bool m_botEnabled = false;

protected:
private:
	void breakCombo(double time);

	int m_score = 0;
	int m_mult = 1;
	int m_combo = 0;
	int m_scr_tick = 0;
	int m_cross_tick = 0;
	double m_past_tap = -1;
	double m_pastScratch = -1;
	double m_scratchDebounce = 0.5;
	double m_eu_value = 0;
	bool m_eu_zone_active = false;
	bool m_euphoria_active = false;
	bool m_double_mult = false;
	double m_lastTapTime = 0.0;
	double m_lastCrossTime = 0.0;
	float m_genBpm = 0;

	bool m_isGreenTapEnabled = true;
	bool m_isRedTapEnabled = true;
	bool m_isBlueTapEnabled = true;

	double m_pastMouseX = 0.0;
	double m_pastMouseY = 0.0;
	double m_nowMouseX = 0.0;
	double m_nowMouseY = 0.0;

	int m_highestCombo = 0;
	bool m_hasPlayerBrokeOnce = false;
};
