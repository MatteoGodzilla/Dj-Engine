#include "Player.h"
#include <iostream>

Player::Player(Generator &g) :m_gen(g) {
}

void Player::key(sf::Event e) {

    if(e.type == sf::Event::KeyPressed) {
        if(e.key.code == GREEN_CODE) {
            if(m_green == false) {
                m_just_g = true;
            } else {
                m_just_g = false;
            }
            m_green = true;
        }
        if(e.key.code == RED_CODE) {
            if(m_red == false) {
                m_just_r = true;
            } else {
                m_just_r = false;
            }
            m_red = true;
        }
        if(e.key.code == BLUE_CODE) {
            if(m_blue == false) {
                m_just_b = true;
            } else {
                m_just_b = false;
            }
            m_blue = true;
        }
        if(e.key.code == CROSS_L_CODE) {
            m_cross = 0;
        }
        if(e.key.code == CROSS_R_CODE) {
            m_cross = 2;
        }
    } else if(e.type == sf::Event::KeyReleased) {
        if(e.key.code == GREEN_CODE) {
            m_green = false;
        }
        if(e.key.code == RED_CODE) {
            m_red = false;
        }
        if(e.key.code == BLUE_CODE) {
            m_blue = false;
        }
        if(e.key.code == CROSS_L_CODE && m_cross == 0) {
            m_cross = 1;
        }
        if(e.key.code == CROSS_R_CODE && m_cross == 2) {
            m_cross = 1;
        }
    }


}

void Player::tick(float time,std::vector<Note>&v) {
    //Note check
    for(int i = 0; i < v.size(); i++) {
        if(v[i].getType()==TAP_R && m_just_r) {
            v[i].click(time);
            break;
        }
        if(v[i].getType()==TAP_G && m_just_g) {
            v[i].click(time);
            break;
        }
        if(v[i].getType()==TAP_B && m_just_b) {
            v[i].click(time);
            break;
        }
        if(v[i].getType()==CROSS_L && m_just_cross_l) {
            v[i].click(time);
            break;
        }
        if(v[i].getType()==CROSS_R && m_just_cross_r) {
            v[i].click(time);
            break;
        }
        if(v[i].getType()==CROSS_C && m_cross == 1) {
            v[i].click(time);
            break;
        }

    }
    if(m_just_r) m_just_r = false;
    if(m_just_g) m_just_g = false;
    if(m_just_b) m_just_b = false;
    if(m_just_cross_l) m_just_cross_l = false;
    if(m_just_cross_r) m_just_cross_r = false;

    //std::cout << m_green << "\\" << m_red << "\\" << m_blue << "\\" << m_cross << std::endl;
}


Player::~Player() {
    //dtor
}
