#include "Note.h"
#include <math.h>
#include <string>
#include <iostream>
#include <SFML/Graphics.hpp>

Note::Note(float milli,std::string file)
{
    m_milli = milli;
    //temporary texture
    sf::Texture loader;
    if(loader.loadFromFile(file)){
        //successful loading
        //copy texture to local variable
        m_tex = loader;
        m_tex.setSmooth(true);
        //sprite setup
        m_sprite.setTexture(m_tex);
        m_sprite.setScale(0.2,0.2);
        m_sprite.setOrigin(200.0,200.0);
    }else{
        std::cerr << "Error loading texture file" << std::endl;
    }
}

void Note::click(float time){
    //if it's too early, don't count
    if(m_milli-time <= 0.300){
        if(abs(m_milli-time)<=0.100)std::cout << "Hit:  " << time <<std::endl;
        else std::cout << "Miss: " << time <<std::endl;
        del();
    }
}

void Note::render(sf::RenderWindow& w){
    w.draw(m_sprite);
}

void Note::tick(float time){
    m_sprite.setPosition(512.0,100.0+200*time);
    //if note is past the hit zone, remove it
    if(time-m_milli >= 0.200)del();
}

void Note::del(){
    //TODO:implement note remove
}

Note::~Note()
{
}
