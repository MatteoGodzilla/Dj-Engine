#ifndef NOTE_H
#define NOTE_H
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

enum type{TAP_R,TAP_G,TAP_B};

class Note
{
    public:
        Note();
        Note(float milli,int ty);
        virtual ~Note();
        void click(float time);
        void set_sprite(sf::Texture t);
        void tick(float time);
        float getMilli();
        int m_type;
    protected:

    private:
        float m_milli;

        sf::Vector2f m_start;
        sf::Vector2f m_end;
        sf::Vector2f m_vel;
        sf::Vector2f m_scale;
};

#endif // NOTE_H
