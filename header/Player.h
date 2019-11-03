#pragma once
#include "GLFW/glfw3.h"
#include <vector>
#include "Generator.h"

enum K {
    GREEN_CODE = GLFW_KEY_J,
    RED_CODE = GLFW_KEY_K,
    BLUE_CODE = GLFW_KEY_L,
    CROSS_L_CODE = GLFW_KEY_A,
    CROSS_R_CODE = GLFW_KEY_D,
    SCRATCH_UP = GLFW_KEY_W,
    SCRATCH_DOWN = GLFW_KEY_S,
	EUPHORIA = GLFW_KEY_E
};

class Player {
public:
    Player();
	void keyCallback(int key, int action, 
		double time, std::vector<Note>& v, std::vector<Note>& ev);
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
	void tick(double time);
protected:

private:
	bool m_red = false;
	bool m_green = false;
	bool m_blue = false;
	int m_cross = 1;
    bool m_scr_up = false;
    bool m_scr_down = false;
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