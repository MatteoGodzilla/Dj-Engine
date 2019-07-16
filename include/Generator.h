#ifndef GENERATOR_H
#define GENERATOR_H
#include <vector>
#include "Note.h"
#include <iostream>

class Generator {
public:
    Generator();
    virtual ~Generator();
    void tick(float time,std::vector<Note> &v);
    int getCross();
protected:

private:
    std::vector<float> m_note_times;
    std::vector<int> m_note_types;
    int m_crossfader = 1;
};

#endif // GENERATOR_H
