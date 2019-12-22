#pragma once
#include "GameRender.h"
#include "Note.h"
#include "Generator.h"
#include "Player.h"
#include "Audio.h"
#include <vector>
#include <iostream>
#include <string>

class Game
{
public:
	Game();
	void init(GLFWwindow* w,std::string path);
	void render();
	void tick();
	void pollInput();
	bool getActive();
	void setActive(bool active);
	Player* getPlayer();
	void start();
	~Game();
	std::vector<Note> m_note_arr;
	std::vector<Note> m_event_arr;
	std::vector<Note> m_cross_arr;

	std::vector<double> m_bpm_arr;
	double m_global_time = -2.0;
	
private:
	double m_pastTime = 0.0;
	bool firstRun = true;
	GameRender m_render;
	Generator m_gen;
	Player m_player;
	//Audio m_audio;
	bool m_active = false;
	int m_mode = 0;
};

