#pragma once
#include "GLFW/glfw3.h"
#include <vector>
#include "Generator.h"

class Player {
public:
    Player();
	void keyCallback(int key, int action, double time,
		std::vector<Note>& v, std::vector<Note>& ev,std::vector<Note>& cross);
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

	int GREEN_CODE = GLFW_KEY_J;
	int RED_CODE = GLFW_KEY_K;
	int BLUE_CODE = GLFW_KEY_L;
	int CROSS_L_CODE = GLFW_KEY_A;
	int CROSS_R_CODE = GLFW_KEY_D;
	int SCRATCH_UP = GLFW_KEY_W;
	int SCRATCH_DOWN = GLFW_KEY_S;
	int EUPHORIA = GLFW_KEY_E;

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