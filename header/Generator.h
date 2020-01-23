#ifndef GENERATOR_H
#define GENERATOR_H
#include "Note.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

class Generator {
public:
	Generator();
	void init(std::string& path, float bpm);
    void tick(double time,std::vector<Note> &v,std::vector<Note>&ev, std::vector<Note>& c);
	//void textParser(std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& c);
	void binaryParser(std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& c);
	void bpm(double time, std::vector<double>& arr);
	int getNotesTotal();
	int getNotesHit();
	void reset();

    bool m_combo_reset = false;
	bool m_eu_start = false;
	bool m_eu_check = false;
	float m_bpm = 60;

	bool m_isGreenTapEnabled = true;
	bool m_isRedTapEnabled = true;
	bool m_isBlueTapEnabled = true;
    ~Generator();
protected:

private:
    void pushNote(double time, int type, double length);
    void pushEvent(double time, int type, double length);
	void pushCross(double time, int type, double length);
    std::vector<double> m_note_times;
	std::vector<double> m_note_length;
    std::vector<int> m_note_types;

    std::vector<double> m_event_times;
	std::vector<double> m_event_length;
    std::vector<int> m_event_types;

	std::vector<double> m_cross_times;
	std::vector<double> m_cross_length;
	std::vector<int> m_cross_types;

	std::ifstream m_chart;
	bool m_isChartBinary = false;
	bool m_placedFinalCF = false;
	double m_bpmChangeTime = -1;
	int m_bpmChangeValue = -1;
	double m_lastBpmTick = 0.0;

	int m_notesHit = 0;
	int m_notesTotal = 0;

	int m_scr_tick = 0;
	double m_initialCrossfade = -2.0;

	const int TICKS_PER_BEAT = 4;
};

#endif // GENERATOR_H
