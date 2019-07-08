#include <iostream>
#include <SFML/Graphics.hpp>
#include "Note.h"

float global_time;
sf::RenderWindow window;

Note n1(2.0);

void check_events(sf::Event e)
{
    if(e.type == sf::Event::Closed)window.close();
    //keyboard pressed
    if(e.type == sf::Event::KeyPressed){
        if(e.key.code == sf::Keyboard::Escape)window.close();
        else if (e.key.code == sf::Keyboard::Space){
            n1.click(global_time);
        }

    }
}

void tick(){
    n1.tick(global_time);

}
void render(){
    n1.render(window);
}

int main()
{
    sf::Clock c;
    window.create(sf::VideoMode(1024, 600), "Dj-Engine");

    sf::Texture temp;
    temp.loadFromFile("res/green note.png");
    n1.set_sprite(temp);

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
