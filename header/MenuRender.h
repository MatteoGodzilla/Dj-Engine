#pragma once
#include "Game.h"
#include "Graphics.h"
#include "MenuNode.h"
#include "Rendr.h"
#include "SongScanner.h"

#include <GLFW/glfw3.h>
#include <array>
#include <iostream>
#include <map>

struct menuinputs {
	int* uk;
	int* dk;
	int* sk;
	int* bk;
	int* ug;
	int* dg;
	int* sg;
	int* bg;
};

class MenuRender : public Rendr {
public:
	void init(GLFWwindow* w);
	void tick();
	void render(MenuNode node, int selected, int vOffset);
	void remapping(Game* game, menuinputs input);
	//void scratches(Player* player);
	void calibration(Game* game, double dt);
	void setDeckSpeed(Game* game);
	void setLaneColors(Game* game);
	void setPollRate(Game* game);
	void result(Game* game);
	void splashArt();
	GLFWwindow* getWindowPtr();
	void doneEditing();
	void credits();

	const size_t VISIBLE_ENTRIES = 6;
	bool m_editingAxis = false;
	bool m_editingKey = false;
	int m_ActionToChange = -1;

	std::string m_testBuffer = "";

	bool m_shouldClose = false;

	double m_timeBeforeAnimating = 3.0f;
	double m_currentIdleTime = 0.0f;
	float m_selectionDX = 0.0f;

private:
	double m_cbPlayingTime = 0.0f;
	bool m_isCalibrating = false;
	ImFont* m_font;

	std::string m_inputSelection = "Keyboard";
	unsigned int m_buttonTexture = 0;
	unsigned int m_splashTexture = 0;
	unsigned int m_calibrationTex = 0;
		unsigned int m_pgBarFrame = 0;
		unsigned int m_menuVynil = 0;

	void editingAxisController(int axis);
	void editingAxisKBAM(int axis);

	std::vector<double> m_latencyHits;

	double m_dTime = 0.0;
	double m_globalTime = 0.0;
	double m_pastTime = 0.0;

	glm::vec2 m_logoDimensions;
	double m_deltaAngle = 0.004;
};
