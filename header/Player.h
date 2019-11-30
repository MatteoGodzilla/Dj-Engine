#pragma once
#include "GLFW/glfw3.h"
#include <vector>
#include "Generator.h"

enum indices {
	GREEN_INDEX,RED_INDEX,BLUE_INDEX,EU_INDEX,
	CF_LEFT_INDEX,CF_RIGHT_INDEX,
	SCR_UP_INDEX,SCR_DOWN_INDEX,
	MENU_UP,MENU_DOWN,MENU_SELECT,MENU_BACK
};

class Player {
public:
    Player();

	void pollInput(GLFWwindow* window);
	void hit(double time,std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& cross);

    virtual ~Player();
	bool getRedClicker();
	bool getGreenClicker();
	bool getBlueClicker();
	int getCross();
    int getScore();
    int getCombo();
	int getHighCombo();
    int getMult();
	double getEuValue();
	bool getEuActive();
	bool getEuZoneActive();
    void pollState(Generator &g);
	void updateGamepadState();
	std::vector<float> getGamepadValues();
	void tick(double time);

	int GREEN_CODE = GLFW_KEY_J;
	int RED_CODE = GLFW_KEY_K;
	int BLUE_CODE = GLFW_KEY_L;
	int CROSS_L_CODE = GLFW_KEY_A;
	int CROSS_R_CODE = GLFW_KEY_D;
	int SCRATCH_UP = GLFW_KEY_W;
	int SCRATCH_DOWN = GLFW_KEY_S;
	int EUPHORIA = GLFW_KEY_E;

	int GREEN_GAMEPAD = GLFW_GAMEPAD_BUTTON_A;
	int RED_GAMEPAD = GLFW_GAMEPAD_BUTTON_B;
	int BLUE_GAMEPAD = GLFW_GAMEPAD_BUTTON_X;
	int EU_GAMEPAD = GLFW_GAMEPAD_BUTTON_Y;

	int CF_LEFT_GAMEPAD = GLFW_GAMEPAD_AXIS_RIGHT_Y + GLFW_GAMEPAD_BUTTON_LAST;
	int CF_RIGHT_GAMEPAD = GLFW_GAMEPAD_AXIS_RIGHT_Y + GLFW_GAMEPAD_BUTTON_LAST + 1;
	int SCR_DOWN_GAMEPAD = GLFW_GAMEPAD_AXIS_LEFT_Y + GLFW_GAMEPAD_BUTTON_LAST;
	int SCR_UP_GAMEPAD = GLFW_GAMEPAD_AXIS_LEFT_Y + GLFW_GAMEPAD_BUTTON_LAST + 1;

	bool m_useSingleCfAxis = true;
	bool m_useSingleScrAxis = true;

	bool m_useKeyboardInput = true;

	std::vector<float> m_gpState;
	std::vector<float> m_gpMult;
	std::vector<float> m_gpDead;
	std::vector<bool> m_gpInvertDead;

	bool m_isRedPressed = false;
	bool m_isGreenPressed = false;
	bool m_isBluePressed = false;
	bool m_isUpPressed = false;
	bool m_isDownPressed = false;
	bool m_isCfLeftPressed = false;
	bool m_isCfRightPressed = false;
	bool m_isEuPressed = false;

	bool m_wasRedPressed = false;
	bool m_wasGreenPressed = false;
	bool m_wasBluePressed = false;
	bool m_wasUpPressed = false;
	bool m_wasDownPressed = false;
	bool m_wasCfLeftPressed = false;
	bool m_wasCfRightPressed = false;
protected:

private:
	int m_cross = 1;

    int m_score = 0;
    int m_mult = 1;
    int m_combo = 0;
    int m_scr_tick = 0;
    double m_past_tap = -1;
	double m_eu_value = 0;
	bool m_eu_zone_active = false;
	bool m_euphoria_active = false;
	bool m_double_mult = false;
	double m_lastTime = 0.0;
	int m_genBpm = 0;

	int m_highestCombo = 0;
};