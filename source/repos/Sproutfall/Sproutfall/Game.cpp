#include <SFML/Graphics.hpp>
#include "Player.h"
int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 960), "Sproutfall");
    Player player(&window);
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            player.handleInput(&event);
        }
        float tf = clock.getElapsedTime().asSeconds();
        player.Update(tf);
        clock.restart();
        window.clear();
        window.draw(player);
        window.display();
    }

    return 0;
}