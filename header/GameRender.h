#pragma once
#include "DJEUtils.h"
// clang-format off
#include "Rendr.h"
// clang-format on
#include "TimerManager.h"
#include "Generator.h"
#include "Note.h"
#include "Player.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <math.h>
#include <string>
#include <vector>

enum AnimId {
	AN_CROSS_GREEN_TO_LEFT,
	AN_CROSS_GREEN_TO_CENTER,
	AN_CROSS_BLUE_TO_RIGHT,
	AN_CROSS_BLUE_TO_CENTER,
	AN_GREEN_CLICKER,
	AN_RED_CLICKER,
	AN_BLUE_CLICKER,
};

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
	TRAIL_GREEN_MIDDLE,
	TRAIL_GREEN_END,
	TRAIL_RED_MIDDLE,
	TRAIL_RED_END,
	TRAIL_BLUE_MIDDLE,
	TRAIL_BLUE_END,
	TRAIL_EUPHORIA_MIDDLE,
	TRAIL_EUPHORIA_END,
	CLICKER_GREEN,
	CLICKER_RED,
	CLICKER_BLUE,
	CLICKER_TRAY,
	SCRATCH_ANYDIR,
	SCRATCH_UP,
	SCRATCH_DOWN,
	FS_CROSS_GREEN_BASE_BOTTOM,
	FS_CROSS_GREEN_BASE_TOP,
	FS_CROSS_BLUE_BASE_BOTTOM,
	FS_CROSS_BLUE_BASE_TOP,
	FS_CROSS_SPEAKER_GREEN_0,
	FS_CROSS_SPEAKER_GREEN_1,
	FS_CROSS_SPEAKER_GREEN_2,
	FS_CROSS_SPEAKER_GREEN_3,
	FS_CROSS_SPEAKER_GREEN_4,
	FS_CROSS_SPEAKER_BLUE_0,
	FS_CROSS_SPEAKER_BLUE_1,
	FS_CROSS_SPEAKER_BLUE_2,
	FS_CROSS_SPEAKER_BLUE_3,
	FS_CROSS_SPEAKER_BLUE_4,
};

class GameRender : public Rendr {
public:
	void init(GLFWwindow* w);
	void highway(double time);
	//TODO:Convert every draw call to either Vertex or Quad
	void clicker();
	void notes(double time, std::vector<Note>& v, std::vector<Note>& cross);
	void events(double time, std::vector<Note>& ev, std::vector<Note>& cross);
	void lanes(double time, std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& cross);
	void bpmTicks(double time, std::vector<double>& bpmArr);
	void clickerTimer();
	void result(Player& player, Generator& generator);
	void meters(double time);
	void pollState(double time, Player& p, Generator& g);
	void updateTimers(double time);
	void debug(double deltaTime, std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& c);
	void reset();

	bool m_isButtonsRight = false;
	float m_noteVisibleTime = 1.0;

	//lane colors
	glm::vec4 m_greenLaneActiveColor = {0.0, 1.0, 0.0, 1.0};
	glm::vec4 m_greenLaneInactiveColor = {0.0, 0.5, 0.0, 1.0};
	glm::vec4 m_greenScratchColor = {0.0, 1.0, 0.0, 1.0};

	glm::vec4 m_blueLaneActiveColor = {0.0, 0.0, 1.0, 1.0};
	glm::vec4 m_blueLaneInactiveColor = {0.0, 0.0, 0.5, 1.0};
	glm::vec4 m_blueScratchColor = {0.0, 0.0, 1.0, 1.0};

	glm::vec4 m_redLaneActiveColor = {1.0, 0.0, 0.0, 1.0};
	glm::vec4 m_euphoriaLaneColor = {1.0, 1.0, 1.0, 1.0};
	glm::vec4 m_euphoriaZoneColor = {1.0, 1.0, 1.0, 0.2};

	glm::vec4 m_fsCrossBaseGreen = {0.0, 1.0, 0.0, 0.2};
	glm::vec4 m_fsCrossBaseBlue = {0.0, 0.0, 1.0, 0.2};

	TimerManager m_animManager;

private:
	static std::vector<Note> getCrossInsideNote(Note& note, std::vector<Note>& crossArr);
	int getCrossAtTime(double time, std::vector<Note>& crossArr, std::vector<Note>* eventArr = nullptr);
	double getAngleFromDT(double dt) const;
	double getDTFromAngle(double angle) const;
	static double getAngleHorizontal(double innerAngle, double innerRadius, double outerRadius);
	Quad createCircleQuad(double angle, double baseRadius, double deltaRadius, double deltaAngle, float plane = 0.0f) const;

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
	unsigned int m_clickerTimer = 0;
	unsigned int m_pgBarFrame = 0;
	unsigned int m_pgBarInside = 0;
	unsigned int m_fontTexture = 0;
	unsigned int m_textureVAO = 0;
	unsigned int m_textureVBO = 0;
	unsigned int m_colorVAO = 0;
	unsigned int m_colorVBO = 0;

	FT_Library m_FTLibrary;
	FT_Face m_font;

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
	float m_genBPM = 120.0f;
};
