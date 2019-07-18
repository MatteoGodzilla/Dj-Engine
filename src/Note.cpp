#include "Note.h"

Note::Note(float milli,int ty) {
    //set hit time
    m_hit_window = 0.1;
    m_milli = milli;
    m_type = ty;
    m_active = true;
}

void Note::click(float time) {
    //if it's too early, don't count
    if (time >= m_milli+m_hit_window){
        std::cout << "Miss: " << m_type << " at "<< time  <<std::endl;
        m_active = false;
    } else if(m_milli-time <= m_hit_window*2 ) {
        if(abs(m_milli-time)<= m_hit_window && m_active == true) {
            std::cout << "Hit: " << m_type << " at "<<time  <<std::endl;
            m_active = false;
        }
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
