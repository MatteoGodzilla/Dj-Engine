#include "Game.h"

using namespace std::chrono;

void Game::init(GLFWwindow* w) {
	m_render.init(w);

	m_player.readMappingFile();

	CSimpleIniA ini;
	const char* section = "Engine";

	ini.LoadFile("config.ini");

	m_audioLatency = ini.GetDoubleValue(section, "audioLatency", 0.0);
	m_deckSpeed = ini.GetDoubleValue(section, "noteVisible", 1.0);
	m_isButtonsRight = ini.GetBoolValue(section, "buttonsRight", false);
	m_debugView = ini.GetBoolValue(section, "debugView", false);
	m_inputThreadPollRate = ini.GetLongValue(section, "pollRate", 240);

	m_render.m_greenLaneActiveColor.r = ini.GetDoubleValue(section, "greenLaneActiveR", 0.133333);
	m_render.m_greenLaneActiveColor.g = ini.GetDoubleValue(section, "greenLaneActiveG", 0.874510);
	m_render.m_greenLaneActiveColor.b = ini.GetDoubleValue(section, "greenLaneActiveB", 0.180392);
	m_render.m_greenLaneActiveColor.a = ini.GetDoubleValue(section, "greenLaneActiveA", 1.000000);

	m_render.m_greenLaneInactiveColor.r = ini.GetDoubleValue(section, "greenLaneInactiveR", 0.059977);
	m_render.m_greenLaneInactiveColor.g = ini.GetDoubleValue(section, "greenLaneInactiveG", 0.392157);
	m_render.m_greenLaneInactiveColor.b = ini.GetDoubleValue(section, "greenLaneInactiveB", 0.083425);
	m_render.m_greenLaneInactiveColor.a = ini.GetDoubleValue(section, "greenLaneInactiveA", 1.000000);

	m_render.m_blueLaneActiveColor.r = ini.GetDoubleValue(section, "blueLaneActiveR", 0.239216);
	m_render.m_blueLaneActiveColor.g = ini.GetDoubleValue(section, "blueLaneActiveG", 0.305882);
	m_render.m_blueLaneActiveColor.b = ini.GetDoubleValue(section, "blueLaneActiveB", 0.745098);
	m_render.m_blueLaneActiveColor.a = ini.GetDoubleValue(section, "blueLaneActiveA", 1.000000);

	m_render.m_blueLaneInactiveColor.r = ini.GetDoubleValue(section, "blueLaneInactiveR", 0.126105);
	m_render.m_blueLaneInactiveColor.g = ini.GetDoubleValue(section, "blueLaneInactiveG", 0.163666);
	m_render.m_blueLaneInactiveColor.b = ini.GetDoubleValue(section, "blueLaneInactiveB", 0.392157);
	m_render.m_blueLaneInactiveColor.a = ini.GetDoubleValue(section, "blueLaneInactiveA", 1.000000);

	m_render.m_redLaneActiveColor.r = ini.GetDoubleValue(section, "redLaneActiveR", 0.823529);
	m_render.m_redLaneActiveColor.g = ini.GetDoubleValue(section, "redLaneActiveG", 0.109804);
	m_render.m_redLaneActiveColor.b = ini.GetDoubleValue(section, "redLaneActiveB", 0.109804);
	m_render.m_redLaneActiveColor.a = ini.GetDoubleValue(section, "redLaneActiveA", 1.000000);

	m_render.m_greenScratchColor.r = ini.GetDoubleValue(section, "greenScratchR", 0.133333);
	m_render.m_greenScratchColor.g = ini.GetDoubleValue(section, "greenScratchG", 0.874510);
	m_render.m_greenScratchColor.b = ini.GetDoubleValue(section, "greenScratchB", 0.180392);
	m_render.m_greenScratchColor.a = ini.GetDoubleValue(section, "greenScratchA", 1.000000);

	m_render.m_blueScratchColor.r = ini.GetDoubleValue(section, "blueScratchR", 0.239216);
	m_render.m_blueScratchColor.g = ini.GetDoubleValue(section, "blueScratchG", 0.305882);
	m_render.m_blueScratchColor.b = ini.GetDoubleValue(section, "blueScratchB", 0.745098);
	m_render.m_blueScratchColor.a = ini.GetDoubleValue(section, "blueScratchA", 1.000000);

	m_render.m_euphoriaLaneColor.r = ini.GetDoubleValue(section, "euphoriaLaneActiveR", 1.000000);
	m_render.m_euphoriaLaneColor.g = ini.GetDoubleValue(section, "euphoriaLaneActiveG", 1.000000);
	m_render.m_euphoriaLaneColor.b = ini.GetDoubleValue(section, "euphoriaLaneActiveB", 1.000000);
	m_render.m_euphoriaLaneColor.a = ini.GetDoubleValue(section, "euphoriaLaneActiveA", 1.000000);

	m_render.m_euphoriaZoneColor.r = ini.GetDoubleValue(section, "euphoriaZoneActiveR", 1.000000);
	m_render.m_euphoriaZoneColor.g = ini.GetDoubleValue(section, "euphoriaZoneActiveG", 1.000000);
	m_render.m_euphoriaZoneColor.b = ini.GetDoubleValue(section, "euphoriaZoneActiveB", 1.000000);
	m_render.m_euphoriaZoneColor.a = ini.GetDoubleValue(section, "euphoriaZoneActiveA", 0.200000);

	m_inputThread = std::thread(inputThreadFun, this);
	std::cout << "Game Message: started input thread" << std::endl;
	setButtonPos(m_isButtonsRight);
}

void Game::inputThreadFun(Game* game) {
	while (game->m_inputThreadRunning) {
		//start timer
		time_point<high_resolution_clock> start = high_resolution_clock::now();

		game->m_gen.addNotesToBuffers(game->m_note_arr, game->m_event_arr, game->m_cross_arr);
		game->m_gen.bpm(game->m_global_time, game->m_bpm_arr);
		game->m_player.pollInput(game->m_render.getWindowPtr(), game->m_global_time, game->m_note_arr, game->m_event_arr, game->m_cross_arr);
		//remove clicked notes
		game->m_gen.tick(game->m_global_time, game->m_note_arr, game->m_event_arr, game->m_cross_arr);

		//update player (combo + multiplier)
		game->m_player.pollState(game->m_gen);
		game->m_player.tick(game->m_global_time);

		time_point<high_resolution_clock> end = high_resolution_clock::now();
		milliseconds delta = duration_cast<milliseconds>(end - start);
		//stop timer

		if (glfwGetKey(game->getGameRender()->getWindowPtr(), GLFW_KEY_ESCAPE) && game->m_active) {
			game->m_mode = 1;
			game->m_audio.stop();
			game->m_active = false;
		}

		std::this_thread::sleep_for(milliseconds(1000 / game->m_inputThreadPollRate) - delta);
		//wait time before next input frame
	}
	std::cout << "Input Thread Message: stopped" << std::endl;
}

void Game::tick() {
	if (m_active) {
		if (m_mode == 0) {
			m_render.m_noteVisibleTime = m_deckSpeed;
			m_gen.m_deckSpeed = m_deckSpeed;

			m_render.pollState(m_global_time, m_player, m_gen);
			m_audio.buffer(m_global_time);
			if (m_global_time >= (double)-m_audioLatency) {
				m_audio.play();
			}

			//add delta time to m_global_time
			double nowTime = glfwGetTime();
			m_deltaTime = nowTime - m_pastTime;
			m_global_time += m_deltaTime;
			m_pastTime = nowTime;
		}
		if (!m_audio.isActive(m_global_time)) {
			m_mode = 1;
			m_audio.stop();
			m_active = false;
		}
	}
}

void Game::render() {
	if (m_active) {
		if (m_mode == 0) {
			//each function is responsible for rendering a single 'object' on screen
			m_render.highway(m_global_time);
			m_render.bpmTicks(m_global_time, m_bpm_arr);
			m_render.meters(m_global_time);

			m_render.clicker();
			m_render.lanes(m_global_time, m_note_arr, m_cross_arr);
			m_render.events(m_global_time, m_event_arr, m_cross_arr);
			m_render.notes(m_global_time, m_note_arr, m_cross_arr);

			m_render.updateAnimations(m_global_time);
			m_render.clickerAnimation();

			//debug
			if (m_debugView) {
				m_render.debug(m_deltaTime, m_note_arr, m_event_arr, m_cross_arr);
			}
		}
	}
}

void Game::reset() {
	m_note_arr.clear();
	m_event_arr.clear();
	m_cross_arr.clear();
	m_bpm_arr.clear();

	m_global_time = -2.0f;
	m_active = false;
	firstRun = true;
	m_mode = 0;

	m_player.m_deltaMouse = false;

	m_render.reset();
	m_gen.reset();
	m_player.reset();
	m_audio.reset();
}

void Game::setButtonPos(bool value) {
	m_isButtonsRight = value;
	m_render.m_isButtonsRight = value;
	m_player.m_isButtonsRight = value;
}

void Game::start(const SongEntry& entry) {
	std::cout << "Game msg: started game" << std::endl;
	glfwSetTime(0.0);
	//glfwSetInputMode(m_render.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//m_player.m_deltaMouse = true;
	m_pastTime = glfwGetTime();
	m_global_time = -2.0f;

	std::string audioPath = entry.path + std::string("/song.ogg");
	m_audio.load(audioPath.c_str());

	m_gen.init(entry);
	m_bpm_arr.push_back(0.0);

	m_active = true;
	m_mode = 0;
}

void Game::writeConfig() {
	CSimpleIniA ini;
	const char* section = "Engine";
	ini.SetDoubleValue(section, "audioLatency", m_audioLatency);
	ini.SetDoubleValue(section, "noteVisible", m_deckSpeed);
	ini.SetBoolValue(section, "buttonsRight", m_isButtonsRight);
	ini.SetBoolValue(section, "debugView", m_debugView);
	ini.SetLongValue(section, "pollRate", m_inputThreadPollRate);

	ini.SetDoubleValue(section, "greenLaneActiveR", m_render.m_greenLaneActiveColor.r);
	ini.SetDoubleValue(section, "greenLaneActiveG", m_render.m_greenLaneActiveColor.g);
	ini.SetDoubleValue(section, "greenLaneActiveB", m_render.m_greenLaneActiveColor.b);
	ini.SetDoubleValue(section, "greenLaneActiveA", m_render.m_greenLaneActiveColor.a);

	ini.SetDoubleValue(section, "greenLaneInactiveR", m_render.m_greenLaneInactiveColor.r);
	ini.SetDoubleValue(section, "greenLaneInactiveG", m_render.m_greenLaneInactiveColor.g);
	ini.SetDoubleValue(section, "greenLaneInactiveB", m_render.m_greenLaneInactiveColor.b);
	ini.SetDoubleValue(section, "greenLaneInactiveA", m_render.m_greenLaneInactiveColor.a);

	ini.SetDoubleValue(section, "blueLaneActiveR", m_render.m_blueLaneActiveColor.r);
	ini.SetDoubleValue(section, "blueLaneActiveG", m_render.m_blueLaneActiveColor.g);
	ini.SetDoubleValue(section, "blueLaneActiveB", m_render.m_blueLaneActiveColor.b);
	ini.SetDoubleValue(section, "blueLaneActiveA", m_render.m_blueLaneActiveColor.a);

	ini.SetDoubleValue(section, "blueLaneInactiveR", m_render.m_blueLaneInactiveColor.r);
	ini.SetDoubleValue(section, "blueLaneInactiveG", m_render.m_blueLaneInactiveColor.g);
	ini.SetDoubleValue(section, "blueLaneInactiveB", m_render.m_blueLaneInactiveColor.b);
	ini.SetDoubleValue(section, "blueLaneInactiveA", m_render.m_blueLaneInactiveColor.a);

	ini.SetDoubleValue(section, "redLaneActiveR", m_render.m_redLaneActiveColor.r);
	ini.SetDoubleValue(section, "redLaneActiveG", m_render.m_redLaneActiveColor.g);
	ini.SetDoubleValue(section, "redLaneActiveB", m_render.m_redLaneActiveColor.b);
	ini.SetDoubleValue(section, "redLaneActiveA", m_render.m_redLaneActiveColor.a);

	ini.SetDoubleValue(section, "greenScratchR", m_render.m_greenScratchColor.r);
	ini.SetDoubleValue(section, "greenScratchG", m_render.m_greenScratchColor.g);
	ini.SetDoubleValue(section, "greenScratchB", m_render.m_greenScratchColor.b);
	ini.SetDoubleValue(section, "greenScratchA", m_render.m_greenScratchColor.a);

	ini.SetDoubleValue(section, "blueScratchR", m_render.m_blueScratchColor.r);
	ini.SetDoubleValue(section, "blueScratchG", m_render.m_blueScratchColor.g);
	ini.SetDoubleValue(section, "blueScratchB", m_render.m_blueScratchColor.b);
	ini.SetDoubleValue(section, "blueScratchA", m_render.m_blueScratchColor.a);

	ini.SetDoubleValue(section, "euphoriaLaneActiveR", m_render.m_euphoriaLaneColor.r);
	ini.SetDoubleValue(section, "euphoriaLaneActiveG", m_render.m_euphoriaLaneColor.g);
	ini.SetDoubleValue(section, "euphoriaLaneActiveB", m_render.m_euphoriaLaneColor.b);
	ini.SetDoubleValue(section, "euphoriaLaneActiveA", m_render.m_euphoriaLaneColor.a);

	ini.SetDoubleValue(section, "euphoriaZoneActiveR", m_render.m_euphoriaZoneColor.r);
	ini.SetDoubleValue(section, "euphoriaZoneActiveG", m_render.m_euphoriaZoneColor.g);
	ini.SetDoubleValue(section, "euphoriaZoneActiveB", m_render.m_euphoriaZoneColor.b);
	ini.SetDoubleValue(section, "euphoriaZoneActiveA", m_render.m_euphoriaZoneColor.a);

	ini.SaveFile("config.ini");
	std::cout << "Game Message: Written engine conigs to 'config.ini'" << std::endl;

	m_player.writeMappingFile();
}

void Game::stopThread() {
	m_inputThreadRunning = false;
	m_inputThread.join();
}

bool Game::getActive() const {
	return m_active;
}

void Game::setActive(bool active) {
	m_active = active;
}

Player* Game::getPlayer() {
	return &m_player;
}

Audio* Game::getAudio() {
	return &m_audio;
}

GameRender* Game::getGameRender() {
	return &m_render;
}

Generator* Game::getGenerator() {
	return &m_gen;
}

Game::~Game() {
	m_note_arr.clear();
	m_event_arr.clear();
}
