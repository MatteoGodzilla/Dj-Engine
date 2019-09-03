#include "Note.h"

Note::Note(double milli,int ty,bool ev){
    //set hit time
    m_hit_window = 0.15;
    m_milli = milli;
    m_type = ty;
    m_is_event = ev;
}

void Note::click(double time) {
    if(m_hittable){
        std::cout << "Hit :" << m_is_event << "\\"<< m_type << " at " << time << std::endl;
        m_touched = true;
    }else {
        std::cout << "Miss:" << m_is_event << "\\"<< m_type << " at " << time << std::endl;
    }
    if(!m_is_event){
        m_dead = true;
    }
}

void Note::tick(double time){
    if(time +1.0 >= m_milli && time <= m_milli+m_hit_window){
        m_render = true;
    }else{
        m_render = false;
    }

    if(m_milli-time <= m_hit_window && time-m_milli <= m_hit_window){
        //std::cout << "hittable " << m_milli-time <<  std::endl;
        m_hittable = true;
    }else m_hittable = false;

    if(time > m_milli+m_hit_window && !m_is_event){
        m_dead = true;
    }
}

double Note::getMilli() {
    return m_milli;
}

int Note::getType() {
    return m_type;
}

bool Note::getRender() {
    return m_render;
}

bool Note::getHit(){
    return m_hittable;
}
bool Note::getTouched(){
    return m_touched;
}

bool Note::getIsEvent(){
    return m_is_event;
}
bool Note::getDead(){
    return m_dead;
}

int Note::getLanMod(){
    return m_lan_mod;
}
void Note::setLanMod(int i){
    m_lan_mod = i;
}

Note::~Note() {
}
