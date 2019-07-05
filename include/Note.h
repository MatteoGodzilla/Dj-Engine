#ifndef NOTE_H
#define NOTE_H
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

class Note
{
    public:
        Note(float milli,std::string file);
        virtual ~Note();
        void click(float time);
        void render(sf::RenderWindow& w);
        void set_sprite(sf::Texture t);
        void tick(float time);
        void del();
    protected:

    private:
        float m_milli;
        float m_x;
        float m_y;
        sf::Texture m_tex;
        sf::Sprite m_sprite;
};

#endif // NOTE_H
