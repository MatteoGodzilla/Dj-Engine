#include "Rendr.h"
#include "Note.h"
#include <iostream>

Rendr::Rendr()
{
    sf::Texture loader;
    if(loader.loadFromFile("res/blue note.png")){
        m_blue = loader;
    }else std::cout << "Error loading texture" << std::endl;

    if(loader.loadFromFile("res/red note.png")){
        m_red = loader;
    }else std::cout << "Error loading texture" << std::endl;

    if(loader.loadFromFile("res/green note.png")){
        m_green = loader;
    }else std::cout << "Error loading texture" << std::endl;

    //position setup
    m_start = sf::Vector2f(512.0,200.0);
    m_end = sf::Vector2f(512.0,500.0);
    m_vel = (m_end-m_start)/1.0f;

    m_scl_start = sf::Vector2f(0.1,0.1);
    m_scl_end = sf::Vector2f(0.2,0.2);
    m_scl_vel = (m_scl_end-m_scl_start)/1.0f;
}

void Rendr::render(float time,std::vector<Note> &v,sf::RenderWindow &w){
    for(int i = v.size(); i > 0;i--){
        Note temp = v[i];
        float dt = temp.getMilli()-time;
        if(dt >= 0.0 && dt <= 1.0 ){
            //position and scale calculations
            sf::Vector2f pos = m_start + m_vel*(1.0f-dt);
            sf::Vector2f scl = m_scl_start +m_scl_vel*(1.0f-dt);

            //sprite drawn on screen
            sf::Sprite sprite;
            sprite.setTexture(m_red);
            sprite.setOrigin(200.0,200.0);
            sprite.setScale(scl);
            sprite.setPosition(pos);
            w.draw(sprite);

        }

    }

}

Rendr::~Rendr()
{
    //dtor
}
