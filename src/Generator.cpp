#include "Generator.h"

Generator::Generator() {

    m_event_times.push_back(2.0f);
    m_event_types.push_back(CROSS_R);

    m_note_times.push_back(4.0f);
    m_note_types.push_back(TAP_B);
}

void Generator::tick(float time,std::vector<Note> &v,std::vector<Note>&ev) {
    for(int i = 0; i < m_note_times.size(); i++) {
        if(time +1.0 >= m_note_times[i]) {
            //note generation
            Note temp(m_note_times[i],m_note_types[i]);
            v.push_back(temp);
            m_note_times.erase(m_note_times.begin()+i);
            m_note_types.erase(m_note_types.begin()+i);
        }
    }
    for(int i = 0;i < m_event_times.size();i++){
        if(m_event_types[i] == SCR_G_START)m_scr_g = true;
        if(m_event_types[i] == SCR_G_END)m_scr_g = false;
        Note temp(m_event_times[i],m_event_types[i]);
        ev.push_back(temp);
        m_event_times.erase(m_event_times.begin()+i);
        m_event_types.erase(m_event_types.begin()+i);
    }

    for (int i = 0; i < v.size(); i++) {
        //remove if outside hit area
        if(time >= v[i].getMilli()+0.2f  || v[i].getActive() == false) {
            v[i].click(time);
            v.erase(v.begin()+i);
        }
    }
    for (int i = 0; i < ev.size(); i++){
        if(time >= ev[i].getMilli() || ev[i].getActive() == false){
            ev.erase(ev.begin()+i);
        }
    }
}

int Generator::getCross(){
    return m_crossfader;
}

Generator::~Generator() {
    //dtor
}
