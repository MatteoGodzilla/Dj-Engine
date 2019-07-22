#ifndef GENERATOR_H
#define GENERATOR_H
#include "Note.h"
#include <vector>
#include <iostream>

class Generator {
public:
    Generator();
    virtual ~Generator();
    void tick(float time,std::vector<Note> &v,std::vector<Note>&ev);
    int getCross();
    bool m_scr_g = false;
    bool m_scr_b = false;
protected:

private:
    std::vector<float> m_note_times;
    std::vector<int> m_note_types;
    std::vector<float>m_event_times;
    std::vector<int>m_event_types;
    int m_crossfader = 1;

};

#endif // GENERATOR_H
