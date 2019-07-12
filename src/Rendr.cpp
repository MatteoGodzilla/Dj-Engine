#include "Rendr.h"
#include "Note.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Rendr::Rendr()
{
    sf::Texture loader;
    if(loader.loadFromFile("res/texture.png"))
    {
        m_tex = loader;
    }

    m_tray1.setTexture(m_tex);
    m_tray1.setTextureRect(sf::IntRect(1320,0,440,880));
    m_tray1.setOrigin(220,440);
    m_tray1.setRotation(90);
    m_tray1.setScale(0.2,0.2);
    m_tray1.setPosition(382.0,500.0);

    m_tray2.setTexture(m_tex);
    m_tray2.setTextureRect(sf::IntRect(1320,0,440,880));
    m_tray2.setOrigin(220,400);
    m_tray2.setRotation(90);
    m_tray2.setScale(0.2,0.2);
    m_tray2.setPosition(652.0,500.0);

    m_clicker.setTexture(m_tex);
    m_clicker.setTextureRect(sf::IntRect(0,400,1320,440));
    m_clicker.setOrigin(660,220);
    m_clicker.setScale(0.2,0.2);
    m_clicker.setPosition(512.0,500.0);

    //scale setup
    m_scl_start = sf::Vector2f(0.1,0.1);
    m_scl_end = sf::Vector2f(0.2,0.2);
    m_scl_vel = (m_scl_end-m_scl_start)/1.0f;
}

void Rendr::render(float time,std::vector<Note> &v,sf::RenderWindow &w)
{

    w.draw(m_tray1);
    w.draw(m_tray2);
    w.draw(m_clicker);


    for(int i = v.size(); i > 0; i--)
    {
        Note temp = v[i];
        float dt = temp.getMilli()-time;

        sf::Sprite sprite;
        sprite.setTexture(m_tex);
        if(temp.m_type == TAP_R)
        {
            sprite.setTextureRect(sf::IntRect(400,0,400,400));
            m_start = sf::Vector2f(512.0,200.0);
            m_end = sf::Vector2f(512.0,500.0);
            m_vel = (m_end-m_start)/1.0f;
        }
        else if (temp.m_type == TAP_G)
        {
            sprite.setTextureRect(sf::IntRect(0,0,400,400));
            m_start = sf::Vector2f(472.0,200.0);
            m_end = sf::Vector2f(422.0,500.0);
            m_vel = (m_end-m_start)/1.0f;
        }
        else if (temp.m_type == TAP_B)
        {
            sprite.setTextureRect(sf::IntRect(800,0,400,400));
            m_start = sf::Vector2f(552.0,200.0);
            m_end = sf::Vector2f(602.0,500.0);
            m_vel = (m_end-m_start)/1.0f;
        }
        sprite.setOrigin(200.0,200.0);

        if(dt >= 0.0 && dt <= 1.0 )
        {
            //position and scale calculations
            sf::Vector2f pos = m_start + m_vel*(1.0f-dt);
            sf::Vector2f scl = m_scl_start +m_scl_vel*(1.0f-dt);

            //sprite drawn on screen
            sprite.setScale(scl);
            sprite.setPosition(pos);
            w.draw(sprite);
        }

    }
    //std::cout <<"------"<<std::endl;
}

Rendr::~Rendr()
{
    //dtor
}
