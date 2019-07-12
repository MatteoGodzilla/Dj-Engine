#ifndef RENDR_H
#define RENDR_H
#include "Note.h"

class Rendr
{
    public:
        Rendr();
        void render(float time,std::vector<Note> &v,sf::RenderWindow &w);
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
};

#endif // RENDR_H
