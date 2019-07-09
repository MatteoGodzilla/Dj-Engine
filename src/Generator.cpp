#include "Generator.h"
#include "Note.h"
#include <iostream>

Generator::Generator()
{
    for(int i = 1; i < 1000; i++){
        m_note_times.push_back(0.02f*i+2.0);
    }
}

void Generator::tick(float time,std::vector<Note> &v)
{
    for(int i = 0; i < m_note_times.size(); i++)
    {
        if(time +1.0 >= m_note_times[i])
        {
            //note generation
            Note temp(m_note_times[i]);
            v.push_back(temp);
            m_note_times.erase(m_note_times.begin()+i);

        }
    }
    for (int i = 0;i < v.size(); i++){
        //remove if outside hit area
        if(time >= v[i].getMilli()+0.2f){
            v.erase(v.begin()+i);
        }
    }
}

Generator::~Generator()
{
    //dtor
}
