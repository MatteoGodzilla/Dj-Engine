#include "Player.h"
#include <iostream>

Player::Player(Generator &g) :m_gen(g){
}

void Player::key(sf::Event e){
    if(e.type == sf::Event::KeyPressed) {
        if(e.key.code == GREEN_CODE) {
            if(m_green == false) {
                m_just_g = true;
            }
            m_green = true;
        } else if(e.key.code == RED_CODE) {
            if (m_red == false) {
                m_just_r = true;
            }
            m_red = true;
        } else if(e.key.code == BLUE_CODE) {
            if(m_blue == false) {
                m_just_b = true;
            }
            m_blue = true;
        } else if(e.key.code == CROSS_L_CODE) {
            m_cross = 0;
        } else if(e.key.code == CROSS_R_CODE) {
            m_cross = 2;
        } else std::cout << "Just pressed something unmapped" << std::endl;
    }
    if(e.type == sf::Event::KeyReleased) {
        if(e.key.code == GREEN_CODE) {
            m_green = false;
        } else if(e.key.code == RED_CODE) {
            m_red = false;
        } else if(e.key.code == BLUE_CODE) {
            m_blue = false;
        } else if(e.key.code == CROSS_L_CODE && m_cross == 0) {
            m_cross = 1;
        } else if(e.key.code == CROSS_R_CODE && m_cross == 2) {
            m_cross = 1;
        }
    }
}

void Player::tick(float time,std::vector<Note>&v) {
    //Note check
    for(int i = 0; i < v.size(); i++) {
        Note selected = v[i];
        if(selected.getType() == TAP_R && m_just_r) {
            selected.click(time);
            selected.destroy();
        } else if(selected.getType() == TAP_G && m_just_g) {
            selected.click(time);
            selected.destroy();
        } else if(selected.getType() == TAP_B && m_just_b) {
            selected.click(time);
            selected.destroy();
        }
    }
    if(m_cross != m_gen.getCross())std::cout << "cross" << std::endl;
}


Player::~Player() {
    //dtor
}
