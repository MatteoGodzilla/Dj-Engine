#pragma once

// clang-format off
#include "Rendr.h"
// clang-format on
#include "AnimationManager.h"
#include "Generator.h"
#include "Note.h"
#include "Player.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <math.h>
#include <string>
#include <vector>

enum AtlasIndices {
	TAP_GREEN_1,
	TAP_GREEN_2,
	TAP_GREEN_3,
	TAP_GREEN_4,
	TAP_RED_1,
	TAP_RED_2,
	TAP_RED_3,
	TAP_RED_4,
	TAP_BLUE_1,
	TAP_BLUE_2,
	TAP_BLUE_3,
	TAP_BLUE_4,
	TAP_EUPHORIA_1,
	TAP_EUPHORIA_2,
	TAP_EUPHORIA_3,
	TAP_EUPHORIA_4,
	CLICKER_GREEN,
	CLICKER_RED,
	CLICKER_BLUE,
	CLICKER_TRAY,
	SCRATCH_ANYDIR,
	SCRATCH_UP,
	SCRATCH_DOWN
};

class GameRender : public Rendr {
public:
	GameRender();
	void init(GLFWwindow* w);
	void highway(double time);
	void clicker();
	void notes(double time, std::vector<Note>& v, std::vector<Note>& cross);
	void events(double time, std::vector<Note>& ev, std::vector<Note>& cross);
	void lanes(double time, std::vector<Note>& v, std::vector<Note>& ev);
	void bpmTicks(double time, std::vector<double>& bpm_arr);
	void clickerAnimation();
	void result(Player& player, Generator& generator);
	void meters(double time);
	void pollState(double time, Player& p, Generator& g);
	void updateAnimations(double time);
	void debug(double deltaTime, std::vector<Note>& note, std::vector<Note>& ev, std::vector<Note>& c);
	void reset();
	virtual ~GameRender();

	bool m_isButtonsRight = false;
	float m_noteVisibleTime = 1.0;

private:
	std::vector<Note> getCrossInsideNote(Note& note, std::vector<Note> crossArr);
	glm::vec2 getCirclePoint(double radius, double angle);
	int getCrossAtTime(double time, std::vector<Note> crossArr);

	bool m_red = false, m_green = false, m_blue = false;
	int m_playerCross = 1;
	int m_playerCombo = 0;
	int m_playerMult = 0;
	int m_playerScore = 0;
	int m_renderCross = 1;
	double m_renderEuValue = 0.0;
	bool m_renderEuActive = false;
	bool m_renderEuZone = false;

	glm::mat4 m_persProj = glm::mat4(1.0f);
	glm::mat4 m_orthoProj = glm::mat4(1.0f);

	unsigned int m_textureProgram = 0;
	unsigned int m_colorProgram = 0;
	unsigned int m_textProgram = 0;
	unsigned int m_highwayTexture = 0;
	unsigned int m_objTexture = 0;
	unsigned int m_metersTexture = 0;
	unsigned int m_clickerAnimation = 0;
	unsigned int m_pgBarFrame = 0;
	unsigned int m_pgBarInside = 0;
	unsigned int m_fontTexture = 0;
	unsigned int m_textureVAO = 0;
	unsigned int m_textureVBO = 0;
	unsigned int m_colorVAO = 0;
	unsigned int m_colorVBO = 0;

	FT_Library m_FTLibrary;
	FT_Face m_font;

	AnimationManager m_animManager;

	std::map<int, glm::vec4> m_objectAtlas;

	//base curve information
	float m_radius = 10.0;
	float m_deltaRadius = 1.5;
	float m_deltaAngle = 0.004;
	float m_maxAngle = glm::quarter_pi<float>();

	//default green clicker position
	float m_greenLeft = 0.0f;
	float m_greenRight = 0.0f;
	float m_back = 0.0f;
	float m_front = 0.0f;

	//default blue clicker position
	float m_blueLeft = 0.0f;
	float m_blueRight = 0.0f;

	int m_genBaseScore = 0;
};
