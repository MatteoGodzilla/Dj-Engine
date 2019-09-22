#include "Generator.h"

void readToInt(std::ifstream& stream, int* i) {
	char c;
	char* mod = (char*)i;
	for (int i = 0; i < 4; i++) {
		stream.read(&c, sizeof(c));
		mod[3 - i] = c;
	}
}

void readToFloat(std::ifstream& stream, float* f) {
	char c;
	char* mod = (char*)f;
	for (int i = 0; i < 4; i++) {
		stream.read(&c, sizeof(c));
		mod[3 - i] = c;
	}
}

Generator::Generator() {
	pushEvent(-2.0, CROSS_C);//Do not remove
	m_chart.open("res/chart.txt");
	if (m_chart.is_open()) {
		m_isTimeRelative = true;
		std::cout << "loaded text chart" << std::endl;
		std::string version;
		m_chart >> version;
		std::cout << "Chart Version: " << version << std::endl;
		m_isTimeRelative = true;
	}
	else {
		std::cout << "text chart not found, opening fgsmub" << std::endl;
		m_chart.open("res/chart.fsgmub",std::ios::binary);
		if (m_chart.is_open()) {
			m_isTimeRelative = false;
			std::cout << "loaded fgsmub chart" << std::endl;

			int version = 0;
			int dummy = 0;
			readToInt(m_chart, &version);
			readToInt(m_chart, &dummy);
			readToInt(m_chart, &dummy);
			readToInt(m_chart, &dummy);

			std::cout << "version: " << version << std::endl;
		}
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

	if (m_bpmChangeTime != -1 && m_bpmChangeValue != -1 && time + 1.0 >= m_bpmChangeTime) {
		m_bpm = m_bpmChangeValue;
		m_bpmChangeValue = -1;
		m_bpmChangeTime = -1;
	}
}

void Generator::textParser(std::vector<Note>& v, std::vector<Note>& ev) {
	while (!m_chart.eof() && v.size() < 100 && ev.size() < 100 && m_isTimeRelative) {
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
				std::cerr << "error parsing token:E " << token << std::endl;
			}
		}
		else if (token == "BPM" || token == "bpm") {
			m_chart >> token;
			m_bpmChangeTime = std::stod(token);
			m_chart >> token;
			m_bpmChangeValue = std::stoi(token);
		}
		else if (token == "SET" || token == "set") {
			m_chart >> token;
			if (token == "TIME" || token == "time") {
				m_chart >> token;
				if (token == "ABS" || token == "abs") m_isTimeRelative = false;
				else if (token == "REL" || token == "rel") m_isTimeRelative = true;
				else std::cerr << "error parsing token:SET TIME" << std::endl;
			}
			else std::cerr << "error parsing token:SET" << std::endl;
		}
		else {
		std::cerr << "unknown token:" << token << std::endl;
		}
	}
}

void Generator::binaryParser(std::vector<Note>& v, std::vector<Note>& ev) {
	while (!m_chart.eof() && v.size() < 100 && ev.size() < 100 && !m_isTimeRelative) {
		float time;
		int type;
		float length;
		float extra;

		readToFloat(m_chart, &time);
		readToInt(m_chart, &type);
		readToFloat(m_chart, &length);
		readToFloat(m_chart, &extra);

		int factor;
		if (m_bpmChangeValue != -1 && m_bpmChangeTime != -1 && time >= m_bpmChangeTime) {
			factor = (60*4)/m_bpmChangeValue;
		}
		else {
			factor = (60*4)/m_bpm ;
		}
		
		time = time * factor;
		length = length * factor;
		

		if (type == 0)pushNote((double)time, TAP_G);
		else if (type == 1)pushNote((double)time, TAP_B);
		else if (type == 2)pushNote((double)time, TAP_R);
		else if (type == 3)pushNote((double)time, SCR_G_UP);
		else if (type == 4)pushNote((double)time, SCR_B_UP);
		else if (type == 5)pushNote((double)time, SCR_G_DOWN);
		else if (type == 6)pushNote((double)time, SCR_B_DOWN);
		else if (type == 7)pushNote((double)time, SCR_G_ANY);
		else if (type == 8)pushNote((double)time, SCR_B_ANY);
		else if (type == 9)pushEvent((double)time, CROSS_B);
		else if (type == 10)pushEvent((double)time, CROSS_C);
		else if (type == 11)pushEvent((double)time, CROSS_G);
		else if (type == 15) {
			pushEvent((double)time, EU_START);
			pushEvent((double)time + (double)length, EU_END);
		}
		else if (type == 20) {
			pushEvent((double)time, SCR_G_START);
			pushEvent((double)time + (double)length, SCR_G_END);
		}
		else if (type == 20) {
			pushEvent((double)time, SCR_B_START);
			pushEvent((double)time + (double)length, SCR_B_END);
		}
		else if (type == 0x0B000001) {
			//bpm marker
		}
		else if (type == 0x0B000002) {
			m_bpmChangeTime = (double)time;
			m_bpmChangeValue = extra;
		}
		else if (type == 0x0AFFFFFF) {}
		else if (type == 0xFFFFFFFF) {
			//chart start
			//pushEvent(time,___)
		}
		else std::cerr << "error parsing entry" << std::endl;
	}
}

void Generator::bpm(double time, std::vector<double>& arr)
{
	for (size_t i = 0; i < arr.size(); i++) {
		if (arr.at(i) < time - 0.2) {
			arr.erase(arr.begin() + i);
		}
	}
	double nextTick = m_lastBpmTick + ((double)60 / m_bpm);
	while (time + 1.0 >= nextTick) {
		arr.push_back(nextTick);
		m_lastBpmTick = nextTick;
		nextTick = m_lastBpmTick + ((float)60 / m_bpm);
	}
}

void Generator::pushNote(double time, int type) {
	if (m_isTimeRelative) m_time += time;
	else m_time = time;
	m_note_times.push_back(m_time);
	m_note_types.push_back(type);
}

void Generator::pushEvent(double time, int type) {
	if (m_isTimeRelative) m_time += time;
	else m_time = time;
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
