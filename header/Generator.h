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
    virtual ~Generator();
    void tick(double time,std::vector<Note> &v,std::vector<Note>&ev);
	void textParser(std::vector<Note>& v, std::vector<Note>& ev);
	void binaryParser(std::vector<Note>& v, std::vector<Note>& ev);
	void bpm(double time, std::vector<double>& arr);
    bool m_combo_reset = false;
	bool m_eu_start = false;
	bool m_eu_check = false;
	int m_bpm = 1;
protected:

private:
    double m_time = 0.0;
    void pushNote(double time, int type);
    void pushEvent(double time, int type);
    std::vector<double> m_note_times;
    std::vector<int> m_note_types;
    std::vector<double>m_event_times;
    std::vector<int>m_event_types;
	std::ifstream m_chart;
	bool m_isTimeRelative = true;
	double m_bpmChangeTime = -1;
	int m_bpmChangeValue = -1;
	double m_lastBpmTick = 0.0;
};

#endif // GENERATOR_H
