#pragma once
#include "Animation.h"
#include "Audio.h"
#include "GameRender.h"
#include "Generator.h"
#include "Note.h"
#include "Player.h"

#include <iostream>
#include <string>
#include <vector>

class Game {
public:
	//Game();
	void init(GLFWwindow* w);
	void render();
	void tick();
	void pollInput();
	bool getActive();
	void setActive(bool active);
	void reset();
	Player* getPlayer();
	Audio* getAudio();
	GameRender* getGameRender();
	void start(const SongEntry& entry);
	void setButtonPos(bool value);
	void writeConfig();
	~Game();
	std::vector<Note> m_note_arr;
	std::vector<Note> m_event_arr;
	std::vector<Note> m_cross_arr;

	std::vector<double> m_bpm_arr;
	double m_global_time = -2.0;
	double m_deltaTime = 0.0;
	float m_audioLatency = 0.0f;
	float m_deckSpeed = 1.0f;
	bool m_debugView = false;

private:
	double m_pastTime = 0.0;
	bool firstRun = true;
	GameRender m_render;
	Generator m_gen;
	Player m_player;
	Audio m_audio;
	bool m_active = false;
	int m_mode = 0;
	bool m_isButtonsRight = false;
};
