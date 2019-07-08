#ifndef GENERATOR_H
#define GENERATOR_H
#include <vector>
#include "Note.h"

///Ignore this file. Nothing to see here

class Generator
{
    public:
        Generator();
        virtual ~Generator();
        void tick(float time);
        void click(float time);
        void render(sf::RenderWindow& window);
        void gen(float time);
    protected:

    private:
        Note n1;
        sf::Texture m_tex_red;
        sf::Texture m_tex_green;
        sf::Texture m_tex_blue;
        std::vector<float> m_note_times;
        std::vector<Note> m_note_arr;
};

#endif // GENERATOR_H
