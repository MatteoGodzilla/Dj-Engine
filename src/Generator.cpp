#include "Generator.h"
#include "Note.h"
#include <iostream>

/// Ignore This file. This is what remains of my attempt to make note generation


Generator::Generator()
{
    sf::Texture loader;
    if(loader.loadFromFile("res/red note.png"))
    {
        m_tex_red = loader;
    }
    else std::cout << "Error loading texture" <<std::endl;
    if(loader.loadFromFile("res/blue note.png"))
    {
        m_tex_blue = loader;
    }
    else std::cout << "Error loading texture" <<std::endl;
    if(loader.loadFromFile("res/green note.png"))
    {
        m_tex_green = loader;
    }
    else std::cout << "Error loading texture" <<std::endl;

    m_note_times = {2.0,5.0};

    /*Note n2(4.0,"res/blue note.png");
    m_note_arr.push_back(n2);

    Note n3(5.0,"res/green note.png");
    m_note_arr.push_back(n3);*/
}

void Generator::click(float time)
{
    for(int i = 0; i < m_note_times.size(); i++)
    {
        m_note_arr[i].click(time);
    }
}

void Generator::gen(float time)
{
    for(int i = 0; i < m_note_times.size(); i++)
    {
        if(time +1.0 >= m_note_times[i])
        {
            std::cout << "Generated" << std::endl;
            Note temp(i);
            temp.set_sprite(m_tex_blue);
            m_note_arr.push_back(temp);
            m_note_times.erase(m_note_times.begin());
        }
    }
}

void Generator::tick(float time)
{
    for(int i = 0; i < m_note_times.size(); i++)
    {
        m_note_arr[i].tick(time);
    }
}

void Generator::render(sf::RenderWindow& window)
{
    for(int i = 0; i < m_note_times.size(); i++)
    {
        m_note_arr[i].render(window);
    }
}

Generator::~Generator()
{
    //dtor
}
