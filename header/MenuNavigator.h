#pragma once
#include "MenuNode.h"
#include "MenuRender.h"
#include "SimpleIni.h"
#include "SongScanner.h"

#include <GLFW/glfw3.h>
#include <algorithm>
#include <fstream>
#include <iostream>

enum scenes {
	MAIN_SCENE,
	REMAPPING,
	CREDITS,
	SCRATCHES,
	CALIBRATION,
	RESULTS
};

enum actions {
	ROOT_ID,
	PLAY_ID,
	OPTIONS_ID,
	CREDITS_ID,
	EXIT_ID,
	SCRATCHES_ID,
	LATENCY_ID,
	LR_BUTTONS_ID,
	SPEED_ID,
	BOT_ID,
	DEBUG_ID,
	REFRESH_ID,
	COLOR_ID,
	POLL_ID,
	SONG_GENERAL_ID,
	SONG_EXPERT_ID,
	SONG_HARD_ID,
	SONG_MEDIUM_ID,
	SONG_EASY_ID,
	SONG_BEGINNER_ID,
	DONT_CARE
};

enum popupId {
	HIGHWAY_SPEED,
	LANE_COLORS,
	POLLRATE_CHANGE
};

class MenuNavigator {
public:
	void init(GLFWwindow* w, Game* gameptr);
	void pollInput();
	void update();
	void render();
	void setActive(bool active);
	bool getActive() const;
	void activate(MenuNode& menu);
	void scan(bool useCache = true);
	bool getShouldClose() const;

	int m_scene = 0;

private:
	void remap();
	void updateGamepadState();
	void updateMenuNode();
	void resetMenu();
	void writeConfigFile();
	//void readConfigFile();

	//gamepad status
	std::vector<float> m_gpState;
	std::vector<float> m_gpDead;

	//variables used to detect press event
	bool m_isUpPressed = false;
	bool m_isDownPressed = false;
	bool m_isSelectPressed = false;
	bool m_isBackPressed = false;

	bool m_wasUpPressed = false;
	bool m_wasDownPressed = false;
	bool m_wasSelectPressed = false;
	bool m_wasBackPressed = false;

	bool m_isEscapePressed = false;
	bool m_wasEscapePressed = false;
	bool m_wasTabPressed = false;
	bool m_isTabPressed = false;

	int m_holdingSomething = -1;
	double m_holdingFor = 0.0;
	double m_holdingThreshold = 1.0;
	double m_holdingRepeatTime = 0.05;

	int m_viewOffset = 0;
	bool m_shouldClose = false;
	bool m_active = false;

	double m_dTime = 0.0;
	double m_pastTime = 0.0;

	//menu structure
	MenuNode m_root = MenuNode("Main Menu", 0);
	MenuNode* m_activeNode = &m_root;
	std::vector<unsigned int> m_selection;

	MenuRender m_render;

	std::vector<SongEntry> m_songList = {};
	Game* m_game = nullptr;

	std::vector<float> m_pastGamepadValues;
	std::vector<float> m_pastKBMState;

	int m_popupId = -1;
	bool m_debounce = false;
};
