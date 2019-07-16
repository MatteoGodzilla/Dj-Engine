#include "Generator.h"

Generator::Generator() {
    for(int i = 0; i < 10; i++) {
        m_note_times.push_back(0.2f*i+2.0);
        m_note_times.push_back(0.2f*i+2.0);
        m_note_times.push_back(0.2f*i+2.0);
        m_note_types.push_back(TAP_R);
        m_note_types.push_back(TAP_G);
        m_note_types.push_back(TAP_B);
    }
    m_note_times.push_back(4.5f);
    m_note_types.push_back(CROSS_L);
}

void Generator::tick(float time,std::vector<Note> &v) {
    for(int i = 0; i < m_note_times.size(); i++) {
        int type = m_note_types[i];
        if(type == TAP_R || type == TAP_G || type == TAP_B) {
            if(time +1.0 >= m_note_times[i]) {
                //note generation
                Note temp(m_note_times[i],m_note_types[i]);
                v.push_back(temp);
                m_note_times.erase(m_note_times.begin()+i);
                m_note_types.erase(m_note_types.begin()+i);
            }
        } else if(type == CROSS_L || type == CROSS_C || type == CROSS_R) {
            if(time >= m_note_times[i]){
                m_crossfader = type-3;
                Note temp(m_note_times[i],m_note_types[i]);
                v.push_back(temp);
                m_note_times.erase(m_note_times.begin()+i);
                m_note_types.erase(m_note_types.begin()+i);
                std::cout << m_crossfader << std::endl;
            }
        }

    }
    for (int i = 0; i < v.size(); i++) {
        //remove if outside hit area
        if(time >= v[i].getMilli()+0.2f || v[i].getActive() == false) {
            v.erase(v.begin()+i);
        }
    }
}

int Generator::getCross(){
    return m_crossfader;
}

Generator::~Generator() {
    //dtor
}
