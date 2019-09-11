#ifndef GENERATOR_H
#define GENERATOR_H
#include "Note.h"
#include <vector>
#include <iostream>

class Generator {
public:
    Generator();
    virtual ~Generator();
    void tick(double time,std::vector<Note> &v,std::vector<Note>&ev);
    bool m_combo_reset = false;
	bool m_eu_start = false;
	bool m_eu_check = false;
protected:

private:
    double m_time = 0.0;
    void pushNote(double time, int type);
    void pushEvent(double time, int type);
    std::vector<double> m_note_times;
    std::vector<int> m_note_types;
    std::vector<double>m_event_times;
    std::vector<int>m_event_types;

};

#endif // GENERATOR_H
