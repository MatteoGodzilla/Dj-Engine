#ifndef RENDR_H
#define RENDR_H
#include "Note.h"
#include "Player.h"
#include <SFML/Graphics.hpp>

class Rendr
{
    public:
        Rendr(sf::RenderWindow& w);
        void render(float time,std::vector<Note> &v);
        void pollState(Player& p);
        virtual ~Rendr();

    protected:

    private:
        sf::Texture m_tex;

        sf::Sprite m_tray1,m_tray2,m_clicker;

        sf::Vector2f m_start;
        sf::Vector2f m_end;
        sf::Vector2f m_vel;
        sf::Vector2f m_scl_start;
        sf::Vector2f m_scl_end;
        sf::Vector2f m_scl_vel;
        sf::RenderWindow &m_window;
        bool m_red,m_green,m_blue;
};

#endif // RENDR_H
