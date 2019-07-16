#include "Rendr.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Rendr::Rendr(sf::RenderWindow &w):m_window(w) {
    sf::Texture loader;
    if(loader.loadFromFile("res/texture.png")) {
        m_tex = loader;
    }

    sf::Font font_loader;
    if(font_loader.loadFromFile("res/NotoSans-Regular.ttf")) {
        m_font = font_loader;
    }
    m_time_counter.setFont(m_font);
    m_time_counter.setPosition(0.0,0.0);
    m_time_counter.setColor(sf::Color::White);


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

    m_red_click.setTexture(m_tex);
    m_red_click.setTextureRect(sf::IntRect(440,400,440,440));
    m_red_click.setOrigin(220,220);
    m_red_click.setScale(0.2,0.2);
    m_red_click.setPosition(512.0,500.0);

    m_green_click.setTexture(m_tex);
    m_green_click.setTextureRect(sf::IntRect(0,400,440,440));
    m_green_click.setOrigin(220,220);
    m_green_click.setScale(0.2,0.2);
    m_green_click.setPosition(424.0,500.0);

    m_blue_click.setTexture(m_tex);
    m_blue_click.setTextureRect(sf::IntRect(880,400,440,440));
    m_blue_click.setOrigin(220,220);
    m_blue_click.setScale(0.2,0.2);
    m_blue_click.setPosition(600.0,500.0);

    //scale setup
    m_scl_start = sf::Vector2f(0.1,0.1);
    m_scl_end = sf::Vector2f(0.2,0.2);
    m_scl_vel = (m_scl_end-m_scl_start)/1.0f;
}

void Rendr::render(float time,std::vector<Note> &v) {
    m_time_counter.setString("Time:"+std::to_string(time));

    if(m_red)m_red_click.setScale(0.15,0.15);
    else m_red_click.setScale(0.2,0.2);

    if(m_green)m_green_click.setScale(0.15,0.15);
    else m_green_click.setScale(0.2,0.2);

    if(m_blue)m_blue_click.setScale(0.15,0.15);
    else m_blue_click.setScale(0.2,0.2);

    if(m_cross == 0) {
        m_green_click.setPosition(338.0,500.0);
        m_blue_click.setPosition(600.0,500.0);
    } else if(m_cross == 1) {
        m_green_click.setPosition(424.0,500.0);
        m_blue_click.setPosition(600.0,500.0);
    } else if(m_cross == 2) {
        m_green_click.setPosition(424.0,500.0);
        m_blue_click.setPosition(688.0,500.0);
    }

    m_window.draw(m_tray1);
    m_window.draw(m_tray2);
    m_window.draw(m_red_click);
    m_window.draw(m_green_click);
    m_window.draw(m_blue_click);
    m_window.draw(m_time_counter);

    for(int i = 0 ; i < v.size(); i++) {
        Note temp = v[i];
        float dt = temp.getMilli()-time;

        sf::Sprite sprite;
        sprite.setTexture(m_tex);
        if(temp.getType() == TAP_R) {
            sprite.setTextureRect(sf::IntRect(400,0,400,400));
            m_start = sf::Vector2f(512.0,200.0);
            m_end = sf::Vector2f(512.0,500.0);
            m_vel = (m_end-m_start)/1.0f;
        } else if (temp.getType() == TAP_G) {
            sprite.setTextureRect(sf::IntRect(0,0,400,400));
            m_start = sf::Vector2f(472.0,200.0);
            m_end = sf::Vector2f(422.0,500.0);
            m_vel = (m_end-m_start)/1.0f;
        } else if (temp.getType() == TAP_B) {
            sprite.setTextureRect(sf::IntRect(800,0,400,400));
            m_start = sf::Vector2f(552.0,200.0);
            m_end = sf::Vector2f(602.0,500.0);
            m_vel = (m_end-m_start)/1.0f;
        }
        sprite.setOrigin(200.0,200.0);

        if(dt >= -0.2 && dt <= 1.0) {
            //position and scale calculations
            sf::Vector2f pos = m_start + m_vel*(1.0f-dt);
            sf::Vector2f scl = m_scl_start +m_scl_vel*(1.0f-dt);

            //sprite drawn on screen
            if(temp.getActive() == true){
                sprite.setScale(scl);
                sprite.setPosition(pos);
                m_window.draw(sprite);
            }
        }

    }
    //std::cout <<"------"<<std::endl;
}

void Rendr::pollState(Player& p) {
    m_red = p.m_red;
    m_blue = p.m_blue;
    m_green = p.m_green;
    m_cross = p.m_cross;
}

Rendr::~Rendr() {
    //dtor
}
