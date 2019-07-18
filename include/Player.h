#ifndef Player_H
#define Player_H
#include <SFML/Window.hpp>
#include <vector>
#include "Generator.h"

enum K {
    GREEN_CODE = sf::Keyboard::J,
    RED_CODE = sf::Keyboard::K,
    BLUE_CODE = sf::Keyboard::L,
    CROSS_L_CODE = sf::Keyboard::A,
    CROSS_R_CODE = sf::Keyboard::D
};

class Player {
public:
    Player(Generator &g);
    void key(sf::Event e);
    void tick(float time,std::vector<Note>& v);
    virtual ~Player();
    bool m_red = false;
    bool m_green = false;
    bool m_blue = false;
    int m_cross = 1;
protected:

private:
    bool m_just_r = false;
    bool m_just_g = false;
    bool m_just_b = false;
    bool m_just_cross_l = false;
    bool m_just_cross_r = false;
    Generator &m_gen;
};

#endif // Player_H
