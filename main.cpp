#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Note.h"
#include "Rendr.h"
#include "Generator.h"

float global_time;
sf::RenderWindow window;

std::vector<Note> note_arr;
Rendr rendr;
Generator gen;

void check_events(sf::Event e)
{
    if(e.type == sf::Event::Closed)window.close();
    //keyboard pressed
    if(e.type == sf::Event::KeyPressed){
        if(e.key.code == sf::Keyboard::Escape)window.close();
        else if (e.key.code == sf::Keyboard::Space){
            float reference_time = note_arr[0].getMilli();
            for(int i = 0; i < note_arr.size();i++){
                if(note_arr[i].getMilli() == reference_time)note_arr[i].click(global_time);
                else break;
            }
        }

    }
}

void tick(){
    gen.tick(global_time,note_arr);
}

void render(){
    //render notes from 'note_arr' based on time, all to window
    rendr.render(global_time,note_arr,window);
}

int main()
{
    sf::Clock c;
    window.create(sf::VideoMode(1024, 600), "Dj-Engine");

    while (window.isOpen())
    {
        global_time += c.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            //separated check of events into function
            check_events(event);
        }
        tick();
        //render cycle
        window.clear();
        render();
        window.display();

    }

    return 0;
}
