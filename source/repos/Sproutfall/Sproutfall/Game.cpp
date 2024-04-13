#include <SFML/Graphics.hpp>
#include "Player.h"
#include "BackgroundLayer.h"
#include "SceneManager.h"
#include "Asteroid.h"
#include <random>
#include <ctime>

enum Scenes{TitleScreen, Space, Sky, Ground, Win};
int Game();
int main()
{
    Game();
    return 0;
}

int Game()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Sproutfall");
    SceneManager sceneManager(window.getSize().x, window.getSize().y, &window);
    sceneManager.loadScene(Sky);
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::Resized)
                sceneManager.handleResize(event.size.width, event.size.height);
            sceneManager.handleInput(&event);
        }
        float tf = clock.getElapsedTime().asSeconds();
        if (tf > 0.15)
            tf = 0.15;
        sceneManager.Update(tf);
        clock.restart();
        window.clear();

        window.draw(sceneManager);
        window.display();
    }
    return 0;
}