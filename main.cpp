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
std::vector<Note> event_arr;
Rendr rendr(window);
Generator gen;
Player player(gen);

void check_events(sf::Event e) {
    if(e.type == sf::Event::Closed)window.close();
    if(e.key.code == sf::Keyboard::Escape)window.close();
    player.key(e);
}

void tick() {
    gen.tick(global_time,note_arr,event_arr);
    rendr.pollState(player,gen);
    player.tick(global_time, note_arr);
}

void render() {
    rendr.clicker();
    rendr.events(global_time,event_arr);
    rendr.notes(global_time,note_arr);

}

int main() {
    sf::Clock c;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 2;
    window.create(sf::VideoMode(1024, 600), "Dj-Engine",sf::Style::Default,settings);
    while (window.isOpen()) {
        global_time += c.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
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
