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
    bool m_combo_reset = false;
protected:

private:
    float m_note_diff = 0.0f;
    float m_event_diff = 0.0f;
    void pushNote(float time, int type);
    void pushEvent(float time, int type);
    std::vector<float> m_note_times;
    std::vector<int> m_note_types;
    std::vector<float>m_event_times;
    std::vector<int>m_event_types;
};

#endif // GENERATOR_H
