#include "Note.h"

double hitWindow = 0.15;
double highwayTimeVisible = 1.0;

Note::Note(double milli,int ty,bool ev, float extra){
    //set hit time
    m_hit_window = hitWindow;
    m_milli = milli;
    m_type = ty;
    m_is_event = ev;
	m_extraData = extra;
}

//when the player clicks the corrisponding button
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

//updating note every frame
void Note::tick(double time){
    if(time + highwayTimeVisible >= m_milli && time <= m_milli+m_hit_window){
        m_render = true;
    }else{
        m_render = false;
    }

    if(m_milli-time <= m_hit_window && time-m_milli <= m_hit_window){
        m_hittable = true;
    }else m_hittable = false;

    if(time > m_milli+m_hit_window && !m_is_event){
        m_dead = true;
    }
}

//utility function to return variables
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

float Note::getExtra()
{
	return m_extraData;
}

Note::~Note() {
}
