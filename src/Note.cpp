#include "Note.h"
#include <math.h>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

Note::Note(){

}

Note::Note(float milli)
{
    //set hit time
    m_milli = milli;
}

void Note::click(float time)
{
    //if it's too early, don't count
    if(m_milli-time <= 0.300)
    {
        if(abs(m_milli-time)<=0.100)std::cout << "Hit:  " << time <<std::endl;
        else std::cout << "Miss: " << time <<std::endl;

    }
}

float Note::getMilli(){
    return m_milli;
}

Note::~Note()
{
}
