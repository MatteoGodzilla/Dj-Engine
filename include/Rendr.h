#ifndef RENDR_H
#define RENDR_H
#include "Note.h"
#include "Player.h"
#include "Generator.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Rendr {
public:
    Rendr(sf::RenderWindow& w);
    void clicker();
    void notes(float time,std::vector<Note> &v);
    void events(float time,std::vector<Note>&ev);
    void lanes(float time, std::vector<Note>&ev);
    void pollState(Player& p,Generator &g);
    virtual ~Rendr();

protected:

private:
    sf::Texture m_tex;

    sf::Sprite m_tray1,m_tray2;
    sf::Sprite m_red_click,m_green_click,m_blue_click;

    sf::Font m_font;
    sf::Text m_time_counter;

    sf::Vector2f m_start;
    sf::Vector2f m_end;
    sf::Vector2f m_vel;
    sf::Vector2f m_scl_start;
    sf::Vector2f m_scl_end;
    sf::Vector2f m_scl_vel;
    sf::RenderWindow &m_window;

    bool m_red,m_green,m_blue;
    int m_play_cross;
    bool m_scr_g = false;
    bool m_scr_b = false;

    int m_ren_cross = 1;
    int resolution = 200;
    std::vector<int> m_lanes;

};

#endif // RENDR_H
