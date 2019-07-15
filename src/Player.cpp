#include "Player.h"
#include <iostream>

Player::Player()
{
    //ctor
}

void Player::tick(sf::Event e, float time){
    if(e.type == sf::Event::KeyPressed){
        if(e.key.code == GREEN_CODE){
            if(m_green == false){
                std::cout << "Just pressed Green" << std::endl;
                //click
            }
            m_green = true;
        }else if(e.key.code == RED_CODE){
            if (m_red == false) {
                std::cout << "Just pressed Red" << std::endl;
                //click
            }
            m_red = true;
        }else if(e.key.code == BLUE_CODE){
            if(m_blue == false) {
                std::cout << "Just pressed Blue" << std::endl;
                //click
            }
            m_blue = true;
        }else std::cout << "Just pressed something" << std::endl;
    }
    if(e.type == sf::Event::KeyReleased){
        if(e.key.code == GREEN_CODE){
            m_green = false;
        }else if(e.key.code == RED_CODE){
            m_red = false;
        }else if(e.key.code == BLUE_CODE){
            m_blue = false;
        }
    }
}


Player::~Player()
{
    //dtor
}
