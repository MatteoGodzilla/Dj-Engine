#include "Generator.h"

Generator::Generator() {
	pushEvent(0.0, CROSS_C);//Do not remove
	m_chart.open("res/chart.txt");
	if (m_chart.is_open()) {
		std::cout << "loaded chart" << std::endl;
		std::string version;
		m_chart >> version;
		std::cout << "Chart Version: " << version << std::endl;
	}
	else {
		std::cout << "error loading chart" << std::endl;
	}
}

void Generator::tick(double time, std::vector<Note>& v, std::vector<Note>& ev) {
	m_combo_reset = false;
	m_eu_start = false;
	m_eu_check = false;
	size_t note_s = m_note_times.size();

	for (size_t i = 0; i < note_s; ++i) {
		//note generation
		Note temp(m_note_times.at(0), m_note_types.at(0));
		v.push_back(temp);
		m_note_times.erase(m_note_times.begin());
		m_note_types.erase(m_note_types.begin());
	}
	size_t event_s = m_event_times.size();

	for (size_t i = 0; i < event_s; ++i) {
		Note e(m_event_times.at(0), m_event_types.at(0), true);
		ev.push_back(e);
		m_event_times.erase(m_event_times.begin());
		m_event_types.erase(m_event_types.begin());
	}
	if (!v.empty()) {
		for (size_t i = v.size(); i-- > 0;) {
			//remove if outside hit area
			v.at(i).tick(time);
			if (v.at(i).getDead()) {
				if (v.at(i).getTouched() == false) {
					m_combo_reset = true;
					v.at(i).click(time);
				}
				v.erase(v.begin() + i);
				break;
			}
		}
	}

	if (!ev.empty()) {
		for (size_t i = ev.size(); i-- > 0;) {
			ev.at(i).tick(time);
			int type = ev.at(i).getType();

			if (type == SCR_G_START) {
				int e = -1;
				for (size_t j = ev.size(); j-- > 0;) {
					if (i != j && ev.at(j).getType() == SCR_G_END && time > ev.at(j).getMilli() + 0.2)e = j;
				}
				if (e != -1) {
					ev.erase(ev.begin() + e);
					ev.erase(ev.begin() + i);
				}
			}

			if (type == SCR_B_START) {
				int e = -1;
				for (size_t j = ev.size(); j-- > 0;) {
					if (i != j && ev.at(j).getType() == SCR_B_END && time >= ev.at(j).getMilli() + 0.2)e = j;
				}
				if (e != -1) {
					ev.erase(ev.begin() + e);
					ev.erase(ev.begin() + i);
				}
			}
			if (type == CROSS_G || type == CROSS_C || type == CROSS_B) {
				for (size_t j = ev.size(); j-- > i;) {
					int next_type = ev.at(j).getType();
					double next_time = ev.at(j).getMilli();
					if (next_type == CROSS_G || next_type == CROSS_C || next_type == CROSS_B) {
						if (j > i && next_time + 0.15 <= time) {
							if (!ev.at(i).getTouched()) {
								m_combo_reset = true;
							}
							ev.erase(ev.begin() + i);
						}
					}
				}
			}
			if (type == EU_START) {
				if (ev.at(i).getHit()) {
					m_eu_start = true;
				}
				int e = -1;
				for (size_t j = i; j < ev.size(); j++) {
					if (i != j && ev.at(j).getType() == EU_END && time >= ev.at(j).getMilli())e = j;
				}
				if (e != -1) {
					m_eu_check = true;
					ev.erase(ev.begin() + e);
					ev.erase(ev.begin() + i);
				}
			}
		}
	}
}

void Generator::gen(std::vector<Note>& v, std::vector<Note>& ev) {
	while (!m_chart.eof() && v.size() < 100 && ev.size() < 100) {
		std::string token;
		double t;
		m_chart >> token;
		if (token == "T" || token == "t") {
			m_chart >> token;
			if (token == "R" || token == "r") {
				m_chart >> token;
				t = std::stod(token);
				pushNote(t, TAP_R);
			}
			else if (token == "G" || token == "g") {
				m_chart >> token;
				t = std::stod(token);
				pushNote(t, TAP_G);
			}
			else if (token == "B" || token == "b") {
				m_chart >> token;
				t = std::stod(token);
				pushNote(t, TAP_B);
			}
			else {
				std::cerr << "error parsing token:T " << token << std::endl;
			}
		}
		else if (token == "C" || token == "c") {
			m_chart >> token;
			if (token == "G" || token == "g") {
				m_chart >> token;
				t = std::stod(token);
				pushEvent(t, CROSS_G);
			}
			else if (token == "B" || token == "b") {
				m_chart >> token;
				t = std::stod(token);
				pushEvent(t, CROSS_B);
			}
			else if (token == "C" || token == "c") {
				m_chart >> token;
				t = std::stod(token);
				pushEvent(t, CROSS_C);
			}
			else {
				std::cerr << "error parsing token:C " << token << std::endl;
			}
		}
		else if (token == "S" || token == "s") {
			m_chart >> token;
			if (token == "G" || token == "g") {
				m_chart >> token;
				if (token == "U" || token == "u") {
					m_chart >> token;
					t = std::stod(token);
					pushNote(t, SCR_G_UP);
				}
				else if (token == "D" || token == "d") {
					m_chart >> token;
					t = std::stod(token);
					pushNote(t, SCR_G_DOWN);
				}
				else if (token == "A" || token == "a") {
					m_chart >> token;
					t = std::stod(token);
					pushNote(t, SCR_G_ANY);
				}
				else if (token == "S" || token == "s") {
					m_chart >> token;
					t = std::stod(token);
					pushEvent(t, SCR_G_START);
				}
				else if (token == "E" || token == "e") {
					m_chart >> token;
					t = std::stod(token);
					pushEvent(t, SCR_G_END);
				}
				else {
					std::cerr << "error parsing token:S G " << token << std::endl;
				}
			}
			else if (token == "B") {
				m_chart >> token;
				if (token == "U" || token == "u") {
					m_chart >> token;
					t = std::stod(token);
					pushNote(t, SCR_B_UP);
				}
				else if (token == "D" || token == "d") {
					m_chart >> token;
					t = std::stod(token);
					pushNote(t, SCR_B_DOWN);
				}
				else if (token == "A" || token == "a") {
					m_chart >> token;
					t = std::stod(token);
					pushNote(t, SCR_B_ANY);
				}
				else if (token == "S" || token == "s") {
					m_chart >> token;
					t = std::stod(token);
					pushEvent(t, SCR_B_START);
				}
				else if (token == "E" || token == "e") {
					m_chart >> token;
					t = std::stod(token);
					pushEvent(t, SCR_B_END);
				}
				else {
					std::cerr << "error parsing token:S B " << token << std::endl;
				}
			}
			else {
				std::cerr << "error parsing token:S " << token;
			}
		}
		else if (token == "E" || token == "e") {
			m_chart >> token;
			if (token == "S" || token == "s") {
				m_chart >> token;
				t = std::stod(token);
				pushEvent(t, EU_START);
			}
			else if (token == "E" || token == "e") {
				m_chart >> token;
				t = std::stod(token);
				pushEvent(t, EU_END);
			}
			else {
				std::cerr << "error parsing token:S " << token << std::endl;
			}
		}
	}
}

void Generator::pushNote(double time, int type) {
	m_time += time;
	m_note_times.push_back(m_time);
	m_note_types.push_back(type);
}

void Generator::pushEvent(double time, int type) {
	m_time += time;
	m_event_times.push_back(m_time);
	m_event_types.push_back(type);
}

Generator::~Generator() {
	m_note_times.clear();
	m_note_types.clear();
	m_event_times.clear();
	m_event_types.clear();
	m_chart.close();
	//dtor
}
