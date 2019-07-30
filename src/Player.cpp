#include "Player.h"
#include <iostream>

Player::Player() {
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
            if(m_cross != 0){
                m_just_cross_l = true;
            }else {
                m_just_cross_l = false;
            }
            m_cross = 0;
        }
        if(e.key.code == CROSS_R_CODE) {
            if(m_cross != 2){
                m_just_cross_r = true;
            }else {
                m_just_cross_r = false;
            }
            m_cross = 2;
        }
        if(e.key.code == SCRATCH_UP){
            m_scr_up = true;
        }
        if(e.key.code == SCRATCH_DOWN){
            m_scr_down = true;
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
        if(e.key.code == SCRATCH_UP){
            m_scr_up = false;
        }
        if(e.key.code == SCRATCH_UP){
            m_scr_down = false;
        }
    }


}

void Player::tick(float time,std::vector<Note>&v,std::vector<Note>&ev) {
    //Note check
    for(size_t i = 0; i < v.size(); ++i) {
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
        if(v[i].getType()==SCR_G_UP && m_green && m_scr_up) {
            v[i].click(time);
            break;
        }if(v[i].getType()==SCR_G_DOWN && m_green && m_scr_down) {
            v[i].click(time);
            break;
        }if(v[i].getType()==SCR_G_ANY && m_green && (m_scr_down || m_scr_up)) {
            v[i].click(time);
            break;
        }if(v[i].getType()==SCR_B_UP && m_blue && m_scr_up) {
            v[i].click(time);
            break;
        }if(v[i].getType()==SCR_B_DOWN && m_blue && m_scr_down) {
            v[i].click(time);
            break;
        }if(v[i].getType()==SCR_B_ANY && m_blue && (m_scr_down || m_scr_up)) {
            v[i].click(time);
            break;
        }

    }
    for(size_t i = 0; i < ev.size(); ++i){
        if(ev[i].getType()==CROSS_L && m_just_cross_l) {
            ev[i].click(time);
            break;
        }
        if(ev[i].getType()==CROSS_R && m_just_cross_r) {
            ev[i].click(time);
            break;
        }
        if(ev[i].getType()==CROSS_C && m_cross == 1) {
            ev[i].click(time);
            break;
        }
    }
    if(m_just_r) m_just_r = false;
    if(m_just_g) m_just_g = false;
    if(m_just_b) m_just_b = false;
    if(m_just_cross_l) m_just_cross_l = false;
    if(m_just_cross_r) m_just_cross_r = false;
}


Player::~Player() {
    //dtor
}
