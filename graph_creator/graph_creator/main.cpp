
#include <SFML/Graphics.hpp>

#include <iostream>
#include <sstream>
#include "graph_program.hpp"

int main()
{
    sf::ContextSettings sett;
    sett.antialiasingLevel = 8;

    sf::RenderWindow App( sf::VideoMode(1280, 972), "graph creator", sf::Style::Titlebar, sett);
    App.setFramerateLimit(30);

    sf::Font ft;
    ft.loadFromFile("Fonts/button_font.otf");

    graph_program Graph("sdf", ft ,&App);

    while(App.isOpen())
    {
        sf::Event events;

        while(App.pollEvent(events))
        {
            if(events.type == sf::Event::Closed)
            {
                App.close();
            }
            else if(events.type == sf::Event::Resized)
            {
                float w = static_cast<float>(events.size.width);
                float h = static_cast<float>(events.size.height);
                App.setView(sf::View(sf::Vector2f(w / 2.0, h / 2.0), sf::Vector2f(w, h)));
            }

            if(!Graph.Is_Active())
            {
                switch(events.type)
                {
                    case sf::Event::KeyPressed:

                        switch(events.key.code)
                        {
                            case sf::Keyboard::T: Graph.clear_graph();          break;
                            case sf::Keyboard::P: Graph.load_from_matrix("qw"); break;
                            case sf::Keyboard::C: Graph.create_adjmatr();       break;
                            case sf::Keyboard::Y: Graph.save_graph("fest_save");      break;
                            case sf::Keyboard::L: Graph.clear_path();           break;
                            case sf::Keyboard::M: Graph.Set_Active_Menu();      break;

                            default: break;
                        }

                    default: break;
                }
            }
         
            Graph.update(events);
        }
        App.clear();
        Graph.draw();
        App.display();
    }
    return 0;
}
