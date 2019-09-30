#pragma once
#include "Generator.h"
#include "Note.h"
#include "GameRender.h"
#include "Player.h"
#include "Audio.h"
#include <vector>
#include <iostream>

class Game
{
public:
	Game();
	void init(GLFWwindow* w);
	void render();
	void tick();
	void input(int key, int action);
	~Game();
	std::vector<Note> m_note_arr;
	std::vector<Note> m_event_arr;
	std::vector<double> m_bpm_arr;
	double m_global_time = -2.0;

private:
	double m_pastTime = 0.0;
	bool firstRun = true;
	GameRender m_render;
	Generator m_gen;
	Player m_player;
	Audio m_audio;
	int m_scene = 0;
};

