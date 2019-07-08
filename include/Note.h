#ifndef NOTE_H
#define NOTE_H
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class Note
{
    public:
        Note();
        Note(float milli);
        virtual ~Note();
        void click(float time);
        void render(sf::RenderWindow& w);
        void set_sprite(sf::Texture t);
        void tick(float time);
    protected:

    private:
        float m_milli;
        float m_dt;
        sf::Sprite m_sprite;
        sf::Texture m_tex;
        sf::Vector2f m_start;
        sf::Vector2f m_end;
        sf::Vector2f m_vel;
        sf::Vector2f m_scale;
};

#endif // NOTE_H
