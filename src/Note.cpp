#include "Note.h"
#include <iostream>

Note::Note() {
}

Note::Note(float milli,int ty) {
    //set hit time
    m_milli = milli;
    m_type = ty;
    m_active = true;
}

void Note::click(float time) {
    //if it's too early, don't count
    if(m_milli-time <= 0.300) {
        if(abs(m_milli-time)<=0.300 && m_active == true) {
            std::cout << "Hit: " << m_type << " at "<<time  <<std::endl;
            m_active = false;
        } else std::cout << "Miss: " << m_type << " at "<< time  <<std::endl;

    }
}

float Note::getMilli() {
    return m_milli;
}

int Note::getType() {
    return m_type;
}

bool Note::getActive() {
    return m_active;
}

void Note::destroy() {
    m_active = false;
}
Note::~Note() {
}
