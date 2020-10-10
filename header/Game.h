#pragma once
#include "Audio.h"
#include "GameRender.h"
#include "Generator.h"
#include "Note.h"
#include "Player.h"
#include "SimpleIni.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

class Game {
public:
	//Game();
	void init(GLFWwindow* w);
	void render();
	void tick();
	static void inputThreadFun(Game* gameptr);
	void pollInput();
	bool getActive() const;
	void setActive(bool active);
	void reset();
	void stopThread();
	Player* getPlayer();
	Audio* getAudio();
	GameRender* getGameRender();
	Generator* getGenerator();
	void start(const SongEntry& entry, int difficulty);
	void setButtonPos(bool value);
	void writeConfig();
	~Game();
	std::vector<Note> m_note_arr;
	std::vector<Note> m_event_arr;
	std::vector<Note> m_cross_arr;

	std::vector<double> m_bpm_arr;
	double m_global_time = -2.0;
	double m_deltaTime = 0.0;
	double m_audioLength = 0.0f;
	float m_audioLatency = 0.0f;
	float m_audioPosition = 0.0f;
	float m_deckSpeed = 1.0f;
	bool m_debugView = false;

	int m_mode = 0;
	int m_inputThreadPollRate = 240;

private:
	double m_pastTime = 0.0;
	bool audioStartedOnce = false;
	GameRender m_render;
	Generator m_gen;
	Player m_player;
	Audio m_audio;
	bool m_active = false;
	bool m_isButtonsRight = false;
	std::thread m_inputThread;
	bool m_inputThreadRunning = true;
};
