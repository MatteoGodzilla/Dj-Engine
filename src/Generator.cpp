#include "Generator.h"

Generator::Generator() {
    pushEvent(2.0f,CROSS_L);
    pushEvent(3.0f,CROSS_C);
    pushEvent(4.0f,CROSS_R);
    pushEvent(5.0f,CROSS_L);
    pushEvent(6.0f,CROSS_R);
    pushEvent(7.0f,CROSS_C);
}

void Generator::tick(float time,std::vector<Note> &v,std::vector<Note>&ev) {
    for(size_t i = 0; i < m_note_times.size(); ++i) {
        if(time +1.0 >= m_note_times.at(i)) {
            //note generation
            Note temp(m_note_times.at(i),m_note_types.at(i));
            v.push_back(temp);
            m_note_times.erase(m_note_times.begin()+i);
            m_note_types.erase(m_note_types.begin()+i);
        }
    }
    for(size_t i = 0;i < m_event_times.size();++i){
        if(time +1.0f >= m_event_times.at(i)){
            Note temp(m_event_times.at(i),m_event_types.at(i));
            ev.push_back(temp);
            m_event_times.erase(m_event_times.begin()+i);
            m_event_types.erase(m_event_types.begin()+i);
        }
    }
    for (size_t i = 0; i < v.size(); ++i) {
        //remove if outside hit area
        if(time >= v.at(i).getMilli()+0.2f  || v.at(i).getActive() == false) {
            v.at(i).click(time);
            v.erase(v.begin()+i);
        }
    }
    for (size_t i = 0; i < ev.size(); ++i){
        if(ev.at(i).getActive() == false){
            ev.erase(ev.begin()+i);
        }else if(ev.at(i).getType()==SCR_G_START){
            int e = -1;
            for(size_t j = 0; j < ev.size();++j){
                if(ev.at(j).getType()==SCR_G_END && time >= ev.at(j).getMilli()+0.2f)e = j;
            }
            if(e != -1){
                ev.erase(ev.begin()+i);
                ev.erase(ev.begin()+e);
            }
        }
    }
}

void Generator::pushNote(float time, int type){
    m_note_times.push_back(time);
    m_note_types.push_back(type);
}

void Generator::pushEvent(float time, int type){
    m_event_times.push_back(time);
    m_event_types.push_back(type);

}

Generator::~Generator() {
    m_note_times.clear();
    m_note_types.clear();
    m_event_times.clear();
    m_event_types.clear();
    //dtor
}
