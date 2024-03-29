// SPDX-FileCopyrightText: 2021 MatteoGodzilla
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "Generator.h"

//read little endian int from data stream
void readToInt(std::ifstream& stream, int* i) {
	char c;
	char* mod = (char*)i;
	for (int i = 0; i < 4; i++) {
		stream.read(&c, sizeof(c));
		mod[3 - i] = c;
	}
}

//read little endian float from data stream
void readToFloat(std::ifstream& stream, float* f) {
	char c;
	char* mod = (char*)f;
	for (int i = 0; i < 4; i++) {
		stream.read(&c, sizeof(c));
		mod[3 - i] = c;
	}
}

void Generator::init(const SongEntry& entry, int difficulty) {
	m_songEntry = entry;
	m_allCross.emplace_back(m_initialCrossfade, CROSS_C, 0.0f, true); //Do not remove
	m_bpm = entry.bpm;

	std::string diff = std::string();
	if (difficulty == 0) {
		diff = std::string("/DJ_Expert.xmk");
		m_chart.open(entry.path + diff);
		if (!m_chart.is_open()) {
			//if not available, fallback to chart.xmk
			diff = std::string("/chart.xmk");
		}
		m_chart.close();
	} else if (difficulty == 1) {
		diff = std::string("/DJ_Hard.xmk");
	} else if (difficulty == 2) {
		diff = std::string("/DJ_Medium.xmk");
	} else if (difficulty == 3) {
		diff = std::string("/DJ_Easy.xmk");
	} else if (difficulty == 4) {
		diff = std::string("/DJ_Beginner.xmk");
	}

	std::cout << "Generator msg: loading " << diff << std::endl;
	std::string chartPath = entry.path + diff;
	m_chart.open(chartPath, std::ios::binary);
	if (m_chart.is_open()) {
		//write chart data to console
		m_isChartBinary = true;
		std::cout << "loaded xmk chart" << std::endl;
		int version = 0;
		int dummy = 0;
		readToInt(m_chart, &version);
		readToInt(m_chart, &dummy);
		readToInt(m_chart, &dummy);
		readToInt(m_chart, &dummy);
		std::cout << "version: " << version << std::endl;
	} else {
		std::cerr << "Generator Error: could not load chart file" << std::endl;
	}
	initialLoad();
}

void Generator::initialLoad() {
	/*
	read the chart IF there are less than 100 notes
	and less than 100 events on screen.

	This limit is arbitrary and can change in the future.
	Mostly it's there to use less amount of ram on the pc

	(but really, 100 notes present on screen is ridiculous)
	*/
	if (m_isChartBinary) {
		while (!m_chart.eof()) {
			float time;
			int type;
			float length = 0.0;
			float extra;

			//read entry from file
			readToFloat(m_chart, &time);
			readToInt(m_chart, &type);
			readToFloat(m_chart, &length);
			readToFloat(m_chart, &extra);

			double factor = 240.0 / m_bpm;

			time *= (float)factor;
			length *= (float)factor;

			//std::cout << time << "\t" << type << "\t" << length << "\t" << extra << std::endl;

			//decode type from entry
			if (type == FSG_TAP_G) {
				m_baseScore += 400;
				m_allTaps.emplace_back(time, TAP_G, length, false);
				if (length > 15 / m_bpm) {
					for (float t = 0; t < length; t += 15 / m_bpm) {
						m_allTaps.emplace_back(time + t, TAP_G_HOLD_TICK, 0, false);
					}
				}
			} else if (type == FSG_TAP_B) {
				m_baseScore += 400;
				m_allTaps.emplace_back(time, TAP_B, length, false);
				if (length > 15 / m_bpm) {
					for (float t = 0; t < length; t += 15 / m_bpm) {
						m_allTaps.emplace_back(time + t, TAP_B_HOLD_TICK, 0, false);
					}
				}
			} else if (type == FSG_TAP_R) {
				m_baseScore += 400;
				m_allTaps.emplace_back(time, TAP_R, length, false);
				if (length > 15 / m_bpm) {
					for (float t = 0; t < length; t += 15 / m_bpm) {
						m_allTaps.emplace_back(time + t, TAP_R_HOLD_TICK, 0, false);
					}
				}
			} else if (type == FSG_SCR_G_UP) {
				m_baseScore += 400;
				m_allTaps.emplace_back(time, SCR_G_UP, length, false);
			} else if (type == FSG_SCR_B_UP) {
				m_baseScore += 400;
				m_allTaps.emplace_back(time, SCR_B_UP, length, false);
			} else if (type == FSG_SCR_G_DOWN) {
				m_baseScore += 400;
				m_allTaps.emplace_back(time, SCR_G_DOWN, length, false);
			} else if (type == FSG_SCR_B_DOWN) {
				m_baseScore += 400;
				m_allTaps.emplace_back(time, SCR_B_DOWN, length, false);
			} else if (type == FSG_SCR_G_ANY) {
				m_baseScore += 400;
				m_allTaps.emplace_back(time, SCR_G_ANY, length, false);

				float end = time + length;
				float betweenTicks = 60.0f / ((float)m_bpm * TICKS_PER_BEAT);
				time += betweenTicks;
				while (time < end) {
					m_baseScore += 100;
					m_allTaps.emplace_back(time, SCR_G_TICK, length, false);
					time += betweenTicks;
				}
			} else if (type == FSG_SCR_B_ANY) {
				m_baseScore += 400;
				m_allTaps.emplace_back(time, SCR_B_ANY, length, false);
				float end = time + length;
				float betweenTicks = 60.0f / (m_bpm * TICKS_PER_BEAT);
				time += betweenTicks;
				while (time < end) {
					m_baseScore += 100;
					m_allTaps.emplace_back(time, SCR_B_TICK, length, false);
					time += betweenTicks;
				}
			} else if (type == FSG_CROSS_B) {
				m_firstSpikeGenerated = false;
				m_addedCrossCenter = false;
				m_baseScore += 400;
				m_allCross.emplace_back(time, CROSS_B, 0.0, true);
				for (double i = 15 / m_bpm; i < length; i += 15 / m_bpm) {
					m_allCross.emplace_back(time + i, CROSS_B_TICK, 0.0, false);
				}
			} else if (type == FSG_CROSS_C) {
				m_firstSpikeGenerated = false;
				m_addedCrossCenter = false;
				if (time > 0.0) {
					m_baseScore += 400;
					m_allCross.emplace_back(time, CROSS_C, 0.0, true);
				}
			} else if (type == FSG_CROSS_G) {
				m_firstSpikeGenerated = false;
				m_addedCrossCenter = false;
				m_baseScore += 400;
				m_allCross.emplace_back(time, CROSS_G, 0.0, true);
				for (double i = 15 / m_bpm; i < length; i += 15 / m_bpm) {
					m_allCross.emplace_back(time + i, CROSS_G_TICK, 0.0, false);
				}
			} else if (type == FSG_EUPHORIA) {
				m_allEvents.emplace_back(time, EU_ZONE, length, true);
			} else if (type == FSG_FS_CROSS) {
				m_allEvents.emplace_back(time, FS_CROSS_BASE, length, true);
			} else if (type == FSG_FS_SAMPLES) {
				m_allEvents.emplace_back(time, FS_SAMPLES, length, true);
			} else if (type == FSG_SCR_G_ZONE) {
				m_allEvents.emplace_back(time, SCR_G_ZONE, length, true);
			} else if (type == FSG_SCR_B_ZONE) {
				m_allEvents.emplace_back(time, SCR_B_ZONE, length, true);
			} else if (type == FSG_CF_SPIKE_G) {
				if (m_firstSpikeGenerated && !m_addedCrossCenter) {
					m_addedCrossCenter = true;
					m_allCross.emplace_back(m_firstSpikeMilli, CROSS_C, 0.0, true);
					m_firstSpikeMilli = 0.0;
				} else {
					m_firstSpikeGenerated = true;
					m_firstSpikeMilli = time;
				}
				m_allTaps.emplace_back(time, CF_SPIKE_G, 0.0, false);
				//pushNote((double)time, CF_SPIKE_G, 0.0);
			} else if (type == FSG_CF_SPIKE_B) {
				if (m_firstSpikeGenerated && !m_addedCrossCenter) {
					m_addedCrossCenter = true;
					m_allCross.emplace_back(m_firstSpikeMilli, CROSS_C, 0.0, true);
					m_firstSpikeMilli = 0.0;
				} else {
					m_firstSpikeGenerated = true;
					m_firstSpikeMilli = time;
				}
				m_allTaps.emplace_back(time, CF_SPIKE_B, 0.0, false);
				//pushNote((double)time, CF_SPIKE_B, 0.0);
			} else if (type == FSG_CF_SPIKE_C) {
				m_allTaps.emplace_back(time, CF_SPIKE_C, 0.0, false);
				//pushNote((double)time, CF_SPIKE_C, 0.0);
			}
			/*
			else if (type == 0x0B000001) {
				//bpm marker
			} else if (type == 0x0B000002) {
				m_bpm = extra;
			} else if (type == 0x0AFFFFFF) {
			} else if (type == 0xFFFFFFFF) {
				//chart start
			}
			*/
			//else std::cerr << "error parsing entry: " << time << "\t"<< type << "\t"<< length << "\t"<< extra<<std::endl;
		}
		if (m_chart.eof() && !m_placedFinalCF) {
			m_placedFinalCF = true;

			//place a crossfade center at the 100 hour mark
			//seems impossible to reach it for me
			//but then, there is always someone that does the impossible
			//pushCross(360000.0f, CROSS_C, 0.0f);
			m_allCross.emplace_back(360000.0, CROSS_C, 0.0f, true);
		}
	}
}

//update notes/events every frame
void Generator::tick(double time, std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& cross) {
	m_combo_reset = false;
	m_eu_check = false;

	if (!v.empty()) {
		for (size_t i = v.size(); i-- > 0;) {
			v.at(i).tick(time);
			int type = v.at(i).getType();
			//remove if outside hit area
			if (v.at(i).getDead()) {
				if (v.at(i).getTouched()) {
					if (type == SCR_G_TICK || type == SCR_B_TICK || type == SCR_G_ANY || type == SCR_B_ANY) {
						m_notesTotal--;
						m_notesHit--;
						m_scr_tick++;
					}
					m_notesHit++;
				} else if (type != TAP_G_HOLD_TICK && type != TAP_R_HOLD_TICK && type != TAP_B_HOLD_TICK) {
					m_combo_reset = true;
				}
				m_notesTotal++;
				v.erase(v.begin() + i);
			}
		}
	}

	if (!ev.empty()) {
		for (size_t i = 0; i < ev.size(); ++i) {
			ev.at(i).tick(time);
			int type = ev.at(i).getType();
			double endTime = ev.at(i).getMilli() + ev.at(i).getLength();

			/*
			every event has a different way to be removed
			for example: a scratch start cannot be removed
			if the corrisponding scratch end is before the clicker
			*/

			if (type == SCR_G_ZONE || type == SCR_B_ZONE) {
				if (endTime < time - ev.at(i).hitWindow) {
					ev.erase(ev.begin() + i);
				}
			}
			if (type == EU_ZONE) {
				//start eu zone check when the event is on the clicker
				if (ev.at(i).getHit() && !ev.at(i).getTouched()) {
					m_eu_start = true;
					ev.at(i).click();
				}
				//set signal for player
				if (endTime < time) {
					m_eu_check = true;
					ev.erase(ev.begin() + i);
				} else if (m_combo_reset) {
					//remove event if misclick
					if (ev.at(i).getMilli() + ev.at(i).hitWindow < time) {
						ev.erase(ev.begin() + i);
					}
				}
			}
			if (type == FS_CROSS_BASE) {
				if (endTime < time) {
					ev.erase(ev.begin() + i);
				}
			}
			if (type == FS_SAMPLES) {
				if (endTime < time) {
					ev.erase(ev.begin() + i);
				}
			}
		}
	}

	if (!cross.empty()) {
		for (size_t i = 0; i < cross.size() - 1; i++) {
			cross.at(i).tick(time);
			int type = cross.at(i).getType();

			double next_time = cross.at(i + 1).getMilli();
			//if the next crossfader has crossed the clickers
			if (next_time + 0.15 <= time) {
				if (cross.at(i).getTouched()) {
					m_notesHit++;
				} else if (type != CROSS_G_TICK && type != CROSS_B_TICK) {
					m_combo_reset = true;
				}
				m_notesTotal++;
				cross.erase(cross.begin() + i);
			}
			if (cross.at(i).getMilli() == m_initialCrossfade) {
				cross.at(i).setTouched(true);
			}
		}
		cross.at(cross.size() - 1).tick(time);
	}

	if (m_scr_tick > 4) {
		m_scr_tick -= 4;
		m_notesHit++;
		m_notesTotal++;
	}

	/*
	if (m_bpmChangeTime != -1 && m_bpmChangeValue != -1 && time + 1.0 >= m_bpmChangeTime) {
		m_bpm = m_bpmChangeValue;
		m_bpmChangeValue = -1;
		m_bpmChangeTime = -1;
	}
	*/
}

void Generator::addNotesToBuffers(std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& cross) {
	while (v.size() < 100 && !m_allTaps.empty()) {
		v.push_back(m_allTaps.front());
		m_allTaps.pop_front();
	}
	while (ev.size() < 100 && !m_allEvents.empty()) {
		ev.push_back(m_allEvents.front());
		m_allEvents.pop_front();
	}
	while (cross.size() < 100 && !m_allCross.empty()) {
		cross.push_back(m_allCross.front());
		m_allCross.pop_front();
	}
}

void Generator::bpm(double time, std::vector<double>& arr) {
	//update bpm tick array

	for (size_t i = 0; i < arr.size(); i++) {
		if (arr.at(i) < time - 0.2) {
			arr.erase(arr.begin() + i);
		}
	}
	double nextTick = m_lastBpmTick + ((double)60 / m_bpm);
	while (time + m_deckSpeed >= nextTick) {
		arr.push_back(nextTick);
		m_lastBpmTick = nextTick;
		nextTick = m_lastBpmTick + ((float)60 / m_bpm);
	}
}

void Generator::reset() {
	m_chart.close();
	m_isChartBinary = false;
	m_placedFinalCF = false;
	m_bpmChangeTime = -1;
	m_bpmChangeValue = -1;
	m_lastBpmTick = 0.0;
	m_notesHit = 0;
	m_notesTotal = 0;
	m_baseScore = 0;

	m_allTaps.clear();
	m_allEvents.clear();
	m_allCross.clear();

	m_combo_reset = false;
	m_eu_start = false;
	m_eu_check = false;
}

int Generator::getNotesTotal() const {
	return m_notesTotal;
}

int Generator::getNotesHit() const {
	return m_notesHit;
}

SongEntry Generator::getSongEntry() const {
	return m_songEntry;
}

//utility functions

Generator::~Generator() {
	m_chart.close();
	//dtor
}
