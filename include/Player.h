#ifndef Player_H
#define Player_H
#include <SFML/Window.hpp>

enum K
{
    GREEN_CODE = sf::Keyboard::J,
    RED_CODE = sf::Keyboard::K,
    BLUE_CODE = sf::Keyboard::L
};

class Player
{
public:
    Player();
    void tick(sf::Event e, float time);
    virtual ~Player();
    bool m_red = false;
    bool m_green = false;
    bool m_blue = false;
protected:

private:
};

#endif // Player_H
