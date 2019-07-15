#include "Generator.h"

Generator::Generator()
{
    for(int i = 0; i < 3; i++)
    {
        m_note_times.push_back(0.2f*i+2.0);
        m_note_types.push_back(TAP_G);
    }
}

void Generator::tick(float time,std::vector<Note> &v)
{
    for(int i = 0; i < m_note_times.size(); i++)
    {
        if(time +1.0 >= m_note_times[i])
        {
            //note generation
            Note temp(m_note_times[i],m_note_types[i]);
            v.push_back(temp);
            m_note_times.erase(m_note_times.begin()+i);
            m_note_types.erase(m_note_types.begin()+i);
        }
    }
    for (int i = 0; i < v.size(); i++)
    {
        //remove if outside hit area
        if(time >= v[i].getMilli()+0.2f || v[i].getActive() == false)
        {
            v.erase(v.begin()+i);
        }
    }
}

Generator::~Generator()
{
    //dtor
}
