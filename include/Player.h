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
    CROSS_R_CODE = sf::Keyboard::D,
    SCRATCH_UP = sf::Keyboard::W,
    SCRATCH_DOWN = sf::Keyboard::S
};

class Player {
public:
    Player();
    void key(sf::Event e,float time,std::vector<Note>& v,std::vector<Note>&ev);
    virtual ~Player();
    bool m_red = false;
    bool m_green = false;
    bool m_blue = false;
    int m_cross = 1;
    int getScore();
    int getCombo();
    int getMult();
    void pollState(Generator &g);
protected:

private:
    bool m_scr_up = false;
    bool m_scr_down = false;
    int m_score = 0;
    int m_mult = 1;
    int m_combo = 0;
    int m_scr_tick = 0;
    float m_past_tap = -1;
};

#endif // Player_H
