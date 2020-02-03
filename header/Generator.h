#pragma once
#include "Note.h"
#include "SongScanner.h"
#include <vector>
#include <deque>
#include <iostream>
#include <fstream>
#include <string>

class Generator {
public:
	Generator();
	void init(SongEntry entry);
    void tick(double time,std::vector<Note> &v,std::vector<Note>&ev, std::vector<Note>& c);
	//void textParser(std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& c);
	void addNotesToBuffer(std::vector<Note>& v, std::vector<Note>& ev, std::vector<Note>& c);
	void initialLoad();
	void bpm(double time, std::vector<double>& arr);
	int getNotesTotal();
	int getNotesHit();
	SongEntry getSongEntry();
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
	std::deque<Note>m_allTaps;
	std::deque<Note>m_allEvents;
	std::deque<Note>m_allCross;

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

	SongEntry m_songEntry;
};