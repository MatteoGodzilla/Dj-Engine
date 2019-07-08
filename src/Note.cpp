#include "Note.h"
#include <math.h>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

Note::Note(){

}

Note::Note(float milli)
{
    //general setup
    m_milli = milli;
    m_start = sf::Vector2f(512.0,100.0);
    m_end = sf::Vector2f(512.0,500.0);
    m_scale = sf::Vector2f(0.05,0.05);

    sf::Vector2f ds = m_end-m_start;
    m_dt=2.0;
    m_vel = sf::Vector2f(ds.x/m_dt,ds.y/m_dt);
    //sprite setup
    m_sprite.setScale(m_scale);
    //center
    m_sprite.setOrigin(200.0,200.0);
}

void Note::set_sprite(sf::Texture t){
    m_tex = t;
    std::cout << "changed texture" << std::endl;
    m_sprite.setTexture(m_tex);
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

void Note::render(sf::RenderWindow& w)
{
    w.draw(m_sprite);
}

void Note::tick(float time)
{
    //S = S0 +vt
    if(m_milli > time){
        float x = m_start.x+m_vel.x*time;
        float y = m_start.y+m_vel.y*time;
        m_sprite.setPosition(x,y);

        sf::Vector2f v = sf::Vector2f(0.1,0.1)/m_dt;
        m_scale = sf::Vector2f(0.1,0.1)+ sf::Vector2f(0.05,0.05)*time;
        m_sprite.setScale(m_scale);
    }
    //if note is past the hit zone, remove it
    //if(time-m_milli >= 0)del();
}

Note::~Note()
{
}
