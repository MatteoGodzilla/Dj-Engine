#include <iostream>
#include <SFML/Graphics.hpp>
#include "Note.h"

float global_time;
sf::RenderWindow window;

//texture to load
sf::Texture blue_note,red_note,green_note;

//Single note at 2 seconds
Note n(2.0,"res/red note.png");

void check_events(sf::Event e)
{
    if(e.type == sf::Event::Closed)window.close();
    //keyboard pressed
    if(e.type == sf::Event::KeyPressed){
        if(e.key.code == sf::Keyboard::Escape)window.close();
        else if (e.key.code == sf::Keyboard::Space){
            n.click(global_time);
        }

    }
    else if(e.type == sf::Event::KeyReleased){
        if (e.key.code == sf::Keyboard::Space){
        }

    }
}

void tick(){
    n.tick(global_time);
}
void render(){
    n.render(window);
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
