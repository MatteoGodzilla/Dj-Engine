#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Note.h"
#include "Rendr.h"
#include "Generator.h"
#include "Player.h"

float global_time;
sf::RenderWindow window;

std::vector<Note> note_arr;
Rendr rendr(window);
Generator gen;
Player player;

void check_events(sf::Event e)
{
    if(e.type == sf::Event::Closed)window.close();
    if(e.key.code == sf::Keyboard::Escape)window.close();
    player.tick(e,global_time);
}

void tick(){
    gen.tick(global_time,note_arr);
    rendr.pollState(player);
}

void render(){

    //render notes from 'note_arr' based on time, all to window
    rendr.render(global_time,note_arr);
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
