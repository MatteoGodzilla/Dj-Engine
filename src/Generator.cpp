#include "Generator.h"

Generator::Generator() {
    pushNote(2.0f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);

    pushEvent(0.2f,CROSS_L);
    pushNote(0.0f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);

    pushEvent(0.2f,CROSS_R);
    pushNote(0.0f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);

    pushEvent(0.2f,CROSS_L);
    pushNote(0.0f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);

    pushEvent(0.2f,CROSS_R);
    pushNote(0.0f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);

    pushEvent(0.2f,CROSS_C);
    pushNote(0.0f,TAP_G);
    pushNote(0.2f,TAP_R);
    pushNote(0.2f,TAP_B);
    pushNote(0.2f,TAP_R);

    pushNote(0.5f,TAP_G);
    pushNote(0.0f,TAP_R);
    pushNote(0.0f,TAP_B);

}

void Generator::tick(float time,std::vector<Note> &v,std::vector<Note>&ev) {
    m_combo_reset = false;
    size_t note_s = m_note_times.size();

    for(size_t i = 0; i < note_s ; ++i) {
        //note generation
        Note temp(m_note_times.at(0),m_note_types.at(0));
        v.push_back(temp);
        m_note_times.erase(m_note_times.begin());
        m_note_types.erase(m_note_types.begin());
    }
    size_t event_s = m_event_times.size();

    for(size_t i = 0;i < event_s ; ++i){
        Note e(m_event_times.at(0),m_event_types.at(0),true);
        ev.push_back(e);
        m_event_times.erase(m_event_times.begin());
        m_event_types.erase(m_event_types.begin());
    }
    if(!v.empty()){
        for (size_t i = v.size(); i-- > 0 ;) {
            //remove if outside hit area
            v.at(i).tick(time);
            if(v.at(i).getDead()){
                if(v.at(i).getTouched() == false){
                    m_combo_reset = true;
                    v.at(i).click(time);
                }
                v.erase(v.begin()+i);
                break;
            }
        }
    }

    if(!ev.empty()){
        for (size_t i = ev.size(); i-- > 0;){
            ev.at(i).tick(time);
            int type = ev.at(i).getType();
            if(type==SCR_G_START){
                int e = -1;
                for(size_t j = ev.size(); j-- > 0;){
                    if(i != j && ev.at(j).getType()==SCR_G_END && time >= ev.at(j).getMilli()+0.2f)e = j;
                }
                if(e != -1){
                    ev.erase(ev.begin()+e);
                    ev.erase(ev.begin()+i);
                }
            }
            if(type==SCR_B_START){
                int e = -1;
                for(size_t j = ev.size();j-- > 0;){
                    if(i != j && ev.at(j).getType()==SCR_B_END && time >= ev.at(j).getMilli()+0.2f)e = j;
                }
                if(e != -1){
                    ev.erase(ev.begin()+e);
                    ev.erase(ev.begin()+i);
                }
            }
            if (type == CROSS_L || type == CROSS_C || type == CROSS_R){
                for(size_t j = ev.size(); j-- > i;){
                    int next_type = ev.at(j).getType();
                    float next_time = ev.at(j).getMilli();
                    if(next_type == CROSS_L || next_type == CROSS_C || next_type == CROSS_R){
                        if(j > i && next_time+0.15f <= time){
                            if(!ev.at(i).getTouched()){
                                m_combo_reset = true;
                            }
                            ev.erase(ev.begin()+i);
                        }
                    }
                }
            }
        }
    }
}

void Generator::pushNote(float time, int type){
    m_time += time;
    m_note_times.push_back(m_time);
    m_note_types.push_back(type);
}

void Generator::pushEvent(float time, int type){
    m_time += time;
    m_event_times.push_back(m_time);
    m_event_types.push_back(type);
}

Generator::~Generator() {
    m_note_times.clear();
    m_note_types.clear();
    m_event_times.clear();
    m_event_types.clear();
    //dtor
}
