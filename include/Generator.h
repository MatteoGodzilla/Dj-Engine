#ifndef GENERATOR_H
#define GENERATOR_H
#include <vector>
#include "Note.h"

class Generator
{
    public:
        Generator();
        virtual ~Generator();
        void tick(float time,std::vector<Note> &v);
    protected:

    private:
        std::vector<float> m_note_times;
        std::vector<int> m_note_types;
};

#endif // GENERATOR_H
