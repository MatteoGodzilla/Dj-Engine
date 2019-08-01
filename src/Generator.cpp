#include "Generator.h"

Generator::Generator() {
    pushNote(2.0f,TAP_G);
    pushNote(2.2f,TAP_R);
    pushNote(2.4f,TAP_B);
    pushNote(2.6f,TAP_R);
    pushNote(2.8f,TAP_G);
    pushNote(3.0f,TAP_R);
    pushNote(3.2f,TAP_B);
    pushNote(3.4f,TAP_R);
    pushNote(3.6f,TAP_G);
    pushNote(3.8f,TAP_R);
    pushNote(4.0f,TAP_B);
    pushNote(4.2f,TAP_R);
    pushNote(4.4f,TAP_G);
    pushNote(4.6f,TAP_R);
    pushNote(4.8f,TAP_B);
    pushNote(5.0f,TAP_R);

    pushNote(5.2f,TAP_G);
    pushNote(5.4f,TAP_R);
    pushNote(5.6f,TAP_B);
    pushNote(5.8f,TAP_R);
    pushNote(6.0f,TAP_G);
    pushNote(6.2f,TAP_R);
    pushNote(6.4f,TAP_B);
    pushNote(6.6f,TAP_R);
    pushNote(6.8f,TAP_G);
    pushNote(7.0f,TAP_R);
    pushNote(7.2f,TAP_B);
    pushNote(7.4f,TAP_R);
    pushNote(7.6f,TAP_G);
    pushNote(7.8f,TAP_R);
    pushNote(8.0f,TAP_B);
    pushNote(8.2f,TAP_R);

    pushNote(8.4f,TAP_G);
    pushNote(8.6f,TAP_R);
    pushNote(8.8f,TAP_B);
    pushNote(9.0f,TAP_R);
    pushNote(9.2f,TAP_G);
    pushNote(9.4f,TAP_R);
    pushNote(9.6f,TAP_B);
    pushNote(9.8f,TAP_R);
    pushNote(10.0f,TAP_G);
    pushNote(10.2f,TAP_R);
    pushNote(10.4f,TAP_B);
    pushNote(10.6f,TAP_R);
    pushNote(10.8f,TAP_G);
    pushNote(11.0f,TAP_R);
    pushNote(11.2f,TAP_B);
    pushNote(11.4f,TAP_R);

    pushNote(11.6f,TAP_G);
    pushNote(11.8f,TAP_R);
    pushNote(12.0f,TAP_B);
    pushNote(12.2f,TAP_R);
    pushNote(12.4f,TAP_G);
    pushNote(12.6f,TAP_R);
    pushNote(12.8f,TAP_B);
    pushNote(13.0f,TAP_R);
    pushNote(13.2f,TAP_G);
    pushNote(13.4f,TAP_R);
    pushNote(13.6f,TAP_B);
    pushNote(13.8f,TAP_R);
    pushNote(14.0f,TAP_G);
    pushNote(14.2f,TAP_R);
    pushNote(14.4f,TAP_B);
    pushNote(14.6f,TAP_R);

    pushNote(15.1f,TAP_G);
    pushNote(15.1f,TAP_R);
    pushNote(15.1f,TAP_B);

    pushEvent(5.2f,CROSS_L);
    pushEvent(8.4f,CROSS_R);
    pushEvent(11.6f,CROSS_L);
    pushEvent(13.2f,CROSS_R);
    pushEvent(14.0f,CROSS_C);

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
        int type = ev.at(i).getType();
        if(ev.at(i).getActive() == false && !(type == CROSS_L || type == CROSS_C || type == CROSS_R )){
            ev.erase(ev.begin()+i);
        }else if(type==SCR_G_START){
            int e = -1;
            for(size_t j = 0; j < ev.size();++j){
                if(ev.at(j).getType()==SCR_G_END && time >= ev.at(j).getMilli()+0.2f)e = j;
            }
            if(e != -1){
                ev.erase(ev.begin()+i);
                ev.erase(ev.begin()+e);
            }
        }else if (type == CROSS_L || type == CROSS_C || type == CROSS_R){
            for(size_t j = i+1; j < ev.size();++j){
                int next_type = ev.at(j).getType();
                if((next_type == CROSS_L || next_type == CROSS_C || next_type == CROSS_R) && ev.at(j).getMilli()<=time+0.2f){
                    ev.erase(ev.begin()+i);
                }
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
