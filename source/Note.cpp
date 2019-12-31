#include "Note.h"

Note::Note(double milli,int ty, double length, bool ev){
    //set hit time
    m_hit_window = hitWindow;
    m_milli = milli;
    m_type = ty;
    m_isEvent = ev;
	m_length = length;
}

//when the player clicks the corrisponding button
void Note::click(double time) {
    if(m_hittable){
        std::cout << "Hit :" << m_isEvent << "\\"<< m_type << " at " << time << std::endl;
        m_touched = true;
    }else {
        std::cout << "Miss:" << m_isEvent << "\\"<< m_type << " at " << time << std::endl;
    }
    if(!m_isEvent){
        m_dead = true;
    }
}

//updating note every frame
void Note::tick(double time){
    if(m_milli-time <= m_hit_window && time-m_milli <= m_hit_window){
        m_hittable = true;
    }else m_hittable = false;

    if(time > m_milli+m_hit_window && !m_isEvent){
        m_dead = true;
    }
}

//utility function to return variables
double Note::getMilli() {
    return m_milli;
}

double Note::getLength()
{
	return m_length;
}

int Note::getType() {
    return m_type;
}

bool Note::getHit(){
    return m_hittable;
}
bool Note::getTouched(){
    return m_touched;
}

void Note::setTouched(bool value){
	m_touched = value;
}

bool Note::getIsEvent(){
    return m_isEvent;
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